#include "CanTp.h"

unsigned char state = CANTP_OFF;

void CanTp_Init( const CanTp_ConfigType* CfgPtr )
{
    //CtgPtr Pointer to the CanTp post-build configuration data.
    
    //This function initializes the CanTp module.
    state = CANTP_ON;
}

void CanTp_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
    //versioninfo Indicator as to where to store the version information of this module.
    
    //This function returns the version information of the CanTp module.
}

void CanTp_Shutdown( void )
{
    //This function returns the version information of the CanTp module.
    state = CANTP_OFF;
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
}

void CanTp_TxConfirmation( PduIdType TxPduId, Std_ReturnType result )
{
    //TxPduId ID of the PDU that has been transmitted.

    //result E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed.

    //The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
}

void CanTp_MainFunction( void )
{
    // The main function for scheduling the CAN TP
}