#ifndef DIAMETER_RESULT_CODES_H
#define DIAMETER_RESULT_CODES_H

/**
   Information from 
   http://publib.boulder.ibm.com/infocenter/wtelecom/v6r1/index.jsp?topic=%2Fcom.ibm.diameter.dev.doc%2Fdiameter_result_codes_r.html
*/
#define DIAMETER_MULTI_ROUND_AUTH		1001 /**<  This informational 
							error is returned by a Diameter server to 
							inform the access device that the 
							authentication mechanism being used requires
							multiple round trips, and a subsequent request
							needs to be issued in order for access to be granted. */
#define DIAMETER_SUCCESS		        2001 /**<  The Request was successfully completed. */
#define DIAMETER_LIMITED_SUCCESS		2002 /**<  When returned, the request was successfully completed, 
							but additional processing is required by the application 
							in order to provide service to the user. */
#define DIAMETER_COMMAND_UNSUPPORTED		3001 /**<  The Request contained a Command-Code that the 
							receiver did not recognize or support. */
#define DIAMETER_UNABLE_TO_DELIVER		3002 /**<  This error is given when Diameter can not deliver the
							message to the destination. */
#define DIAMETER_REALM_NOT_SERVED		3003 /**<  The intended realm of the request is not recognized. */
#define DIAMETER_TOO_BUSY		        3004 /**<  When returned, a Diameter node should attempt to send the
							message to an alternate peer. */
#define DIAMETER_LOOP_DETECTED		        3005 /**<  An agent detected a loop while trying to get the message to
							the intended recipient. */
#define DIAMETER_REDIRECT_INDICATION		3006 /**<  A redirect agent has determined that the request could 
							not be satisfied locally. */
#define DIAMETER_APPLICATION_UNSUPPORTED	3007 /**<  A request was sent for an application that is
								not supported. */
#define DIAMETER_INVALID_HDR_BITS		3008 /**<  A request was received whose bits in the Diameter header
							were either set to an invalid combination. */
#define DIAMETER_INVALID_AVP_BITS		3009 /**<  A request was received that included an AVP whose flag
							bits are set to an unrecognized value. */
#define DIAMETER_UNKNOWN_PEER		        3010 /**<  A CER was received from an unknown peer. */
#define DIAMETER_AUTHENTICATION_REJECTED	4001 /**<  The authentication process for the user failed, 
							most likely due to an invalid password used by the user. */
#define DIAMETER_OUT_OF_SPACE		        4002 /**<  A Diameter node received the accounting request but was unable to
							commit it to stable storage due to a temporary lack of space. */
#define ELECTION_LOST		                4003 /**<  The peer has determined that it has lost the election process and has 
							therefore disconnected the transport connection. */
#define DIAMETER_AVP_UNSUPPORTED		5001 /**<  The peer received a message that contained an
							AVP that is not recognized or supported and was marked with 
							the Mandatory bit. */
#define DIAMETER_UNKNOWN_SESSION_ID		5002 /**<  The request contained an unknown Session-Id. */
#define DIAMETER_AUTHORIZATION_REJECTED		5003 /**<  A request was received for which the user could not be authorized. */
#define DIAMETER_INVALID_AVP_VALUE		5004 /**<  The request contained an AVP with an invalid value
							in its data portion. */
#define DIAMETER_MISSING_AVP		        5005 /**<  The request did not contain an AVP that is required by the 
							Command Code definition. */
#define DIAMETER_RESOURCES_EXCEEDED		5006 /**<  A request was received that cannot be authorized because the
							user has already expended allowed resources. */
#define DIAMETER_CONTRADICTING_AVPS		5007 /**<  One or more Failed-AVP AVPs MUST be present, containing the
							AVPs that contradicted each other. */
#define DIAMETER_AVP_NOT_ALLOWED		5008 /**<  A message was received with an AVP that should not be present. 
							The Failed-AVP AVP must be included and contain a copy of the
							offending AVP */
#define DIAMETER_AVP_OCCURS_TOO_MANY_TIMES	5009 /**<  A message was received that included an AVP that 
							appeared more often than permitted in the message definition. 
							The Failed-AVP AVP must be included and contain a copy of
							the first instance of the offending AVP that exceeded the 
							maximum number of occurrences. */
#define DIAMETER_NO_COMMON_APPLICATION		5010 /**<  This error is returned when a CER message is received, and 
							there are no common applications supported between the peers. */
#define DIAMETER_UNSUPPORTED_VERSION		5011 /**<  This error is returned when a request was received, whose 
							version number is unsupported. */
#define DIAMETER_UNABLE_TO_COMPLY		5012 /**<  This error is returned when a request is rejected for 
							unspecified reasons. */
#define DIAMETER_INVALID_BIT_IN_HEADER		5013 /**<  This error is returned when an unrecognized bit in the Diameter
							header is set to one (1). */
#define DIAMETER_INVALID_AVP_LENGTH		5014 /**<  The request contained an AVP with an invalid length. A Diameter
							message indicating this error MUST include the offending AVPs 
							within a Failed-AVP AVP. */
#define DIAMETER_INVALID_MESSAGE_LENGTH		5015 /**<  This error is returned when a request is received with an 
							invalid message length. */
#define DIAMETER_INVALID_AVP_BIT_COMBO		5016 /**<  The request contained an AVP with which is not allowed to
							have the given value in the AVP Flags field. A Diameter message
							indicating this error MUST include the offending AVPs within 
							a Failed-AVPAVP. */
#define DIAMETER_NO_COMMON_SECURITY		5017 /**<  This error is returned when a CER message is received, 
							and there are no common security mechanisms supported between 
							the peers. */

#endif
