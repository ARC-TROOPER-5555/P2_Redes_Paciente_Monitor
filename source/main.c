#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_RAW && LWIP_NETCONN && LWIP_DHCP && LWIP_DNS

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_phy.h"

#include "lwip/api.h"
#include "lwip/apps/mqtt.h"
#include "lwip/dhcp.h"
#include "lwip/netdb.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#include "lwip_mqtt_id.h"

#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
#include "fsl_device_registers.h"

#include "FreeRTOS.h"
#include "task.h"
#include "ADC.h"
#include "ctype.h"
#include "stdio.h"

#ifndef configMAC_ADDR
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x10 \
    }
#endif

#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS
#define EXAMPLE_MDIO_OPS enet_ops
#define EXAMPLE_PHY_OPS phyksz8081_ops
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#define EXAMPLE_MQTT_SERVER_HOST "broker.hivemq.com"
#define EXAMPLE_MQTT_SERVER_PORT 1883
#define INIT_THREAD_STACKSIZE 1024
#define INIT_THREAD_PRIO DEFAULT_THREAD_PRIO
#define APP_THREAD_STACKSIZE 1024
#define APP_THREAD_PRIO DEFAULT_THREAD_PRIO

static void connect_to_mqtt(void *ctx);

static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};
static mqtt_client_t *mqtt_client;
static char client_id[40];
static ip_addr_t mqtt_addr;
static volatile bool connected = false;

static const struct mqtt_connect_client_info_t mqtt_client_info = {
    .client_id   = (const char *)&client_id[0],
    .client_user = NULL,
    .client_pass = NULL,
    .keep_alive  = 100,
    .will_topic  = NULL,
    .will_msg    = NULL,
    .will_qos    = 0,
    .will_retain = 0,
#if LWIP_ALTCP && LWIP_ALTCP_TLS
    .tls_config = NULL,
#endif
};

static void mqtt_topic_subscribed_cb(void *arg, err_t err)
{
    const char *topic = (const char *)arg;

    if (err == ERR_OK)
        PRINTF("Subscribed to the topic \"%s\".\r\n", topic);
    else
        PRINTF("Failed to subscribe to the topic \"%s\": %d.\r\n", topic, err);
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    LWIP_UNUSED_ARG(arg);
    PRINTF("Received %u bytes from the topic \"%s\": \"", tot_len, topic);
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    LWIP_UNUSED_ARG(arg);
    for (int i = 0; i < len; i++)
    {
        PRINTF("%c", isprint(data[i]) ? (char)data[i] : '.');
    }
    if (flags & MQTT_DATA_FLAG_LAST)
        PRINTF("\"\r\n");
}

static void mqtt_subscribe_topics(mqtt_client_t *client)
{
    static const char *topics[] = {"lwip_topic/Pc", "lwip_topic/Temp", "monitor/alarm", "monitor/msg", "lwip_topic/medicina"};
    int qos[] = {0, 0, 0, 0, 0};

    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb,
                            LWIP_CONST_CAST(void *, &mqtt_client_info));

    for (int i = 0; i < ARRAY_SIZE(topics); i++)
    {
        err_t err = mqtt_subscribe(client, topics[i], qos[i], mqtt_topic_subscribed_cb, LWIP_CONST_CAST(void *, topics[i]));

        if (err == ERR_OK)
            PRINTF("Subscribing to the topic \"%s\" with QoS %d...\r\n", topics[i], qos[i]);
        else
            PRINTF("Failed to subscribe to the topic \"%s\" with QoS %d: %d.\r\n", topics[i], qos[i], err);
    }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    const struct mqtt_connect_client_info_t *client_info = (const struct mqtt_connect_client_info_t *)arg;

    connected = (status == MQTT_CONNECT_ACCEPTED);

    switch (status)
    {
        case MQTT_CONNECT_ACCEPTED:
            PRINTF("MQTT client \"%s\" connected.\r\n", client_info->client_id);
            mqtt_subscribe_topics(client);
            break;
        case MQTT_CONNECT_DISCONNECTED:
        case MQTT_CONNECT_TIMEOUT:
            PRINTF("MQTT client \"%s\" connection failed (status: %d).\r\n", client_info->client_id, status);
            sys_timeout(1000, connect_to_mqtt, NULL);
            break;
        default:
            PRINTF("MQTT connection error: %d\r\n", status);
            sys_timeout(10000, connect_to_mqtt, NULL);
            break;
    }
}

static void connect_to_mqtt(void *ctx)
{
    LWIP_UNUSED_ARG(ctx);
    PRINTF("Connecting to MQTT broker at %s...\r\n", ipaddr_ntoa(&mqtt_addr));
    mqtt_client_connect(mqtt_client, &mqtt_addr, EXAMPLE_MQTT_SERVER_PORT, mqtt_connection_cb,
                        LWIP_CONST_CAST(void *, &mqtt_client_info), &mqtt_client_info);
}

static void mqtt_message_published_cb(void *arg, err_t err)
{
    const char *topic = (const char *)arg;
    if (err == ERR_OK)
        PRINTF("Published to the topic \"%s\".\r\n", topic);
    else
        PRINTF("Failed to publish to the topic \"%s\": %d.\r\n", topic, err);
}

volatile adc_opciones opciones;

static void publish_message(void *ctx)
{
    static const char *topic_1 = "lwip_topic/Pc";
    static char message_Pc[64];

    static const char *topic_2 = "lwip_topic/Temp";
    static char message_Temp[64];

    LWIP_UNUSED_ARG(ctx);

    //Pc
    snprintf(message_Pc, sizeof(message_Pc), "%d\r\n", opciones.real_pulso);
    //Temp
    snprintf(message_Temp, sizeof(message_Temp), "%d\r\n",opciones.real_temperatura);

    mqtt_publish(mqtt_client, topic_1, message_Pc, strlen(message_Pc), 1, 0, mqtt_message_published_cb, (void *)topic_1);

    mqtt_publish(mqtt_client, topic_2, message_Temp, strlen(message_Temp), 1, 0, mqtt_message_published_cb, (void *)topic_2);

}

void vThread_pasiente(void *pvParameters)
{
    while (1)
    {
        opciones = ADC_Pc_Temp();
        // Enviar por MQTT
        if (connected)
        {
            tcpip_callback(publish_message, NULL);
        }

        vTaskDelay(pdMS_TO_TICKS(20000)); // 1 segundo entre mediciones
    }
}

static void app_thread(void *arg)
{
    struct netif *netif = (struct netif *)arg;
    struct dhcp *dhcp;
    err_t err;

    PRINTF("Getting IP address from DHCP...\r\n");

    do {
        if (netif_is_up(netif))
            dhcp = netif_dhcp_data(netif);
        else
            dhcp = NULL;
        sys_msleep(20U);
    } while ((dhcp == NULL) || (dhcp->state != DHCP_STATE_BOUND));

    PRINTF("IPv4 Address     : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
    PRINTF("IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif->netmask));
    PRINTF("IPv4 Gateway     : %s\r\n", ipaddr_ntoa(&netif->gw));

    if (ipaddr_aton(EXAMPLE_MQTT_SERVER_HOST, &mqtt_addr) && IP_IS_V4(&mqtt_addr))
        err = ERR_OK;
    else {
        PRINTF("Resolving \"%s\"...\r\n", EXAMPLE_MQTT_SERVER_HOST);
        err = netconn_gethostbyname(EXAMPLE_MQTT_SERVER_HOST, &mqtt_addr);
    }

    if (err == ERR_OK)
    {
        err = tcpip_callback(connect_to_mqtt, NULL);
        if (err != ERR_OK)
            PRINTF("Failed to connect via tcpip_thread: %d\r\n", err);
    }
    else
        PRINTF("Failed to obtain IP address: %d.\r\n", err);

    for (int i = 0; i < 5;)
    {
        if (connected)
        {
            err = tcpip_callback(publish_message, NULL);
            if (err != ERR_OK)
                PRINTF("Publish error: %d\r\n", err);
            i++;
        }
        sys_msleep(1000U);
    }

    // MQTT finalizado → Iniciar ADC task
    ADC_init_P();
    ADC_init_T();
    xTaskCreate(vThread_pasiente, "thread_pasiente", 256, NULL, 3, NULL);

    vTaskDelete(NULL);
}

static void generate_client_id(void)
{
    uint32_t mqtt_id[MQTT_ID_SIZE];
    get_mqtt_id(&mqtt_id[0]);
    int res = snprintf(client_id, sizeof(client_id), "nxp_%08lx%08lx%08lx%08lx", mqtt_id[3], mqtt_id[2], mqtt_id[1], mqtt_id[0]);
    if ((res < 0) || (res >= sizeof(client_id)))
    {
        PRINTF("snprintf failed: %d\r\n", res);
        while (1);
    }
}

static void stack_init(void *arg)
{
    static struct netif netif;
    ip4_addr_t ipaddr, netmask, gw;
    ethernetif_config_t enet_config = {.phyHandle = &phyHandle, .macAddress = configMAC_ADDR};

    LWIP_UNUSED_ARG(arg);
    generate_client_id();

    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;
    IP4_ADDR(&ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&gw, 0U, 0U, 0U, 0U);

    tcpip_init(NULL, NULL);

    LOCK_TCPIP_CORE();
    mqtt_client = mqtt_client_new();
    UNLOCK_TCPIP_CORE();
    if (mqtt_client == NULL)
    {
        PRINTF("mqtt_client_new() failed.\r\n");
        while (1);
    }

    netifapi_netif_add(&netif, &ipaddr, &netmask, &gw, &enet_config, EXAMPLE_NETIF_INIT_FN, tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);
    netifapi_dhcp_start(&netif);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" MQTT client example\r\n");
    PRINTF("************************************************\r\n");

    if (sys_thread_new("app_task", app_thread, &netif, APP_THREAD_STACKSIZE, APP_THREAD_PRIO) == NULL)
        LWIP_ASSERT("stack_init(): Task creation failed.", 0);

    vTaskDelete(NULL);
}

int main(void)
{
    SYSMPU_Type *base = SYSMPU;
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    base->CESR &= ~SYSMPU_CESR_VLD_MASK;
    SIM->SOPT2 |= SIM_SOPT2_RMIISRC_MASK;

    if (sys_thread_new("main", stack_init, NULL, INIT_THREAD_STACKSIZE, INIT_THREAD_PRIO) == NULL)
        LWIP_ASSERT("main(): Task creation failed.", 0);

    vTaskStartScheduler();
    return 0;
}

#endif
