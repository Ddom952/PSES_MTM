#include "ComStackTypes.h"

typedef struct {
    //Implementation specific cokolwiek to znaczy
    //Data structure type for the post-build configuration parameters.
} CanTp_ConfigType;

void CanTp_Init( const CanTp_ConfigType* CfgPtr );
void CanTp_GetVersionInfo( Std_VersionInfoType* versioninfo );
void CanTp_Shutdown( void );
Std_ReturnType CanTp_Transmit( PduIdType TxPduId, const PduInfoType* PduInfoPtr );
Std_ReturnType CanTp_CancelTransmit( PduIdType TxPduId );
Std_ReturnType CanTp_CancelReceive( PduIdType RxPduId );
Std_ReturnType CanTp_ChangeParameter( PduIdType id, TPParameterType parameter, int value );
Std_ReturnType CanTp_ReadParameter( PduIdType id, TPParameterType parameter, int* value );
Std_ReturnType CanTp_ReadParameter( PduIdType id, TPParameterType parameter, int* value );
void CanTp_RxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr );
void CanTp_TxConfirmation( PduIdType TxPduId, Std_ReturnType result );