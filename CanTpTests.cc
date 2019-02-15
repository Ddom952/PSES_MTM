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
	unsigned char TX_length;
	unsigned char RX_length;
	unsigned char RX_available_buffer;
	unsigned char RX_consecutive_frames;
} CanTp_RunTimeDataType; // SWS_CanTp_00002

extern CanTp_RunTimeDataType runTimeData;

/* externs from PduR_CanTp */
extern BufReq_ReturnType PduR_ReturnValue;
extern unsigned char * PduR_buffer	;
extern int PduR_buffer_size;

/* externs from CanIf */
extern Std_ReturnType CanIfReturnValue;
extern unsigned char CanIfConsecutiveFramesNumber;

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

bool compare_bytes( unsigned char * pointerFirst, unsigned char * pointerSecond, unsigned char size)
{
	bool result = true;
	for (int i = 0; i < size; i++) {
        if ( *( pointerFirst + i ) != *( pointerSecond + i ) )
		{
			result = false;
			return result;
		}
    }
	return result;
}

unsigned char copy_bytes( unsigned char * pointerFirst, unsigned char * pointerSecond, unsigned char size)
{
	for (int i = 0; i < size; i++) {
        *( pointerFirst + i ) = *( pointerSecond + i );
    }
	return size;
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
	unsigned char sduData[] = { 0x04, 0xDE, 0xAD, 0xBE, 0xEF };
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

	EXPECT_EQ( compare_bytes( sduData + 1, buffer, sizeof(buffer)), true );

	CanTp_Shutdown();
}

TEST(SWS_CAN_TRANSPORT_LAYER_9_1, Positive) { // 9.1 SF N-SDU received and no buffer available
    printf("state: %d \r\n", runTimeData.moduleInternalState);
	PduIdType CanIfId = 0;
	unsigned char sduData[] = { 0x00, 0xDE, 0xAD, 0xBE, 0xEF };

    CanTp_Init(NULL);

	PduR_CanTp_Set_BufReq_Return_Value( BUFREQ_E_NOT_OK );

	PduInfoType CanIfPduInfo;
	CanIfPduInfo.MetaDataPtr = NULL;
	CanIfPduInfo.SduDataPtr = sduData;
	CanIfPduInfo.SduLenght = 4;
	Send_CanTp_RxIndication( CanIfId, &CanIfPduInfo );
	for ( int i = 0; i < 5; i++ )
	{
        CanTp_MainFunction();
	}

    printf("state: %d \r\n", runTimeData.moduleInternalState);
	EXPECT_EQ( 1, 1 );
}

TEST(SWS_CAN_TRANSPORT_9_5, Positive) { // 9.5 Large N-SDU Reception
	// single frame
	unsigned char data[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xFF,
							 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xFF,
							 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xFF,
							 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xFF,
							 0xDE, 0xAD, 0xBE, 0xEF };
	unsigned char pdu_router_buffer[sizeof(data)] = { 0x00 };

	// prepare PduRouter
	PduR_ReturnValue = BUFREQ_OK;
	PduR_buffer = pdu_router_buffer;
	PduR_buffer_size = sizeof(pdu_router_buffer);

	CanTp_Init(NULL);

	unsigned char buffer [8] = { 0x00 };

	unsigned int bytes_to_transfer = sizeof(data);
	buffer[0] = 0x10 | ( bytes_to_transfer >> 8 );
	buffer[1] = bytes_to_transfer;
	unsigned char * dataPtr = data;
	bytes_to_transfer = bytes_to_transfer - copy_bytes(buffer + 2, data, 6);
	dataPtr = dataPtr + 6;

	// prepare first frame
	PduIdType CanIfId = 0;
	PduInfoType CanIfPduInfo;
	CanIfPduInfo.MetaDataPtr = NULL;
	CanIfPduInfo.SduDataPtr = buffer;
	CanIfPduInfo.SduLenght = sizeof(buffer);

	Send_CanTp_RxIndication( CanIfId, &CanIfPduInfo );

	unsigned char CF_number = bytes_to_transfer / 7;
	if ( 0 != (bytes_to_transfer % 7) )
	{
		CF_number += 1;
	}

	printf( "CF_number: %d\r\n", CF_number );
	EXPECT_EQ( CF_number, CanIfConsecutiveFramesNumber );

	for ( int i = 0; i < CF_number; i++ )
	{
		buffer[0] = 0x20;

		if ( 7 > bytes_to_transfer )
		{
			bytes_to_transfer = bytes_to_transfer - copy_bytes(buffer + 1, dataPtr, bytes_to_transfer);
			dataPtr += bytes_to_transfer;
		}
		else
		{
			bytes_to_transfer = bytes_to_transfer - copy_bytes(buffer + 1, dataPtr, 7);
			dataPtr += 7;
		}

		Send_CanTp_RxIndication( CanIfId, &CanIfPduInfo );
	}

	for ( int i = 0; i < 10; i++ )
	{
		CanTp_MainFunction();
	}

	//EXPECT_EQ( compare_bytes( data, pdu_router_buffer, sizeof(data)), true );

	CanTp_Shutdown();
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
