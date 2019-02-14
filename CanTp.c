#include "CanTp.h"

#include "CanIf.h"
#include "SchM_CanTp.h"
#include "PduR_CanTp.h"

#define NULL    (0)

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif

typedef enum {
    CANTP_OFF,
    CANTP_ON
} CanTp_ModuleInternalStateType; //SWS_CanTp_00027

typedef enum {
    CANTP_TX_WAIT,
    CANTP_TX_PROCESSING
} CanTp_TransmitChannelStateType;

typedef enum {
    CANTP_RX_WAIT,
    CANTP_RX_PROCESSING
} CanTp_ReceiveChannelStateType;

typedef struct { 
    CanTp_ModuleInternalStateType moduleInternalState;
    CanTp_TransmitChannelStateType transmitChannelState;
    CanTp_ReceiveChannelStateType receiveChannelState;
    PduIdType TX_id;
    PduIdType RX_id;
    PduInfoType * TX_PduInfoPtr;
    PduInfoType * RX_PduInfoPtr;
} CanTp_RunTimeDataType; // SWS_CanTp_00002

STATIC CanTp_RunTimeDataType runTimeData;

void CanTp_Init( const CanTp_ConfigType* CfgPtr )
{
    //CtgPtr Pointer to the CanTp post-build configuration data.
    
    //This function initializes the CanTp module.
    runTimeData.moduleInternalState = CANTP_ON;
    runTimeData.transmitChannelState = CANTP_TX_WAIT;
    runTimeData.receiveChannelState = CANTP_RX_WAIT;

    runTimeData.TX_id = 0;
    runTimeData.RX_id = 0;
    runTimeData.TX_PduInfoPtr = NULL;
    runTimeData.RX_PduInfoPtr = NULL;
}

void CanTp_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
    //versioninfo Indicator as to where to store the version information of this module.
    // Należy do wskaźńika przypisać odpowiednią zmienną
    //This function returns the version information of the CanTp module.
}

void CanTp_Shutdown( void )
{
    //This function is called to shutdown the CanTp module.
    runTimeData.moduleInternalState = CANTP_OFF;
    runTimeData.transmitChannelState = CANTP_TX_WAIT;
    runTimeData.receiveChannelState = CANTP_RX_WAIT;
}

Std_ReturnType CanTp_Transmit( PduIdType TxPduId, const PduInfoType* PduInfoPtr )
{
    //TxPduId Identifier of the PDU to be transmitted
    //PduInfoPtr Length of and pointer to the PDU data and pointer to MetaData.

    //E_OK: Transmit request has been accepted. E_NOT_OK: Transmit request has not been accepted.

    //Requests cancellation of an ongoing transmission of a PDU in a lower layer communication module.
    return E_OK;
    
    
  // slå upp channel via txsduid
/*  sint8 channel = txNSduData[CanTpTxSduId].channel;
  if(channel != -1) {
    // channel busy, return error
    return error;
  }
  // allocate free channel
  channel = 31 - CountLeadingZeros(channelFreeMask);
  if(channel < 0) {
    // no free channel, return
    return error
  }
  // clear free flag
  channelFreeMask ^= 1 << channel;
  txNSduData[CanTpTxSduId].channel = channel;
  // start transmission
  StartToTransmit(CanTpTxSduId, CanTpTxInfoPtr, channel);
}
*/
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Tych funkcji mamy nie implementowac, tylko zostawic je puste:
Std_ReturnType CanTp_CancelTransmit( PduIdType TxPduId )
{
    return E_OK;
}

Std_ReturnType CanTp_CancelReceive( PduIdType RxPduId )
{
    return E_OK;
}

Std_ReturnType CanTp_ChangeParameter( PduIdType id, TPParameterType parameter, int value )
{
    return E_OK;
}

Std_ReturnType CanTp_ReadParameter( PduIdType id, TPParameterType parameter, int* value )
{
    return E_OK;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
static Std_ReturnType CanTp_ReadParameter( PduIdType id, TPParameterType parameter, int* value )
{
    //id Identifier of the received N-SDU on which the reception parameter are read.
    //parameter Specify the parameter to which the value has to be read (BS or STmin).

    //E_OK: request is accepted. E_NOT_OK: request is not accepted.

    //This service is used to read the current value of reception parameters BS and STmin for a specified N-SDU.
    return E_OK;
}
*/
void CanTp_RxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr )
{
    //RxPduId ID of the received PDU.
    //PduInfoPtr Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr) containing the PDU, and the MetaData related to this PDU.

    //Indication of a received PDU from a lower layer communication interface module.

    if ( CANTP_RX_WAIT == runTimeData.receiveChannelState )
    {
        runTimeData.RX_id = RxPduId;
        runTimeData.RX_PduInfoPtr = (PduInfoType*) PduInfoPtr;
        runTimeData.receiveChannelState = CANTP_RX_PROCESSING;
    }
}

void CanTp_TxConfirmation( PduIdType TxPduId, Std_ReturnType result )
{
    //TxPduId ID of the PDU that has been transmitted.

    //result E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed.

    //The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
}

void CanTp_MainFunction( void )
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
    PduLengthType RX_Length = 0;

    // The main function for scheduling the CAN TP
    if ( CANTP_ON == runTimeData.moduleInternalState )
    {
        if ( CANTP_RX_PROCESSING == runTimeData.receiveChannelState )
        {

            ret = PduR_CanTpStartOfReception( runTimeData.RX_id, runTimeData.RX_PduInfoPtr, runTimeData.RX_PduInfoPtr->SduLenght, &RX_Length);
            if ( BUFREQ_E_NOT_OK == ret )
            {
                runTimeData.receiveChannelState = CANTP_RX_WAIT;
            }
        }
    }
}
