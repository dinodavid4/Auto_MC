/*
 * Test_case.h
 *
 *  Created on: Apr 16, 2014
 *      Author: Kitty
 */

#ifndef TEST_CASE_H_
#define TEST_CASE_H_

#include "msCANdrv.h"

typedef enum test_case_no{
Demo_Transmit_DataFrame,              
Demo_Receive_DataFrame,                
Demo_Receive_DataFrame_Random,         
Demo_Get_TimeStamp,                    
Demo_RemoteFrame_Automatic_Function,   
Demo_Get_msCAN_status,                 
Demo_Sleep_And_Wakeup,                 
Demo_Get_ERR_Counter,                 
Demo_Abort_Transmission,               
Demo_Callback_Functions,               
Demo_ReceiveTransmit,   
Demo_Remote_Frame       
}TEST_CASE_NO;

typedef void(*Test_Case_Func_Ptr)(void);
Test_Case_Func_Ptr Test_Case_Func[12];

#endif /* TEST_CASE_H_ */
