#ifndef DIAMETER_CONSTANTS_H
#define DIAMETER_CONSTANTS_H

#define DIAMETER_PORT             3868 //default port
#define DIAMETER_VERSION          1 //default version
#define DIAMETER_MAX_MSG_SIZE     8192 //4K
#define DIAMETER_HEADER_SIZE      20 //

// Header flags
#define DIAMETER_REQUEST_FLAG                 0x80  
#define DIAMETER_PROXIABLE_FLAG               0x40
#define DIAMETER_ERROR_FLAG                   0x20
#define DIAMETER_POTENTIAL_RETRANSMITTED_FLAG 0x10

// AVP flags
#define DIAMETER_AVP_VENDOR_SPECIFIC_FLAG 0x80
#define DIAMETER_AVP_MANDATORY_FLAG       0x40
#define DIAMETER_AVP_PBIT_FLAG            0x20
#define DIAMETER_AVP_INVALID_FLAG         0x00

//Vendor-Ids
#define CAMIANT_VENDOR_ID    21274
#define CISCO_VENDOR_ID      9
#define ETSI_VENDOR_ID       13019
#define THREE_GPP_VENDOR_ID  10415
#define THREE_GPP2_VENDOR_ID 5535
#define SIEMENS_VENDOR_ID    4329
#define VZW_VENDOR_ID        12951
#define ATT_VENDOR_ID        971
#define JUNIPER_VENDOR_ID    2636
  
#define TWINIT                    30 //Watchdog initial value in seconds
#define TCINIT                    3 //Defaulting to 3 seconds for Tc value (reconnect interval)
#define NUM_DWA_BEFORE_CONN_REUSE 3 //number of successful DWR/DWA before connection re-use
#define PRODUCT_NAME_DEFAULT      "3GPP_PCEF"

//DiameterScheduler for handling timeouts, timed events, etc...
#define RESPONSE_TIMEOUT      5  //5 secs
#define SCHEDULER_NUM_THREADS 5
#define SCHEDULER_GRANULARITY 500  // .5 sec
  
//Basic app ids
#define BASE_APP_ID           0
#define NASREQ_APP_ID         1
#define MOBILEIP_APP_ID       2
#define BASE_ACCT_APP_ID      3
#define CREDIT_CONTROL_APP_ID 4
#define RELAY_APP_ID          0xffffffff
  
//AVP codes
  
#define ACCT_INTERIM_INTERVAL    85
#define ACCT_REALTIME_REQUIRED   483
#define ACCT_MULTI_SESSION_ID    50
#define ACCT_RECORD_NUMBER       485
#define ACCT_RECORD_TYPE         480
#define ACCT_SESSION_ID          44
#define ACCT_SUB_SESSION_ID      287
#define ACCT_APP_ID              259
#define AUTH_APP_ID              258
#define AUTH_REQUEST_TYPE        274
#define AUTH_LIFETIME            291
#define AUTH_GRACE_PERIOD        276
#define AUTH_SESSION_STATE       277
#define CLASS                    25
#define DESTINATION_HOST         293
#define DESTINATION_REALM        283
#define DISCONNECT_CAUSE         273
#define E2E_SEQUENCE             300
#define ERROR_MESSAGE            281
#define ERROR_REPORTING_HOST     294
#define EVENT_TIMESTAMP          55
#define EXPERIMENTAL_RESULT      297
#define EXPERIMENTAL_RESULT_CODE 298
#define FAILED_AVP               279
#define FIRMWARE_REVISION        267
#define HOST_IP_ADDRESS          257
#define INBAND_SECURITY_ID       299
#define MULTI_ROUND_TIMEOUT      272
#define ORIGIN_HOST              264
#define ORIGIN_REALM             296
#define ORIGIN_STATE_ID          278
#define PRODUCT_NAME             269
#define PROXY_HOST               280
#define PROXY_INFO               284
#define PROXY_STATE              33
#define REDIRECT_HOST            292
#define REDIRECT_HOST_USAGE      261
#define REDIRECT_MAX_CACHE_TIME  262
#define RE_AUTH_REQUEST_TYPE     285
#define RESULT_CODE              268
#define ROUTE_RECORD             282
#define SESSION_ID               263
#define SESSION_TIMEOUT          27
#define SESSION_BINDING          270
#define SESSION_SERVER_FAILOVER  271
#define SUPPORTED_VENDOR_ID      265
#define TERMINATION_CAUSE        295
#define USER_NAME                1
#define VENDOR_ID                266
#define VENDOR_SPECIFIC_APP_ID   260
  
//AVPsfrom 3GPP TS 29.229 (3GPP vendor id)
#define SUPPORTED_FEATURES  628
#define FEATURE_LIST_ID     629
#define FEATURE_LIST        630

#endif
