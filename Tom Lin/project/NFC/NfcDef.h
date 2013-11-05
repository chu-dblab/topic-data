/*
 *               Copyright (c), NXP Semiconductors
 *
 *                       (C)NXP B.V.2008
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *       NXP reserves the right to make changes without notice at any time.
 *      NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *      copyright or trademark. NXP must not be liable for any loss or damage
 *                     arising from its use.
 *
 */


/*!
 * \file main.h
 *
 *
 * Project: CAS Example Project for NFC MW / HAL
 *
 * $Date: Wed Oct 22 17:37:40 2008 $
 * $Author: frq04844 $
 * $Revision: 1.1 $
 * $Aliases:  $
 *
*/
/*@{*/
#ifndef MAIN_H
#define MAIN_H
/*@}*/


/**
 *  \name HAL
 *
 * File: \ref main.h
 *
 */
///*@{*/
//#define PHHALNFC_FILEREVISION "$Revision: 1.1 $" /**< \ingroup grp_file_attributes */
//#define PHHALNFC_FILEALIASES  "$Aliases:  $"     /**< \ingroup grp_file_attributes */
///*@}*/

#include <stdio.h>
#include <string.h>

#include <phHalNfc.h>
#include "phNfcStatus.h"
#include <phNfcTypes.h>
#include <phNfcIoctlCode.h>

// Allowed number of local NFC devices (needed @ENUMERATE)
#define MAX_BOARD_NUMBER                5
// Defines the send and receive buffer size
#define DATABUFFERSIZE      (uint16_t)  4000
// Defines the send and receive buffer for the reader/writer data exchange
#define RWBUFFERSIZE                    4000
// The maximum number of modes which are polled for
#define NB_OP_MODE_MAX                  6
// Define the number of general bytes transceived during ATR_REQ (max number is defined in ISO18092)
#define NUMBER_GENERAL_BYTES            12
// The maximum number of tags that are polled for
#define NB_TAG_MAX                      5


//--------------------------------------------------------------------------------------------------

//---------------------------------
// DATA STRUCTURES FOR THE EXAMPLES
//---------------------------------

// All data needed during Enumeration of local devices
typedef struct phcsDeviceParam
{
    // List of all local NFC hardware found
    phHal_sHwReference_t    HwRef[MAX_BOARD_NUMBER];
    // Length of local NFC hardware list found
    uint8_t                 NbrOfDevDetected;
	//Stor the data of Reader List
	LPTSTR          lpszReaderList;
    // Indicates to which of the local hardware devices HAL is connected to
    uint8_t                 index;
    // store maximum data transfer size without MetaChaining
    uint32_t                MTU;
    // MultiPoll supported
    uint8_t                 MultiPoll;
    // Error handling
    NFCSTATUS               status;


} phcsDeviceParam_t;

#if 0
// All data needed during NFC Initiator mode
typedef struct phcsExampleInitiatorParam
{
    // Error handling
    NFCSTATUS               status;       /*!< \brief [in/out]  */
    // Variable to store user interface input
    uint8_t                 menu_choice;
    // Data buffer to store transmit data
    uint8_t                 buffer[DATABUFFERSIZE];
    // Data buffer to store receive data
    uint8_t                 receivebuffer[DATABUFFERSIZE];

    //---------------------
    // INITIATOR ACTIVATION
    //---------------------

    // Hardware information of the NFC Initiator
    phHal_sHwReference_t    *HwRef;

    //------------------
    // TARGET ACTIVATION
    //------------------

    // Number of NFC remote devices found
    uint8_t                 nbRemoteDev;
    // Table of target modes accepted
    phHal_eOpModes_t        PollModes[NB_OP_MODE_MAX];
    // Target device information struct, only one target shall be accepted as P2P
    phHal_sRemoteDevInformation_t   remoteDevInfoList[1];
    // Table of target data used by the initiator to establish the P2P communication
    // NFCIDs, Use DiD, NAD, General Bytes...
    phHal_sDevInputParam_t  communicationparams[2];

    //--------------
    // DATA EXCHANGE
    //--------------

    // DEP_REQ command
    phHal_uCmdList_t        DEPcmd;
    // how many data (payload) bytes shall be transmitted to the target
    uint16_t                SendLength;
    // how many bytes were received as target response
    uint16_t                RcvLength;
    // more information for DEP_REQ e.g. the MI bit is set inhere
    phHal_sDepAdditionalInfo_t DepMoreInfo;
    // is meta chaining needed to fulfill SendLength settings ?
    uint8_t                 MetaChaining;
    // to keep in mind how many data bytes are still open
    // to be transmitted when using meta chaining
    uint16_t                OpenChainingData;
    // keep track on number of meta chaining operations
    uint16_t                MetaChainingCounter;
    // counter to keep track on the iteration status which is set b
    // by the user inside p2pDataFlowParams_t
    uint16_t                iterations;
    // keep track on maximum amount of data without using meta chaining
    uint16_t                MTU;
    //from which index of the buffer onwards data is send
    uint16_t                index;
    // is incoming data chained using MI?
    uint16_t                receiveindex;
    // receive loop finished?
    uint8_t                 receivecomplete;

} phcsExampleInitiatorParam_t;


// All data needed during NFC Initiator mode
typedef struct phcsExampleTargetParam
{
    // Hardware information of the local NFC Target
    phHal_sHwReference_t    *HwRef;
    // Data buffer to store receive and response (send) data
    uint8_t                 buffer[DATABUFFERSIZE];
    // Error handling
    NFCSTATUS               status;

    //------------------
    // START TARGET MODE
    //------------------

    // Data used by the target to establish the P2P communication
    // NFCIDs, General Bytes...
    phHal_sTargetInfo_t     communicationparams;
    // Table of modes which will be accepted during remote device poll
    phHal_eOpModes_t        PollModes[NB_OP_MODE_MAX];
    // Activation data from initiator
    uint8_t                 ConnectionReq[64];
    // Length of activation data from initiator
    uint8_t                 ConnectionReqBufLength;
    //// marker for DiD used
    //uint8_t                 DiDUsed; 

    //--------------
    // DATA EXCHANGE
    //--------------

    // DEP_RES command
    phHal_uCmdList_t        DEPcmd;
    // how many data (payload) bytes shall be sent to the initiator
    uint16_t                SendLength;
    // to keep in mind how many data bytes are still open
    // to be transmitted when using meta chaining
    uint16_t                OpenChainingData;
    // how many bytes were received as initiaor request
    uint16_t                RcvLength;
    // is meta chaining needed to fulfill SendLength settings ?
    uint8_t                 MetaChaining;
    // keep track on number of meta chaining operations
    uint16_t                MetaChainingCounter;
    // more information for DEP_REQ e.g. the MI bit is set inhere
    phHal_sDepAdditionalInfo_t  DepMoreInfo;
    // counter to keep track on the iteration status which is set b
    // by the user inside p2pDataFlowParams_t
    uint16_t                iterations;
    // keep track on maximum amount of data without using meta chaining
    uint32_t                MTU;
    // General Bytes
    uint8_t                 GeneralBytes[NUMBER_GENERAL_BYTES];
    //quit DEP exchange
    uint8_t                 QuitDEP;
    //from which index of the buffer onwards data is send
    uint16_t                index;


} phcsExampleTargetParam_t;


// All data needed during P2P data flow setup
typedef struct p2pDataFlowParams
{
    // Error handling
    NFCSTATUS                   status;

    // ---------------------
    // NFC INITIATOR RELATED
    // ---------------------

    // How many bytes shall be transmitted dring one DEP sequence to the target
    uint16_t                    NbrOfBytesTransceived;
    // how often shall the amount of NbrOfBytesTransceived be transmitted consecutevely
    uint16_t                    iterations;
    // if iteration > 0 the amount of AddBytesPerIteration is added to NbrOfBytesTransceived per iteration
    uint16_t                    AddBytesPerIteration;
    // if iteration > 0 the amount of SubtractBytesPerIteration is subtracted from NbrOfBytesTransceived per iteration
    uint16_t                    SubtractBytesPerIteration;
    // indicate DiD usage in P2P data exchange
    uint8_t                     DiDUsage;
    // indicate NAD usage in P2P data exchange
    uint8_t                     NADUsage;

    // ------------------
    // NFC TARGET RELATED
    // ------------------

    // How many bytes shall be send back to an initiator DEP request
    uint16_t                    NbrOfBytesResponse;
    // if iteration of initiator > 1 the amount of AddBytesPerRequest is added to NbrOfBytesResponse
    uint16_t                    AddBytesPerRequest;
    // if iteration of initiator > 1 the amount of SubtractBytesPerRequest is subtracted from NbrOfBytesResponse
    uint16_t                    SubtractBytesPerRequest;

    // Defines a target waiting time [ms] between data received and data response to initiator
    // Is used to test target waiting timeout extension (WTX) capabilites
    uint16_t                    LatencyOfResponseToRequest;

} p2pDataFlowParams_t;
#endif

// All data needed during Reader / Writer Example
typedef struct phcsCardParam
{
	//pointer of the init parameters
	//phcsDeviceInitParam_t *pInitParams;

    // Hardware information of the NFC Target
    phHal_sHwReference_t    *HwRef;
    // Data buffer to store receive and response (send) data
//    uint8_t                 buffer[RWBUFFERSIZE];
    // Error handling
    NFCSTATUS               status;

    // SAM config marker
    //uint8_t                 S2CMarker;

    // -----------------------
    // TARGET (TAG) ACTIVATION
    // -----------------------

    phHal_sRemoteDevInformation_t   remoteDevInfo;

    // --------------------------
    // TARGET (TAG) COMMUNICATION
    // --------------------------

    // keep track on maximum amount of data without using meta chaining
    //uint16_t                MTU;
    // MultiPoll Session Support
   // uint8_t                 MultiPollSession;
    // store command for data transfer inhere
    phHal_uCmdList_t        Cmd;
    // communication command
    //phHal_uCmdList_t        DEPcmd;

    // more information needed for data exchange e.g. NAD, Chaining
    phHal_sDepAdditionalInfo_t DepMoreInfo;
    // decide on data written to remote (tag) devices
    //uint8_t                 write_zeros;

} phcsCardParam_t;

//------------------------------------------------------------------------------------------------------



#endif
