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

typedef enum { // Protocol Control Information
	PCI_SINGLE_FRAME = 0,
	PCI_FIRST_FRAME = 1,
	PCI_CONSECUTIVE_FRAME = 2,
	PCI_FLOW_CONTROL_FRAME = 3
} CanTp_PCI_FrameType;

typedef enum { // Flow Control Transfer Info Frame Type
	CONTINUETOSEND = 0,
	WAIT = 1,
	OVERFLOW_ABORT = 2,
} CanTp_PCI_FlowControlInfoType;

typedef struct { 
	CanTp_ModuleInternalStateType moduleInternalState;
	CanTp_TransmitChannelStateType transmitChannelState;
	CanTp_ReceiveChannelStateType receiveChannelState;
	PduIdType TX_id;
	PduIdType RX_id;
	PduInfoType * TX_PduInfoPtr;
	PduInfoType * RX_PduInfoPtr;
	unsigned char TX_length;
	unsigned char RX_length;
	unsigned char RX_available_buffer;
	unsigned char RX_consecutive_frames;
} CanTp_RunTimeDataType; // SWS_CanTp_00002

STATIC CanTp_RunTimeDataType runTimeData;

/* non exported function declarations */
STATIC CanTp_PCI_FrameType getFrameType( unsigned char firstByte );
STATIC void sendFlowControlFrame( CanTp_PCI_FlowControlInfoType frameInfo, unsigned char blockSize );

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
	if ( CANTP_OFF == runTimeData.moduleInternalState)
		return;

	unsigned char first_byte = *( PduInfoPtr->SduDataPtr );
	CanTp_PCI_FrameType frameType = getFrameType( first_byte );
	BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

	switch (frameType)
	{
		case PCI_SINGLE_FRAME:
			runTimeData.RX_id = RxPduId;
			runTimeData.RX_PduInfoPtr = ( PduInfoType *) PduInfoPtr;
			runTimeData.RX_length = first_byte;
			ret = PduR_CanTpStartOfReception( runTimeData.RX_id, runTimeData.RX_PduInfoPtr, runTimeData.RX_PduInfoPtr->SduLenght, &runTimeData.RX_available_buffer);
			if ( BUFREQ_OK == ret )
			{
				runTimeData.RX_PduInfoPtr->SduDataPtr = runTimeData.RX_PduInfoPtr->SduDataPtr + 1;
				runTimeData.RX_PduInfoPtr->SduLenght = runTimeData.RX_length;
				ret = PduR_CanTpCopyRxData(runTimeData.RX_id, runTimeData.RX_PduInfoPtr, &runTimeData.RX_available_buffer);
				if ( BUFREQ_OK == ret )
				{
					PduR_CanTpRxIndication( runTimeData.RX_id, E_OK );
				}
			}
			break;

		case PCI_FIRST_FRAME:
			runTimeData.RX_id = RxPduId;
			runTimeData.RX_PduInfoPtr = (PduInfoType *) PduInfoPtr;
			runTimeData.RX_length = ( ( ( first_byte & 0x0F ) << 8 ) | *( PduInfoPtr->SduDataPtr + 1) );
			runTimeData.RX_PduInfoPtr->SduDataPtr = runTimeData.RX_PduInfoPtr->SduDataPtr + 2;
			ret = PduR_CanTpStartOfReception( runTimeData.RX_id, runTimeData.RX_PduInfoPtr, runTimeData.RX_length, &runTimeData.RX_available_buffer);
			if ( BUFREQ_OK == ret )
			{
				runTimeData.RX_PduInfoPtr->SduDataPtr = runTimeData.RX_PduInfoPtr->SduDataPtr + 2;
				runTimeData.RX_PduInfoPtr->SduLenght = runTimeData.RX_PduInfoPtr->SduLenght - 2;
				ret = ret = PduR_CanTpCopyRxData(runTimeData.RX_id, runTimeData.RX_PduInfoPtr, &runTimeData.RX_available_buffer);
				if ( BUFREQ_OK == ret )
				{
					runTimeData.RX_length = runTimeData.RX_length - runTimeData.RX_PduInfoPtr->SduLenght;
					runTimeData.receiveChannelState = CANTP_RX_PROCESSING;
					
					if ( 7 <= runTimeData.RX_available_buffer )
					{
						runTimeData.RX_consecutive_frames = runTimeData.RX_length / 7;
						if ( 0 != (runTimeData.RX_length % 7) )
						{
							runTimeData.RX_consecutive_frames = runTimeData.RX_consecutive_frames + 1;
						}
						sendFlowControlFrame( CONTINUETOSEND, runTimeData.RX_consecutive_frames );
					}
					else
					{
						
					}
				}
			}
			break;

		case PCI_CONSECUTIVE_FRAME:
			runTimeData.RX_id = RxPduId;
			runTimeData.RX_PduInfoPtr = ( PduInfoType * ) PduInfoPtr;
			runTimeData.RX_PduInfoPtr->SduDataPtr = runTimeData.RX_PduInfoPtr->SduDataPtr + 1;
			runTimeData.RX_PduInfoPtr->SduLenght = runTimeData.RX_PduInfoPtr->SduLenght - 1;

			ret = PduR_CanTpCopyRxData(runTimeData.RX_id, runTimeData.RX_PduInfoPtr, &runTimeData.RX_available_buffer);
			if ( BUFREQ_OK == ret )
			{
				runTimeData.RX_length = runTimeData.RX_length - runTimeData.RX_PduInfoPtr->SduLenght;
				runTimeData.receiveChannelState = CANTP_RX_PROCESSING;
				runTimeData.RX_consecutive_frames = runTimeData.RX_consecutive_frames - 1;
				if ( runTimeData.RX_consecutive_frames == 0 )
					runTimeData.receiveChannelState = CANTP_RX_WAIT;
			}
			break;

		default:
			break;
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

		}
	}
}

CanTp_PCI_FrameType getFrameType( unsigned char first_byte )
{
	// get older nibble
	unsigned char nibble = first_byte >> 4;

	return ( CanTp_PCI_FrameType ) nibble;
}

void sendFlowControlFrame( CanTp_PCI_FlowControlInfoType frameInfo, unsigned char blockSize )
{
	unsigned char buffer[8] = { 0 };

	buffer[0] = (PCI_FLOW_CONTROL_FRAME << 4) | frameInfo;
	buffer[1] = blockSize;

	PduInfoType pduInfoType;
	pduInfoType.MetaDataPtr = NULL;
	pduInfoType.SduDataPtr = buffer;
	pduInfoType.SduLenght = sizeof(buffer);
	CanIf_Transmit( runTimeData.RX_id, &pduInfoType );
}
