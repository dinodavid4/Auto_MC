/** @file sci.h
*   @brief SCI Driver Definition File
*   @date 15.July.2015
*   @version 04.05.00
*   
*/

/* 
* Copyright (C) 2009-2015 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/



#ifndef __SCI_H__
#define __SCI_H__

#include "reg_sci.h"

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN (0) */
/* USER CODE END */

/** @enum sciIntFlags
*   @brief Interrupt Flag Definitions
*
*   Used with sciEnableNotification, sciDisableNotification
*/
enum sciIntFlags
{
    SCI_FE_INT    = 0x04000000U,  /* framing error */
    SCI_OE_INT    = 0x02000000U,  /* overrun error */
    SCI_PE_INT    = 0x01000000U,  /* parity error */
    SCI_RX_INT    = 0x00000200U,  /* receive buffer ready */
    SCI_TX_INT    = 0x00000100U,  /* transmit buffer ready */
    SCI_WAKE_INT  = 0x00000002U,  /* wakeup */
    SCI_BREAK_INT = 0x00000001U   /* break detect */
};

/** @def SCI_IDLE
*   @brief Alias name for the SCI IDLE Flag
*
*   This is an alias name for the SCI IDLE Flag.
*
*/
#define SCI_IDLE 0x00000004U

/** @struct sciBase
*   @brief SCI Register Definition
*
*   This structure is used to access the SCI module registers.
*/
/** @typedef sciBASE_t
*   @brief SCI Register Frame Type Definition
*
*   This type is used to access the SCI Registers.
*/

enum sciPinSelect
{
    PIN_SCI_TX = 4U,
    PIN_SCI_RX = 2U
};


/* Configuration registers */
typedef struct sci_config_reg
{
    uint32 CONFIG_GCR0;
    uint32 CONFIG_GCR1;
    uint32 CONFIG_SETINT;
    uint32 CONFIG_SETINTLVL;
    uint32 CONFIG_FORMAT;
    uint32 CONFIG_BRS;
    uint32 CONFIG_PIO0;
    uint32 CONFIG_PIO1;
    uint32 CONFIG_PIO6;
    uint32 CONFIG_PIO7;	
	uint32 CONFIG_PIO8;
} sci_config_reg_t;




/** 
 *  @defgroup SCI SCI
 *  @brief Serial Communication Interface Module.
 *  
 *  The SCI module is a universal asynchronous receiver-transmitter that implements the standard nonreturn
 *  to zero format. The SCI can be used to communicate, for example, through an RS-232 port or over a K-line.
 *
 *	Related Files
 *   - reg_sci.h
 *   - sci.h
 *   - sci.c
 *  @addtogroup SCI
 *  @{
 */
 
/* SCI Interface Functions */
void sciInit(void);
void sciSetFunctional(sciBASE_t *sci, uint32 port);
void sciSetBaudrate(sciBASE_t *sci, uint32 baud);
uint32  sciIsTxReady(sciBASE_t *sci);
void sciSendByte(sciBASE_t *sci, uint8 byte);
void sciSend(sciBASE_t *sci, uint32 length, uint8 * data);
uint32  sciIsRxReady(sciBASE_t *sci);
uint32  sciIsIdleDetected(sciBASE_t *sci);
uint32  sciRxError(sciBASE_t *sci);
uint32  sciReceiveByte(sciBASE_t *sci);
void sciReceive(sciBASE_t *sci, uint32 length, uint8 * data);
void sciEnableNotification(sciBASE_t *sci, uint32 flags);
void sciDisableNotification(sciBASE_t *sci, uint32 flags);
void sciEnableLoopback(sciBASE_t *sci, loopBackType_t Loopbacktype);
void sciDisableLoopback(sciBASE_t *sci);
/** @fn void sciNotification(sciBASE_t *sci, uint32 flags)
*   @brief Interrupt callback
*   @param[in] sci   - sci module base address
*   @param[in] flags - copy of error interrupt flags
*
* This is a callback that is provided by the application and is called upon
* an interrupt.  The parameter passed to the callback is a copy of the 
* interrupt flag register.
*/
void sciNotification(sciBASE_t *sci, uint32 flags);

/* USER CODE BEGIN (1) */
/* USER CODE END */
/**@}*/
#ifdef __cplusplus
}
#endif

#endif
