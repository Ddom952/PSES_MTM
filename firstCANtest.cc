#include <limits.h>
#include "CanTp.h"
#include "CanIf.h"
#include "PduR_CanTp.h"
#include "SchM_CanTp.h"
#include "gtest/gtest.h"

/* copy of defined types in CanTp.c */
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

extern CanTp_RunTimeDataType runTimeData;

/* externs from PduR_CanTp */
extern BufReq_ReturnType PduR_ReturnValue;
extern unsigned char * PduR_buffer	;
extern int PduR_buffer_size;

namespace {

void print_bytes( char * name, unsigned char * pointer, unsigned char size)
{
	printf("\r\n");
	printf(name);
	printf(":\r\n");
	for (int i = 0; i < size; i ++) {
        printf(" %2x", pointer[i]);
    }
	printf("\r\n");
}

// Check empty funtion
TEST(CanTp_CanTp_CancelReceive, Positive) {
	EXPECT_EQ( (Std_ReturnType) E_OK, CanTp_CancelReceive( (PduIdType) 11));
	CanTp_MainFunction();
}

TEST(CanTp_Init_TEST, Positive) { // SWS_CanTp_00170
	CanTp_Init(NULL);

	EXPECT_EQ( CANTP_ON, runTimeData.moduleInternalState );

	CanTp_Shutdown();
}

TEST(CanTp_Shutdown_TEST, Positive) { // SWS_CanTp_00010
	CanTp_Init(NULL);

	CanTp_Shutdown();

	EXPECT_EQ( CANTP_OFF, runTimeData.moduleInternalState );
}

TEST(CanTp_Init_Shutdown_TEST, Positive) { // SWS_CanTp_00170
	CanTp_Init(NULL);
	EXPECT_EQ( CANTP_ON, runTimeData.moduleInternalState );

	CanTp_Shutdown();
	EXPECT_EQ( CANTP_OFF, runTimeData.moduleInternalState );
}

TEST(CanTp_Reception, Positive) { // Single frame send 4 bytes
	// single frame
	unsigned char sduData[] = { 0x00, 0xDE, 0xAD, 0xBE, 0xEF };
	unsigned char buffer[4] = { 0x00 };

	// prepare PduRouter
	PduR_ReturnValue = BUFREQ_OK;
	PduR_buffer = buffer;
	PduR_buffer_size = sizeof(buffer);

	PduIdType CanIfId = 0;
	PduInfoType CanIfPduInfo;
	CanIfPduInfo.MetaDataPtr = NULL;
	CanIfPduInfo.SduDataPtr = sduData;
	CanIfPduInfo.SduLenght = 4;

	CanTp_Init(NULL);

	Send_CanTp_RxIndication( CanIfId, &CanIfPduInfo );

	print_bytes("sduData", sduData, sizeof(sduData));
	print_bytes("buffer", buffer, sizeof(buffer));

	int cmp_result = strcmp( (char *) sduData + 1, (char *) buffer );

	EXPECT_EQ( cmp_result, 0 );
}

// 9.1 SF N-SDU received and no buffer available
TEST(SWS_CAN_TRANSPORT_LAYER_9_1, Positive) {
    printf("state: %d \r\n", runTimeData.moduleInternalState);
	PduIdType CanIfId = 0;
	unsigned char sduData[] = { 0xDE, 0xAD, 0xBE, 0xEF };
	unsigned char metaDataPtr[] = { 0xDE, 0xAD, 0xBE, 0xEF };

    CanTp_Init(NULL);

	PduR_CanTp_Set_BufReq_Return_Value( BUFREQ_E_NOT_OK );

	PduInfoType CanIfPduInfo;
	CanIfPduInfo.MetaDataPtr = metaDataPtr;
	CanIfPduInfo.SduDataPtr = sduData;
	CanIfPduInfo.SduLenght = 4;
	Send_CanTp_RxIndication( CanIfId, &CanIfPduInfo );
	for ( int i = 0; i < 5; i++ )
	{
        CanTp_MainFunction();
	}

    printf("state: %d \r\n", runTimeData.moduleInternalState);
	//EXPECT_EQ(  1, 1 );
}

/* Mock Modules Function tests */

TEST(CanIf_Test, Positive) {
	Set_CanIf_Return_Value(E_OK);
	EXPECT_EQ( (Std_ReturnType) E_OK, CanIf_Transmit( 0, NULL));
	Set_CanIf_Return_Value(E_NOT_OK);
	EXPECT_EQ( (Std_ReturnType) E_NOT_OK, CanIf_Transmit( 0, NULL));
}

TEST(PduRouterCAN_Test, Positive) {
	BufReq_ReturnType TestReturnType = BUFREQ_E_NOT_OK;
	PduR_CanTp_Set_BufReq_Return_Value( TestReturnType );
	EXPECT_EQ(  TestReturnType, PduR_CanTpCopyRxData( 0, NULL, NULL ));
	EXPECT_EQ(  TestReturnType, PduR_CanTpCopyTxData( 0, NULL, NULL, NULL ));
	EXPECT_EQ(  TestReturnType, PduR_CanTpStartOfReception( 0, NULL, 0, NULL ));

	TestReturnType = BUFREQ_E_BUSY;
	PduR_CanTp_Set_BufReq_Return_Value( TestReturnType );
	EXPECT_EQ(  TestReturnType, PduR_CanTpCopyRxData( 0, NULL, NULL ));
	EXPECT_EQ(  TestReturnType, PduR_CanTpCopyTxData( 0, NULL, NULL, NULL ));
	EXPECT_EQ(  TestReturnType, PduR_CanTpStartOfReception( 0, NULL, 0, NULL ));

	TestReturnType = BUFREQ_E_OVFL;
	PduR_CanTp_Set_BufReq_Return_Value( TestReturnType );
	EXPECT_EQ(  TestReturnType, PduR_CanTpCopyRxData( 0, NULL, NULL ));
	EXPECT_EQ(  TestReturnType, PduR_CanTpCopyTxData( 0, NULL, NULL, NULL ));
	EXPECT_EQ(  TestReturnType, PduR_CanTpStartOfReception( 0, NULL, 0, NULL ));

	TestReturnType = BUFREQ_OK;
	PduR_CanTp_Set_BufReq_Return_Value( TestReturnType );
	EXPECT_EQ(  TestReturnType, PduR_CanTpCopyRxData( 0, NULL, NULL ));
	EXPECT_EQ(  TestReturnType, PduR_CanTpCopyTxData( 0, NULL, NULL, NULL ));
	EXPECT_EQ(  TestReturnType, PduR_CanTpStartOfReception( 0, NULL, 0, NULL ));
}

}  // namespace
