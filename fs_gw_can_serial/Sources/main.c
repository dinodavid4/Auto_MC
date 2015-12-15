
#include "derivative.h" /* include peripheral declarations */
#include "CLK.h"		/*include the CLK initialisation declaration*/
#include "MSCAN_Module.h"/* include the msCAN module enable declaration*/
#include "Test_case.h"	/*include the Test Cases enumeration*/
#include "UART.h"
#include "typedefs.h"
#include <stdio.h>


#define TEST_CASE_FUNC(index) (Test_Case_Func[index]())
/*User defined function API to decide which test_case for test
 * parameters
 * 			index: one of the enumeration test_case_no defined in Test_case.h to indicate which one DEMO to run
 * 			    Demo_Transmit_DataFrame,              
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
 * return: none
 * note: this function is an API to user to use test_cases for MSCAN
 */

#define UserDefine

void delay_ms(UINT16 delay);
void Uart2_Receive(UINT8 rxBuffer);
void Enable_Interrupt(UINT8 vector_number);
/*main function block used for test case
 * 
 */
#ifdef TestCase
int main(void)
{
	Clk_Init();

	MSCAN_ModuleEn();
	GPIOB_PDDR |= (1<<16);//to enable ths STB mode of MC33901
	GPIOB_PCOR = (1<<16);

	while(1)
	{
		TEST_CASE_FUNC(Demo_Transmit_DataFrame);//set different parameter to choose other test case to run
		delay_ms(1);
	}
	return 0;
}
#endif

#ifdef UserDefine
int main(void)
{
	//uint8 data[9] = { 8, 'F', 'R', 'E', 'E', 'S', 'C', 'A', 'L' };
	Clk_Init();
	UART_Init();
	Uart_SetCallback(Uart2_Receive);
	 

	MSCAN_ModuleEn();
	GPIOB_PDDR |= (1<<16);//to enable ths STB mode of MC33901
	GPIOB_PCOR = (1<<16);

	EnableInterrupts;//Enable CAN interrupts 
	Init_CAN(0,FAST);//Initializen msCAN channel 0 and switch without completing current transmission(FAST) 
	 
	Config_CAN_MB (0, 1, TXDF, 1); //Configure the msCAN channel 0 buffer 1 to be TXDF mode (transmit dataframe), "0" means the message to 
	                                             //transmit is the message object 0 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x81 which can be modified;
	Enable_Interrupt(INT_UART2);
	while(1)
	{
		//Load_CAN_MB (0, 1, data);//Load the data to the buffer of Channel 0 buffer 1
		//Transmit_CAN_MB(0, 1);//Transmit the frame of Channel 0 buffer 1;
		//delay_ms(1);
	}
	
	return 0;
}
#endif

void Uart2_Receive(UINT8 rxBuffer)
{
	static uint8 can_data_len = 0;
	static uint8 data[9] = {8, 0, 0, 0, 0, 0, 0, 0, 0};
	can_data_len++;
	data[can_data_len] = rxBuffer;
	
	if(can_data_len == 8)
	{
		can_data_len = 0;
		Load_CAN_MB (0, 1, data);//Load the data to the buffer of Channel 0 buffer 1
		Transmit_CAN_MB(0, 1);//Transmit the frame of Channel 0 buffer 1;

			printf("Length 8\n");
	}
	//Uart_SendChar(rxBuffer);
	//printf("Recieved UART data = %c\n", rxBuffer);
	//TODO: Frame the CAN frame here to transmit to HERCULES and then call CAN transmit function
}

void Enable_Interrupt(UINT8 vector_number)
{
	vector_number= vector_number-16;
	
	/* Set the ICPR and ISER registers accordingly */
	NVIC_ICPR |= 1 << (vector_number%32);
	NVIC_ISER |= 1 << (vector_number%32);

}

void delay_ms(UINT16 delay)
{
	UINT32 i=0;
	while (i<delay*1333)
	{ i++; }	
}
