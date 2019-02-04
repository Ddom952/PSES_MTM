// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// A sample program demonstrating using Google C++ testing framework.

// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include "CanTp.h"
#include "CanIf.h"
#include "PduR_CanTp.h"
#include "SchM_CanTp.h"
#include "windows.h"
#include "gtest/gtest.h"
namespace {

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>


// Check empty funtion
TEST(CanTp_CanTp_CancelReceive, Positive) {
	EXPECT_EQ( (Std_ReturnType) E_OK, CanTp_CancelReceive( (PduIdType) 11));
	CanTp_MainFunction();
}

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

// 9.1 SF N-SDU received and no buffer available
TEST(SWS_CAN_TRANSPORT_LAYER_9_1, Positive) {
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
		Sleep(200);
        CanTp_MainFunction();
	}
	//EXPECT_EQ(  1, 1 );
}

}  // namespace

// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?
