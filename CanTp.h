#include "StandardTypes.h"
#include "ComStackTypes.h"

typedef struct {
    //Implementation specific cokolwiek to znaczy
    //Data structure type for the post-build configuration parameters.
} CanTp_ConfigType;

static void CanTp_Init( const CanTp_ConfigType* CfgPtr );
static void CanTp_GetVersionInfo( Std_VersionInfoType* versioninfo );
static void CanTp_Shutdown( void );
static Std_ReturnType CanTp_Transmit( PduIdType TxPduId, const PduInfoType* PduInfoPtr );
static Std_ReturnType CanTp_CancelTransmit( PduIdType TxPduId );
Std_ReturnType CanTp_CancelReceive( PduIdType RxPduId );
static Std_ReturnType CanTp_ChangeParameter( PduIdType id, TPParameterType parameter, int value );
static Std_ReturnType CanTp_ReadParameter( PduIdType id, TPParameterType parameter, int* value );
static Std_ReturnType CanTp_ReadParameter( PduIdType id, TPParameterType parameter, int* value );
static void CanTp_RxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr );
static void CanTp_TxConfirmation( PduIdType TxPduId, Std_ReturnType result );