//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Ethernet-TP: target driver interface

   Interface to target specific Ethernet driver wrapper functions.

   The target layer needs to provide one TCP port and one UDP port for each bus.
   For UDP only broadcast messages need to be accepted.

   The transport protocol uses
   - the TCP port for "point-to-point" communication (i.e. to respond to requests from a client tool).
   - the UDP port for broadcast communication
      (broadcasts received from a client tool and broadcasts sent into the local network)

   Typical sequence of functions calls from transport protocol:
   ============================================================
   - osy_etp_trg_init_communication for communication initialization (once per interface)
   - Then for each communication cycle:
   -- osy_etp_trg_tcp_retrieve_message to check for incoming new connections and data
   -- osy_etp_trg_tcp_send_message to send response data on an already established TCP connection
   -- osy_etp_trg_udp_retrieve_message to read data from the UDP port
   -- osy_etp_trg_udp_send_message to send data via UDP
   -- osy_etp_trg_tcp_flush at the end of the communication cycle

   Handling of TCP communication
   =============================
   For full technical details see #53094.

   Most openSYDE communication is sequential. The client sends a request, the servers sends a response.
   However there are cases where multiple responses are sent without an explicit request.
    This includes cyclic transmission of Datapool elements and tunneling of L2 CAN messages received by the server.
   With that in mind we want to balance two goals:
   - individual responses to "polled" requests should be sent without delays to get short handshaking times
   - multiple non-polled responses should be placed into one TCP frame to reduce amount of traffic

   This is achieved by the following means which need to be supported by the target layer implementation:
   By default the Nagle algorithm can result in responses getting delayed. It can be deactivated with the "TCP_NODELAY"
    option.
   To prevent individual responses to be sent in separate TCP packages we also need to use the "TCP_CORK" option.
   This allows us to control exactly when the TCP data shall be sent out.

   Specific steps to be taken in the target layer implementation:
   - set socket option TCP_NODELAY=1 after accepting a new connection in osy_etp_trg_tcp_retrieve_message
   -- deactivates Nagle algorithm to not wait till TCP packages "fill up"
   - set socket option TCP_CORK=1 whenever sending data with osy_etp_trg_tcp_send_message and it is not yet set
   -- tells IP-stack to "collect" data and not send out immediately
   - set socket option TCP_CORK=0 in osy_etp_trg_tcp_flush
   -- tells IP-stack to flush out data sent before corking

   If the "TCP_CORK" option is not available on the target then it is suggested to only use TCP_NODELAY.
   As a result responses to requests will be sent immediately. But there will be additional TCP traffic due to
    each response being placed in its own TCP package.

   Concept and typical sequence to setup a Ethernet to Ethernet routing:
   =====================================================================
   - Client connects to a server
   - Client sends Ip2Ip routing activation request
      -> Server's UDS stack calls osy_etp_trg_setup_routing() which opens a TCP connection to the routing target.
   - Client polls the routing status until routing is ready or an error occurred
      -> UDS stack calls osy_etp_trg_get_routing_status()
     A timeout of 2000ms shall be applied. After this timeout the connection try shall be considered to have failed.
     If an error occurs, it is reported to the client; the TCP connection can be used for further communication.
     On success (connection to target established), the request's response is sent to the client. The UDS stack
     calls osy_etp_trg_tcp_send_message() right after returning from osy_etp_trg_setup_routing(), without calling
     osy_etp_trg_tcp_retrieve_message() in between.
   - The target layer shall force the UDS stack to drop the connection instance after the routing has been
     established. This can be done by returning C_TIMEOUT in the next call of osy_etp_trg_tcp_retrieve_message().
     The stack calls osy_etp_trg_tcp_close_connection() in this case. Make sure not to close the routing connection
     here.
   - All subsequent data on the client connection shall be forwarded to the routing target now. Data from the routing
     target connection shall be forwarded to the client. The target layer is responsible for this task.
   - After a routing has been established, the server shall be ready to accept a new client connection. The routing
     connections shall be handled in background.
   - A routing is canceled by closing the TCP connection on the client or routing target side.
   - Multiple routing connections per bus shall be supported. The maximum number is target specific (usually limited by
     memory or IP stack resources).

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef OSY_ETP_TRG_IFH
#define OSY_ETP_TRG_IFH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "osy_etp_trg.h" //we need the target specific initialization definition

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

///size of DoIp message header
#define OSY_ETP_MESSAGE_HEADER_SIZE  (8U)
///maximum size for each buffer: UDS message size; <=4095 bytes plus header + 4 bytes for source and target address
#define OSY_ETP_TCP_BUFFER_SIZE (4095U + (OSY_ETP_MESSAGE_HEADER_SIZE) +4U)
///maximum size for each buffer: Maximum UDP service size; <=47 bytes plus header
#define OSY_ETP_UDP_BUFFER_SIZE (47U + (OSY_ETP_MESSAGE_HEADER_SIZE))

///port to use for UDP:
#define OSY_ETP_MP_UDP_PORT   (13400U)
///port to use for TCP:
#define OSY_ETP_MP_TCP_PORT   (13400U)

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check whether Ethernet link is up

   This function shall return C_NO_ERR when it first detects that a link was established on the specified bus.
   Otherwise it shall return C_NOACT (i.e.: either if no link is established or it was already reported).

   If the link gets dropped and re-established the function shall again return C_NO_ERR for one call.

   If an internal switch is used a new link may be reported for each new link detected internally.

   This function may be used by upper layers to find out about new events. It shall not perform any lengthy
    operations. Depending on the target remembering and reporting a "link up" event

   \param[in]   ou8_BusNumber          ETH bus number (0 = first supported bus, ...)

   \retval C_NO_ERR   new link detected
   \retval C_NOACT    no new link detected
   \retval C_RANGE    one of the parameters is out of range
*/
//----------------------------------------------------------------------------------------------------------------------
/*PRQA S 1501 1*/ /*STW_MD4_M_Rule-2.1_1501 Not used in UDS stack directly; to be used by higher layer */
extern sint16 osy_etp_trg_has_new_link(const uint8 ou8_BusNumber);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize communication channel

   Perform target specific communication initialization action.

   Sockets for UDP and TCP communication shall be configured.
   The required port is 13400 for both.
   The sockets shall be bound to the IP of the specified bus.
   The UDP socket shall be configured to received broadcasts.

   The communication interface itself is typically not initialized by this function
   as multiple protocols use the same physical interface.

   The target layer implementation must store the handles for both, UDP and TCP
   connections, to use them for communication.

   Notes on communication status:
   The openSYDE Server does not provide a generic mechanism to inform the application about low-level communication
   problems. If needed this information should be provided to the application by the target integration.
   See the openSYDE integration manual requirements for more information.

   Handling of opt_InitParameters:
   If the target integration requires information from this structure the function shall check whether the pointer
    is != NULL and return C_RANGE if it is.

   \param[in]   ou8_BusNumber          ETH bus number (0 = first supported bus, ...)
   \param[in]   opt_InitParameters     target specific initialization parameters (NULL if none passed by application)

   \retval C_NO_ERR   initialization successful
   \retval C_RANGE    one of the parameters is out of range (or NULL)
   \retval C_NOACT    initialization failed
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_init_communication(const uint8 ou8_BusNumber,
                                             const T_osy_etp_trg_eth_init_params * const opt_InitParameters);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Read incoming TCP data

   Return incoming TCP data, provide the corresponding handle to calling function.

   Steps:
   - if not yet connected check whether a client wants to connect and accept that connection
   -- activate the "TCP_NODELAY" socket option on that new connection
      (See this file's comment header for the complete description of the TCP_NODELAY/TCP_CORK strategy)
   - if connected check for incoming data on that connection

   Only one concurrent TCP connection per channel is supported at a time.
   The function shall not return any data unless it can provide as many data bytes as specified.
   It shall not block if not enough data is available.

   The "handle" parameter can be used to report the handle for the opened connection to the transport protocol.
   The transport protocol will report this value back to subsequent calls of "osy_etp_trg_tcp_send_message" and
   "osy_etp_trg_tcp_close_connection". Using this parameter is optional if all the connection data is held statically
   in this target layer module.

   \param[in]     ou8_BusNumber       ETH bus number (0 = first supported bus, ...)
   \param[in]     ou16_Size           the expected number of bytes to read
   \param[out]    opu8_Message        read message
   \param[out]    oppv_Handle         handle of current communication path

   \retval C_NO_ERR   message retrieved (ou16_Size bytes were placed in opu8_Message)
   \retval C_RANGE    one of the parameters is out of range (or NULL)
   \retval C_CONFIG   communication not initialized
   \retval C_NOACT    no new message or error reading message
   \retval C_TIMEOUT  connection is no more valid (broken) and shall be closed by UDS stack
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_tcp_retrieve_message(const uint8 ou8_BusNumber, const uint16 ou16_Size,
                                               uint8 * const opu8_Message, void ** const oppv_Handle);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Read incoming UDP messages

   Return incoming UDP messages.
   Only broadcast messages shall be accepted.
   The "SockAddr" information returned by this function will be passed back by the stack in
   in a subsequent call of osy_etp_trg_udp_send_message()

   \param[in]     ou8_BusNumber       ETH bus number (0 = first supported bus, ...)
   \param[in,out] opu16_Size          size of available buffer/size of retrieved message
   \param[out]    opu8_Message        read message (caller guarantees a valid buffer matching the size parameter)
   \param[out]    opt_SockAddr        socket address related to sender of request (caller guarantees a valid pointer)

   \retval C_NO_ERR   message retrieved
   \retval C_RANGE    one of the parameters is out of range
   \retval C_CONFIG   communication not initialized
   \retval C_NOACT    no new message or error reading message
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_udp_retrieve_message(const uint8 ou8_BusNumber, uint16 * const opu16_Size,
                                               uint8 * const opu8_Message,
                                               T_osy_etp_trg_eth_sockaddr * const opt_SockAddr);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Send a TCP message

   Send a TCP message on specified channel by the use of the provided handle.

   If not yet set, then set the "TCP_CORK" socket option to make sure the data will not be sent out immediately.
   (See this file's comment header for the complete description of the TCP_NODELAY/TCP_CORK strategy)

   \param[in]   ou8_BusNumber       ETH bus number (0 = first supported bus, ...)
   \param[in]   ou16_Size           size of message
   \param[in]   opu8_Message        message to send (caller guarantees a valid buffer matching the size parameter)
   \param[in]   opv_Handle          handle of current communication path (provided by osy_etp_trg_tcp_retrieve_message)

   \retval C_NO_ERR   message successfully sent
   \retval C_RANGE    one of the parameters is out of range (or NULL)
   \retval C_CONFIG   communication not initialized
   \retval C_NOACT    could not send message
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_tcp_send_message(const uint8 ou8_BusNumber, const uint16 ou16_Size,
                                           const uint8 * const opu8_Message, const void * const opv_Handle);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Send a UDP message

   Send a UDP message on a specified channel.

   The message shall be sent
   * to target port 13400
   * as UDP broadcast
   * on the correct Ethernet interface as defined by ou8_BusNumber

   If opt_SockAddr != NULL, it provides the source address of the original request. It may be used to determine the
   interface to send the message on or may be ignored if the correct interface can be determined in another way.

   \param[in]   ou8_BusNumber       ETH bus number (0 = first supported bus, ...)
   \param[in]   ou16_Size           size of message
   \param[in]   opu8_Message        message to send (caller guarantees a valid buffer matching the size parameter)
   \param[in]   opt_SockAddr        socket address related to sender of original request

   \retval C_NO_ERR   message successfully sent
   \retval C_RANGE    one of the parameters is out of range
   \retval C_CONFIG   communication not initialized
   \retval C_NOACT    could not send message
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_udp_send_message(const uint8 ou8_BusNumber, const uint16 ou16_Size,
                                           const uint8 * const opu8_Message,
                                           const T_osy_etp_trg_eth_sockaddr * const opt_SockAddr);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Flush TX TCP data

   During one UDS stack communication cycle multiple sets of TCP data might have been sent with
    osy_etp_trg_tcp_send_message.
   This function shall make sure to "flush" out the data from those calls by deactivating the "TCP_CORK" option.
   (See this file's comment header for the complete description of the TCP_NODELAY/TCP_CORK strategy).

   \param[in]   ou8_BusNumber       ETH bus number (0 = first supported bus, ...)

   \retval C_NO_ERR   flushing done
   \retval C_RANGE    one of the parameters is out of range
   \retval C_NOACT    flushing did not work
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_tcp_flush(const uint8 ou8_BusNumber);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Closing TCP communication channel

   Perform target specific communication closing action.

   The active TCP connection opened by function osy_etp_trg_tcp_retrieve_message()
   shall be closed.

   \param[in]   ou8_BusNumber  ETH bus number (0 = first supported bus, ...)
   \param[in]   opv_Handle     handle for connection to close

   \retval C_NO_ERR   closing successful
   \retval C_RANGE    one of the parameters is out of range (or NULL)
   \retval C_NOACT    closing failed
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_tcp_close_connection(const uint8 ou8_BusNumber, void * const opv_Handle);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set IP address

   Perform target specific configuration of IPv4 address.
   The configured value shall be stored on the target permanently (e.g. in NVM).
   The new IP address shall be applied after the next system start.

   The caller guarantees valid pointer parameters.

   \param[in]   ou8_BusNumber           ETH bus number (0 = first supported bus, ...)
   \param[in]   opu8_IpAddress          array of 4 bytes containing the new IPv4 address (MSB first)
   \param[in]   opu8_NetMask            array of 4 bytes containing the new IPv4 net mask (MSB first)
   \param[in]   opu8_DefaultGateway     array of 4 bytes containing the new IPv4 default gateway (MSB first)

   \retval C_NO_ERR   storing new IP address successful
   \retval C_RANGE    one of the parameters is out of range
   \retval C_NOACT    storing new IP address failed
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_set_ip_address(const uint8 ou8_BusNumber, const uint8 opu8_IpAddress[4],
                                         const uint8 opu8_NetMask[4], const uint8 opu8_DefaultGateway[4]);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set routing between two channels

   Start the process to establish a TCP connection to the routing target. Do not wait until the connection has
   been established (e.g. create a socket, make in non-blocking, call connect() and return).

   The caller guarantees valid pointer parameters.

   \param[in]   ou8_SourceBusNumber      Source ETH bus number (0 = first supported bus, ...)
   \param[in]   ou8_TargetBusNumber      Target ETH bus number (0 = first supported bus, ...)
   \param[in]   opu8_TargetIpAddress     Target IP address (MSB first)
   \param[in]   opv_Handle               Handle of current communication path

   \retval C_NO_ERR   setup routing successful
   \retval C_RANGE    one of the parameters is out of range
   \retval C_NOACT    setup routing failed
   \retval C_BUSY     routing setup already in progress
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_setup_routing(const uint8 ou8_SourceBusNumber, const uint8 ou8_TargetBusNumber,
                                        const uint8 opu8_TargetIpAddress[4], void * const opv_Handle);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check the status of the routing setup procedure.

   This function shall be called cyclically be the client after setting up a Ethernet to Ethernet routing to
   check the progress.

   It shall return "in progress" until the routing has been set up or an error occurred.

   If the routing is set up (TCP connection to routing target established), the function shall return "connected".
   From this time on, all data from the client is forwarded to the target until the client or the target close
   their connection.
   When reporting the status as "error" the function shall reset it to "idle" internally.

   The caller guarantees a valid pointer parameter.

   \param[in]   ou8_SourceBusNumber     Bus number of interface connected to the client
   \param[in]   opv_Handle              Handle of current communication path

   \retval eRoutingIdle(0)          No routing setup in progress
   \retval eRoutingInProgress(1)    Routing setup in progress
   \retval eRoutingError(2)         An error occurred, routing setup aborted
   \retval eRoutingConnected(3)     Connection to target established, all further requests
                                    from client are forwarded to target now
*/
//----------------------------------------------------------------------------------------------------------------------
extern uint8 osy_etp_trg_get_routing_status(const uint8 ou8_SourceBusNumber, void * const opv_Handle);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Clear entire routing

   Clear all routings set up with osy_etp_trg_setup_routing.
   Messages for the net of the target channel shall not be forwarded anymore.

   \retval C_NO_ERR   clear routing successful
   \retval C_WARN     no routing active
   \retval C_NOACT    clear routing failed
*/
//----------------------------------------------------------------------------------------------------------------------
extern sint16 osy_etp_trg_clear_routing(void);

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check whether the target supports Eth2Eth routing

   The prerequisite for supporting Eth2Eth routing are
   * at least 2 available Ethernet channels
   * target layer implementation of Ethernet routing functions as specified

   \retval 0   Ethernet to Ethernet routing not supported
   \retval 1   Ethernet to Ethernet routing supported
*/
//----------------------------------------------------------------------------------------------------------------------
extern uint8 osy_etp_trg_get_eth2eth_routing_supported(void);

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif
