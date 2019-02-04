#ifndef CANTP_H_
#define CANTP_H_

#include "ComStackTypes.h"

// CanTpConfig
#define     CanTpMainFunctionPeriod         (0.2)
#define     CanTpMaxChannelCnt              (1)

// CanTpGeneral
#define     CanTpChangeParameterApi         (0)
#define     CanTpDevErrorDetect             (0)
#define     CanTpDynIdSupport               (0)
#define     CanTpFlexibleDataRateSupport    (0)
#define     CanTpGenericConnectionSupport   (0)
#define     CanTpPaddingByte                (0xAA)
#define     CanTpReadParameterApi           (0)
#define     CanTpVersionInfoApi             (0)

// ddoda się jak będzie potrzebne
// CanTpChannel
// CanTpRxNSdu
// CanTpTxNSdu

#define     CANTP_OFF                       (0)
#define     CANTP_ON                        (1)

#define     CANTP_RX_WAIT                   (2)
#define     CANTP_RX_PROCESSING             (3)

#define     CANTP_TX_WAIT                   (2)
#define     CANTP_TX_PROCESSING             (3)

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

#endif // CANTP_H_