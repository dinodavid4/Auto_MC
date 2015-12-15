/*
 * CLK.c
 *
 *  Created on: Sep 24, 2013
 *      Author: B46911
 */

#include "CLK.h"
#include "derivative.h"
/***********************************************************************************************
*
* @brief    CLK_Init - Initialise the clocks to run at 20 MHz from the 10Mhz external XTAL
* @param    none
* @return   none
*
************************************************************************************************/  
//void Clk_Init()

//{
	/* the following code segment demonstrates setting ICS to FBE mode generating 20MHZ bus*/
	/* supposing external 20MHZ crystal is installed */
	//OSC_CR |=(OSC_CR_OSCEN_MASK
			//|OSC_CR_OSCSTEN_MASK
			//|OSC_CR_RANGE_MASK
			//|OSC_CR_HGO_MASK);/* enable clock in high range, high gain mode */
	//ICS_C2 = 0x00; /* BDIV=0, no prescalar*/
	//ICS_C1 = 0x80; /* external clock reference (20MHZ) to FLL output */	

//}
/*

/***********************************************************************************************
*
* @brief    CLK_Init - Initialise the clocks to run at 20 MHz from the 31.25KHz internal reference clock
* @param    none
* @return   none
*
************************************************************************************************/  
void Clk_Init()

{	
	ICS_C1|=ICS_C1_IRCLKEN_MASK; 		/* Enable the internal reference clock*/ 
	ICS_C3= 0x90;						/* Reference clock frequency = 31.25 KHz*/		
	while(!(ICS_S & ICS_S_LOCK_MASK));   /* Wait for PLL lock, now running at 40 MHz (1280 * 31.25Khz) */		
    ICS_C2|=ICS_C2_BDIV(1); 			/*BDIV=2, Bus clock = 20 MHz*/
	ICS_S |= ICS_S_LOCK_MASK; 			/* Clear Loss of lock sticky bit */			
}

