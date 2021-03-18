#ifndef RMW_MICROXRCEDDS_CONFIG_H
#define RMW_MICROXRCEDDS_CONFIG_H

#include <uxr/client/config.h>

/* #undef RMW_UXRCE_TRANSPORT_UDP */
/* #undef RMW_UXRCE_TRANSPORT_SERIAL */
#define RMW_UXRCE_TRANSPORT_CUSTOM_SERIAL
/* #undef RMW_UXRCE_TRANSPORT_IPV4 */
/* #undef RMW_UXRCE_TRANSPORT_IPV6 */
/* #undef RMW_UXRCE_TRANSPORT_USE_REFS */
#define RMW_UXRCE_TRANSPORT_USE_XML
/* #undef RMW_UXRCE_ALLOW_DYNAMIC_ALLOCATIONS */
/* #undef RMW_UXRCE_GRAPH */

#ifdef RMW_UXRCE_TRANSPORT_UDP
    #define RMW_UXRCE_MAX_TRANSPORT_MTU UXR_CONFIG_UDP_TRANSPORT_MTU
    #define RMW_UXRCE_DEFAULT_UDP_IP ""
    #define RMW_UXRCE_DEFAULT_UDP_PORT ""
#elif defined(RMW_UXRCE_TRANSPORT_SERIAL) || defined(RMW_UXRCE_TRANSPORT_CUSTOM_SERIAL)
    #define RMW_UXRCE_MAX_TRANSPORT_MTU UXR_CONFIG_SERIAL_TRANSPORT_MTU
    #define RMW_UXRCE_DEFAULT_SERIAL_DEVICE ""
#endif

/* #undef RMW_UXRCE_STREAM_HISTORY_INPUT */
/* #undef RMW_UXRCE_STREAM_HISTORY_OUTPUT */
#if defined(RMW_UXRCE_STREAM_HISTORY_INPUT) && defined(RMW_UXRCE_STREAM_HISTORY_OUTPUT)
#undef RMW_UXRCE_STREAM_HISTORY_INPUT
#undef RMW_UXRCE_STREAM_HISTORY_OUTPUT
#define RMW_UXRCE_STREAM_HISTORY_INPUT 
#define RMW_UXRCE_STREAM_HISTORY_OUTPUT 
#else
#define RMW_UXRCE_STREAM_HISTORY_INPUT 4
#define RMW_UXRCE_STREAM_HISTORY_OUTPUT 4
#endif

#define RMW_UXRCE_ENTITY_CREATION_DESTROY_TIMEOUT 1000
#define RMW_UXRCE_PUBLISH_RELIABLE_TIMEOUT 1000

#define RMW_UXRCE_MAX_HISTORY 1
#define RMW_UXRCE_MAX_INPUT_BUFFER_SIZE (RMW_UXRCE_MAX_TRANSPORT_MTU * RMW_UXRCE_STREAM_HISTORY_INPUT)
#define RMW_UXRCE_MAX_OUTPUT_BUFFER_SIZE (RMW_UXRCE_MAX_TRANSPORT_MTU * RMW_UXRCE_STREAM_HISTORY_OUTPUT)

#define RMW_UXRCE_MAX_SESSIONS 1
#define RMW_UXRCE_MAX_NODES 1
#define RMW_UXRCE_MAX_PUBLISHERS 10 + 1
#define RMW_UXRCE_MAX_SUBSCRIPTIONS 5
#define RMW_UXRCE_MAX_SERVICES 1
#define RMW_UXRCE_MAX_CLIENTS 1
#define RMW_UXRCE_MAX_TOPICS -1

#if RMW_UXRCE_MAX_TOPICS == -1
#define RMW_UXRCE_MAX_TOPICS_INTERNAL RMW_UXRCE_MAX_PUBLISHERS + RMW_UXRCE_MAX_SUBSCRIPTIONS
#else
#define RMW_UXRCE_MAX_TOPICS_INTERNAL RMW_UXRCE_MAX_TOPICS
#endif

#define RMW_UXRCE_NODE_NAME_MAX_LENGTH 128
#define RMW_UXRCE_TOPIC_NAME_MAX_LENGTH 100
#define RMW_UXRCE_TYPE_NAME_MAX_LENGTH 128
#define RMW_UXRCE_XML_BUFFER_LENGTH 600
#define RMW_UXRCE_REF_BUFFER_LENGTH 100

#endif // RMW_MICROXRCEDDS_CONFIG_H
