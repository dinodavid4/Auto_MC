#include "can.h"
#include "esm.h"
#include <stdio.h>
#include "system.h"
#include "sys_core.h"
#include "sys_common.h"

#define LOW  		0
#define HIGH 		1
#define DATA_LEN	8

uint8 rx_flag = LOW;
uint8 tx_flag = HIGH;
uint8 rx_data[DATA_LEN];
uint8 *rx_ptr;

void main(void)
{
	uint8 tx_data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint8 *tx_ptr;
	tx_ptr = tx_data;
	rx_ptr = rx_data;
	/* enable irq interrupt in Cortex R4F */
	_enable_interrupt_();

	/* Configuring CAN1: MB1, Msg ID-0x80 to transmit MB2, Msg ID-0x81 to recieve */
	canInit();

	/* enabling error interrupts */
	canEnableErrorNotification(canREG1);

	while (1)
	{
		if(tx_flag == HIGH)
		{

			canTransmit(canREG1, canMESSAGE_BOX1, tx_ptr); /* transmitting data */
			//printf("Tx_data %s\n", *tx_ptr);
			tx_flag = LOW;
		}

		if(rx_flag == HIGH)
		{
			/* Compute the DATA */
			//tx_data = rx_data;
			tx_ptr = rx_ptr;
			rx_flag = LOW;
			tx_flag = HIGH;
		}
	}
}

/* can interrupt notification */
void canMessageNotification(canBASE_t *node, uint32 messageBox)
{
	if(node == canREG1)
	{
		if(messageBox == canMESSAGE_BOX1)
		{
			//tx_flag = 1; /* Transmission Completed */
			//printf("Tx_interrupt\n");
		}

		if(messageBox == canMESSAGE_BOX2)
		{
			while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX2));
			canGetData(canREG1, canMESSAGE_BOX2, rx_ptr); /* Recieve Completed */
			rx_flag = HIGH; /* Data Recieved in Message Box */
			//printf("Rx_interrupt\n");
		}
	}
}
