/*
 * Test_case.c
 *
 *  Created on: Apr 16, 2014
 *      Author: Kitty
 */
#include "Test_case.h"		 /*Header to the Test_case functions*/
#include "derivative.h"
/****************************************************************************** 
* Transmit_DataFrame
* This function shows how to transmit a dataframe through a msCAN channel 
******************************************************************************/
 void Transmit_DataFrame(void) {
 
 uint8 err_status;    //To store the return value of the driver APIs.
 
 /*the first number of the array "6" means the transmitted data length of
 the transmitted data,the MAX datalength is 8, the rest numbers are the data
 bytes to transmit.*/
 uint8 data[9] = { 8, 'H', 'E', 'R', 'C', 'U', 'L', 'E', 'S' };
 
 EnableInterrupts;
 
 err_status = Init_CAN(0,FAST);		         //Initialization msCAN channel 0, FAST means the reset won't wait until current transmission completed 
 
 err_status = Config_CAN_MB (0, 1, TXDF, 1); //Configure the msCAN channel 0 buffer 1 to be TXDF mode (transmit dataframe), "0" means the message to 
                                             //transmit is the message object 0 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x80 which can be modified;
 
 err_status = Load_CAN_MB (0, 1, data);		 //Load the data[7] to the buffer of Channel 0 buffer 1

 err_status = Transmit_CAN_MB(0, 1);		 //Transmit the frame of Channel 0 buffer 1;
 err_status = err_status;                     /*prevent compiling warning*/
 }

/****************************************************************************** 
* Receive_DataFrame
* This function shows how to Receive an expected dataframe through a msCAN channel
* Use msCAN channel 0 buffer 0 to receive a 0x80 dataframe and store to the specific buffer, then use msCAN channel 0 buffer 1 
* to send it out 
******************************************************************************/
 
void Receive_DataFrame(void) {
 
    uint8 err_status;    //To store the return value of the driver APIs.

    uint8 data_rec[9];                             //The first number of data_rec is the datalenth, the max data length is 8, so the max length of data_rec should be 9
    uint8 buffer_status[2];						//the array get the required buffer status,the first byte is status,the second byte is ID Ref
    uint8 CAN_status[3];						    //the array get the required msCAN Module status, the first byte CANCTL0, second CANCTL1, third CANRFLG
				    
    EnableInterrupts;
 
    err_status = Init_CAN(0, FAST);		        //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
 
    while ((CAN_status[0] & SYNCH) == 0) 	     	//Wait for msCAN channel 0 synchronized to CAN Bus
    {	
        err_status = Check_CAN_Status(0, CAN_status);
    }
      
    err_status = Config_CAN_MB (0, 1, TXDF, 1); //Configure the msCAN channel 0 buffer 1 to be TXDF mode (transmit dataframe), "1" means the message to send out with ID:0x81;                                               //transmit is the message object 1 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x81,Can be modified;
    err_status = Config_CAN_MB (0, 0, RXDF, 0);	//Configure the msCAN channel 0 buffer 0 to be RXDF mode (receive dataframe),"0" means to receive the specific frame with ID:0x80
    
    err_status = Check_CAN_MB_Status(0, 0, buffer_status);
    while (buffer_status[0] != NEWDATA) 			//Wait for the Receive ISR to finish and change the buffer status, NEWDATA indicates that the buffer has receive a new data
	{	
		err_status = Check_CAN_MB_Status(0, 0, buffer_status);
	}
	err_status = Read_CAN_MB_Data(0, 0, data_rec);
	/*transmit received message by CAN channel 0 message buffer 1.*/
	err_status = Load_CAN_MB(0, 1, data_rec);
	err_status = Transmit_CAN_MB(0, 1);
	err_status = err_status;
}

/*********************************************************************************************
* Receive_DataFrame_Random
* This function shows how to Receive an unexpected dataframe through a msCAN channel
* Use msCAN channel 0 receive a random dataframe
* A successful received message will be stored to the realtime buffer of each channel which received the frame
* Note: To use the function, CAN_status in msCANcfg.h should be defined
*********************************************************************************************/ 

void Receive_DataFrame_Random(void) {
    uint8 err_status;                              //To store the return value of the driver APIs.
    /*
    uint32 ID_rec;								               //The ID of frame received
    uint16 TimeStamp;                            //Note: TIMESTAMP_CAN should  be defined to use Time_Stamp 
    uint8 data_rec[8];                           //The data should be 8 bytes to store the max length frame, if the frame DLC is less than 8, the rest bytes of data_rec[] is meaningless;
    uint8 ID_Type_rec;							             //Extended ID : 08, Standard ID : 0;
    uint8 Frame_Type_rec;							           //Remote Frame for Srandard ID : 0x10, Remote Frame for Extended ID : 0x01, DataFrame : 0;
    uint8 DLC_rec;								       	       //Data Length
  	*/
    uint8 CAN_status[3];						             //the array get the required msCAN Module status, the first byte CANCTL0, second CANCTL1, third CANRFLG
	
    EnableInterrupts;
 
    err_status = Init_CAN(0, FAST);		        //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
 
    while ((CAN_status[0] & SYNCH) == 0) 	    //Wait for msCAN channel 0 synchronized to CAN Bus
    {	
        err_status = Check_CAN_Status(0, CAN_status);
    }	
    /* if REAL_TIME_BUFFER_ENABLE == TRUE, this code block can be uncommented.
    while(1)	
    err_status = Read_REALTIME_Buffers(
                                        0, 		     //channel 0 has been used to receive the random frame
                                        &ID_rec, 
                                        &ID_Type_rec, 
                                        &Frame_Type_rec, 
                                        &DLC_rec, 
                                        data_rec, 
                                        &TimeStamp
                                       ); */
    err_status = err_status;                    // prevent comipling warning
}
/****************************************************************************** 
* Get_TimeStamp
* This function shows how to reads the time stamp in a Message Buffer.  
* This function will only read a Message Buffer which has received a Data Frame since it was last 
* configured, or transmitted a Data Frame since it was last loaded with data.
* Note: To use the timestamp function, TIMESTAMP_CAN0,TIMESTAMP_CAN1..should be define to be true in "msCANcfg.h" 
* Use msCAN channel 0 to Receive it and store to a specific buffer 
******************************************************************************/
void Get_TimeStamp(void) {
    uint8 err_status;
	uint8 data_rec[9];                             //The first number of data_rec is the datalenth, the max data length is 8, so the max length of data_rec should be 9
    uint8 buffer_status[2];						  //the array get the required buffer status,the first byte is status,the second byte is ID Ref
    uint16* rec_timestamp;							//To store the time stamp of receive channel
    uint16 my_rec_timestamp;						//The real address of *my_rec_timestamp
    uint8 CAN_status[3];						    //the array get the required msCAN Module status, the first byte CANCTL0, second CANCTL1, third CANRFLG
 
    rec_timestamp = &my_rec_timestamp;
    
    EnableInterrupts;
    err_status = Init_CAN(0, FAST);		  //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
    while ((CAN_status[0] & SYNCH) == 0) 		       //Wait for msCAN channel 0 synchronized to CAN Bus
    {	
        err_status = Check_CAN_Status(0, CAN_status);
    }
    
    err_status = Config_CAN_MB (0, 0, RXDF, 0);	   //Configure the msCAN channel 0 buffer 0 to be RXDF mode (receive dataframe),
    err_status = Check_CAN_MB_Status(0, 0, buffer_status); 
    while (buffer_status[0] != NEWDATA) 			     //Wait for the Receive ISR to finish and change the buffer status, NEWDATA indicates that the buffer has receive a new data
    {	
        err_status = Check_CAN_MB_Status(0, 0, buffer_status);  //Use this API to get the status of buffer
    }
    
    err_status = Read_CAN_MB_Data(0, 0, data_rec);        //Copy the received data in msCAN channel 0 buffer 0 to data_rec;
    err_status = Read_CAN_MB_Time(0, 0, rec_timestamp);   //copy the receive timestamp of channel 0 buffer1 to rec_timestamp
    err_status = err_status;                     /*prevent compiling warning*/
}

/****************************************************************************** 
* RemoteFrame_Automatic_Function
* This function shows how to use the driver to send a Remote frame and a quested channel will automatically
* send the CAN message, and the message will be stored to the buffer of the Remote frame above automatically.   
* Use msCAN channel 0 buffer 1 to Send a 0x80 Remote frame and the msCAN channel 0 buffer 0 will be 
* configured to answer the quest and send a corresponding Message which will be 
* received by channel 0 buffer 1 and stored to the buffer of the Remote frame.
* Note : The ID of Remote frame should be equal to the ID of AUTOTXDF Mode buffer
* 		 void RemoteFrame_Automatic_Function(void) works well demanding the Macro LOOPBACK_CAN0 in  
* 		 the file msCANcfg.h set to be "TRUE"
******************************************************************************/
void RemoteFrame_Automatic_Function(void) {
 
    uint8 err_status;    //To store the return value of the driver APIs.
 
    /*the first number of the array "6" means the transmitted data length of
    the transmitted data,the MAX datalength is 8, the rest numbers are the data
    bytes for transmission.*/
    uint8 data[9] = { 8, 0, 0xff, 0, 0xff, 0, 0xff, 0, 0xff };
    uint8 data_rec[9];                //The first number of data_rec is the datalenth, the max data length is 8, so the max length of data_rec should be 9
    uint8 buffer_status[2];						//the array get the required buffer status,the first byte is status,the second byte is ID Ref
    uint8 CAN_status[3];						  //the array get the required msCAN Module status, the first byte CANCTL0, second CANCTL1, third CANRFLG

		
    EnableInterrupts;
 
    err_status = Init_CAN(0, FAST);		              //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
 
    while ((CAN_status[0] & SYNCH) == 0) 	    	    //Wait for msCAN channel 1 synchronized to CAN Bus
    {	
        err_status = Check_CAN_Status(0, CAN_status);
    }
    
    err_status = Config_CAN_MB (0, 1, TXRF, 0);     //Configure the msCAN channel 0 buffer 1 to be TXRF mode (transmit Remote frame), "0" means the message to 
                                                    //transmit is the message object 0 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x80,Can be modified;
 
    err_status = Config_CAN_MB (0, 0, AUTOTXDF, 0); //Configure the msCAN channel 0 buffer 0 to be AUTOTXDF mode (Transmit dataframe on receiving the corresponding Remote Frame),
 
    err_status = Load_CAN_MB (0, 0, data);		      //Load the data[9] to the buffer of Channel 0 buffer 0
 
    err_status = Transmit_CAN_MB(0, 1);		          //Transmit the remote frame of Channel 0 buffer 1;
 
    while (buffer_status[0] != NEWDATA) 			      //Wait for the Receive ISR to finish and change the buffer status, NEWDATA indicates that the buffer has receive a new data
    {	
        err_status = Check_CAN_MB_Status(0, 1, buffer_status);  //Use this API to get the status of buffer
    }
    err_status = Read_CAN_MB_Data(0, 1, data_rec);  //Copy the received data in msCAN channel 0 buffer 1 to data_rec;
    err_status = err_status;                     /*prevent compiling warning*/
}

/****************************************************************************** 
* Get_msCAN_status
* This Demo shows how to use the driver to get the status of a msCAN channel
* Note: Status consists of CANCTL0 and CANCTL1 register, These two register indicate
* the working mode and status of each msCAN channel
******************************************************************************/
void Get_msCAN_status(void) {
 
    uint8 err_status;                   //To store the return value of the driver APIs.
 
    uint8 CAN_status[3];							//CAN_status to store channel status,byte 0 store CANCTL0, byte 1 store CANCTL1, byte 2 store CANRFLG
  
		
    EnableInterrupts;
 
    err_status = Init_CAN(0, FAST);		        //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
 
    err_status = Check_CAN_Status(0,CAN_status);   //read the channel0 status
 
    err_status = Clear_CAN_Status(0); 				//clear CAN status, write 1's to clear all flags in CANRFLG except RXF
    err_status = err_status;                     /*prevent compiling warning*/
}

/****************************************************************************** 
* Sleep_And_Wakeup
* This function shows how to use the driver to make a msCAN channel go to sleep mode and wake up.
******************************************************************************/

void Sleep_And_Wakeup(void) {
 
    uint8 err_status;    //To store the return value of the driver APIs.
 
    /*the first number of the array "8" means the transmitted data length of
    the transmitted data,the MAX datalength is 8, the rest numbers are the data
    bytes for transmission.*/
    uint8 data[9] = { 8, 0, 0xff, 0, 0xff, 0, 0xff, 0, 0xff };
 
    uint8 CAN_status[3];							//CAN_status to store channel status, byte 0 store CANCTL0, byte 1 store CANCTL1, byte 2 store CANRFLG
  
		
    EnableInterrupts;
 
    err_status = Init_CAN(0, FAST);		            //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
 
    while ((CAN_status[0] & SYNCH) == 0) 	     	  //Wait for msCAN channel 0 synchronized to CAN Bus
    {	
        err_status = Check_CAN_Status(0, CAN_status);
    }
 
    err_status = Config_CAN_MB (0, 1, TXDF, 0);   //Configure the msCAN channel 0 buffer 1 to be TXDF mode (transmit dataframe), "0" means the message to 
                                                  //transmit is the message object 0 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x80,Can be modified;
    err_status = Load_CAN_MB (0, 1, data);		    //Load the data to the buffer of Channel 0 buffer 1
  
    err_status = Transmit_CAN_MB(0, 1);		        //Transmit the frame of Channel 0 buffer 1;
                                                  //Transmission completed, return ERR_OK
 
    err_status = Sleep_CAN(0, FAST);              //CAN channel 0 go to sleep without completing transmission scheduled, if Sleep_CAN(1,CMPTX),go to sleep after all transmission.

    while (!(CAN_status[1]&SLPAK)) 				        //When SLPAK sets bit, channel go to sleep mode
    {
 	      err_status = Check_CAN_Status(0,CAN_status);  //read the channel 0 status
    }												  
     
     												                      //Sleep success
    err_status = Wakeup_CAN(0); 				          //CAN channel 0 wakeup
  
    while ((CAN_status[0] & SYNCH) == 0) 	     	  //Wait for msCAN channel 0 synchronized to CAN Bus
    {	
        err_status = Check_CAN_Status(0, CAN_status);
    }
 
     											                   //Channel wake up success
    err_status = Transmit_CAN_MB(0, 1);		   //Transmit the frame of Channel 0 buffer 1;
    err_status = err_status;                     /*prevent compiling warning*/                                         //Transmission completed, return ERR_OK
}			   


/****************************************************************************** 
* Get_ERR_Counter
* This function shows how to get the Transmit Error Counter Register and Receive Error Counter Register 
* Use msCAN channel 0 buffer 1 to Send a 0x80 dataframe and use msCAN channel 0 buffer 2 to Receive
* Read the Transmit Error Counter Register and Receive Error Counter Register of channel 0
* Note:	void Get_ERR_Counter(void) works well demanding the Macro LOOPBACK_CAN0 in  
* 		the file msCANcfg.h set to be "TRUE"
******************************************************************************/
void Get_ERR_Counter(void) {
 
    uint8 err_status;    //To store the return value of the driver APIs.
 
    /*the first number of the array "6" means the transmitted data length of
    the transmitted data,the MAX datalength is 8, the rest numbers are the data
    bytes for transmission.*/
    uint8 data[7] = { 6, 0, 0xff, 0, 0xff, 0, 0xff };
    uint8 data_rec[7];                //The first number of data_rec is the datalenth, the max data length is 8, so the max length of data_rec should be 9
    uint8 buffer_status[2];						//the array get the required buffer status,the first byte is status,the second byte is ID Ref
    uint8 Tran_Err_Counter;						//To store the Transmit Error Counter Register of channel 0
    uint8 Rec_Err_Counter;					  //To store the Receive Error Counter Register of channel 1
    uint8 CAN_status[3];						  //the array get the required msCAN Module status, the first byte CANCTL0, second CANCTL1, third CANRFLG
 
		
    EnableInterrupts;
 
    err_status = Init_CAN(0, FAST);		          //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
 
    while ((CAN_status[0] & SYNCH) == 0) 	    	//Wait for msCAN channel 0 synchronized to CAN Bus
    {	
        err_status = Check_CAN_Status(0, CAN_status);
    }
    
    err_status = Config_CAN_MB (0, 1, TXDF, 0); //Configure the msCAN channel 0 buffer 1 to be TXDF mode (transmit dataframe), "0" means the message to 
                                                //transmit is the message object 0 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x80,Can be modified;
 
    err_status = Config_CAN_MB (0, 2, RXDF, 0);	//Configure the msCAN channel 0 buffer 2 to be RXDF mode (receive dataframe),
 
    err_status = Load_CAN_MB (0, 1, data);		  //Load the data[7] to the buffer of Channel 0 buffer 1
  
    err_status = Transmit_CAN_MB(0, 1);		      //Transmit the frame of Channel 0 buffer 1;
 
    while (buffer_status[0] != NEWDATA) 			  //Wait for the Receive ISR to finish and change the buffer status, NEWDATA indicates that the buffer has receive a new data
    {	
        err_status = Check_CAN_MB_Status(0, 2, buffer_status);
    }
    
    err_status = Read_CAN_MB_Data(0, 2, data_rec); //Copy the received data in msCAN channel 1 buffer 1 to data_rec;
 
    err_status = Read_Tran_Err_Counter (0, &Tran_Err_Counter);  //Get the Transmit error counter of channel 0 
 
    err_status = Read_Rec_Err_Counter (0, &Rec_Err_Counter);    //Get the Receive error counter of channel 1
    err_status = err_status;                     /*prevent compiling warning*/
}

/****************************************************************************** 
* Abort_Transmission
* This function shows how to use the driver to abort a queued transmit buffer
* Note: The API can only abort queued buffer, and if the message has be written
* in a TX message buffer in the MCU (in msCAN, the buffer number is 3), the 
* abort request will be ignored and the message will be sent successfully
* The return value "err_status" indicates whether the abortion is done successfully
******************************************************************************/ 
void Abort_Transmission(void) {
 
    uint8 err_status;    //To store the return value of the driver APIs.
 
    /*the first number of the array "6" means the transmitted data length of
    the transmitted data,the MAX datalength is 8, the rest numbers are the data
    bytes for transmission.*/
    uint8 data0[7] = { 6, 0, 0xff, 0, 0xff, 0, 0xff }; 
    uint8 data1[9] = { 8, 1, 2, 3, 4, 5, 6, 7, 8 };
  
		
    EnableInterrupts;
 
    err_status = Init_CAN(0,FAST);		          //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
 
    err_status = Config_CAN_MB (0, 1, TXDF, 0); //Configure the msCAN channel 0 buffer 1 to be TXDF mode (transmit dataframe), "0" means the message to 
                                                //transmit is the message object 0 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x80,Can be modified;
                                             
    err_status = Config_CAN_MB (0, 2, TXDF, 5); //Configure the msCAN channel 0 buffer 2 to be TXDF mode (transmit dataframe), "5" means the message to 
                                                //transmit is the message object 5 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x85,Can be modified;
                                             
    err_status = Load_CAN_MB (0, 1, data0);		  //Load the data0[7] to the buffer of Channel 0 buffer 1
 
    err_status = Load_CAN_MB (0, 2, data1);		  //Load the data1[9] to the buffer of Channel 0 buffer 2
   
    err_status = Transmit_CAN_MB(0, 1);		      //Transmit the frame of Channel 0 buffer 1;
 
    err_status = Transmit_CAN_MB(0, 2);		      //Transmit the frame of Channel 0 buffer 2;
 
    err_status = Abort_CAN_MB(0, 2);            //error_status return 0, abort succeed; in this case, abort of Channel 0 buffer 1
    err_status = err_status;                     /*prevent compiling warning*/                                           //won't succeed because the transmission has started and not in queue status, won't be aborted
}

 /*************************************************************************************************  
* Callback_Functions
* This function shows how to use the driver to register a Callback_Function, how to
* delete a Callback_Function of a msCAN channel
* This Demo shows how to write a correct user defined callback_function and use it 
* For Example: If the Receiver full interrupt enable bit is disabled,then the data in the FIFO will not be read,
* in the situation the Overrun interrupt will generated.
* Note: To use the callback function, make sure the "Err_Callback_Enable" be defined in msCANcfg.h
**************************************************************************************************/
void Callback_Function_For_Channel0 (uint8 err_status);		   //All callback fuctions defined by user should be written in this type    bit7 | bit6 | bit5 | bit4 | bit3| bit2|  bit1 | bit0
 															   //and err_status is put in by driver,CANRFLG cared,err_status should be  RSTAT1|RSTAT0|TSTAT1|TSTAT0|CSCIF|OVRIF|NOTUSED|NOTUSED
uint8 Channel0_err_status;
 
void Callback_Function(void) {
 
    uint8 err_status;    //To store the return value of the driver APIs.
	uint8 CAN_status[2];	
    EnableInterrupts;
 
    err_status = Init_CAN(0,FAST);		         //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
    while ((CAN_status[0] & SYNCH) == 0) 	     	//Wait for msCAN channel 1 synchronized to CAN Bus
	{	
		err_status = Check_CAN_Status(0, CAN_status);
	}
    err_status = Register_Callback(0,Callback_Function_For_Channel0);	 //Callback function can not be used until register it to a msCAN channel
 
    err_status = err_status;                     /*prevent compiling warning*/
    while(1);
}
 
void Callback_Function_For_Channel0 (uint8 err_status)	  // The realization of callback of channel 0, Can be modified by user for different needs
{     
    Channel0_err_status = err_status; 
}
 

/*************************************************************************************************** 
* ReceiveTransmit
* This function shows how to use KEA128 to communicate with other MCUs with CAN protocol.
* Another MCU sends data frame 0x80 and  KEA128 receive the frame 0x80 and make the 
* received Signal plus by 1 and then send back the Signal in frame 0x81
* User can see the Two frames 0x80 and 0x81 carrying a signal always plus by 1 (Data field grows)
***************************************************************************************************/
void Data_Field_INI (int32, uint8*);
 
uint32 Convert_Data (uint8*);
 
void ReceiveTransmit(void) {
 
    uint8 err_status;         //To store the return value of the driver APIs.
 
    int32 Singal_Value;
  
    uint8 data[9]; 
 
    uint8 buffer_status[2];	//the array get the required buffer status,the first byte is status,the second byte is ID Ref

		
    EnableInterrupts; 
 
    err_status = Init_CAN(0,FAST);		          //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
 
    err_status = Config_CAN_MB (0, 1, TXDF, 1); //Configure the msCAN channel 0 buffer 1 to be TXDF mode (transmit dataframe), "0" means the message to 
                                                //transmit is the message object 1 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x81,Can be modified;
 
    err_status = Config_CAN_MB (0, 0, RXDF, 0);	//Configure the msCAN channel 0 buffer 0 to be RXDF mode (receive dataframe),
    err_status = err_status;                     /*prevent compling warning*/
 
    for(;;) 
    {        
        while (buffer_status[0] != NEWDATA)     //Wait for the Receive ISR to finish and change the buffer status, NEWDATA indicates that the buffer has receive a new data
        {	
            err_status = Check_CAN_MB_Status(0, 0, buffer_status);
        }
    
    buffer_status[0] = 0;					               //Clear buffer_status[0]
       
    err_status = Read_CAN_MB_Data(0, 0, data);   //Copy the received data in msCAN channel 0 buffer 0 to data;        
         
    Singal_Value = Convert_Data (data);
    
    Singal_Value++;
    
    Data_Field_INI(Singal_Value,data);
    
    err_status = Load_CAN_MB (0, 1, data);		  //Load the data to the buffer of Channel 0 buffer 1
  
    err_status = Transmit_CAN_MB(0, 1);		      //Transmit the frame of Channel 0 buffer 1;   
        
    }                                           /* loop forever */
                                             
}
 
void Data_Field_INI (int32 Singal_Value , uint8* data)    //Convert a uint32 parameter to four bytes and a DLC byte
{
   int i;
     
   *data = 4;
     
   for(i = 0; i < 4; i++) 
   {
       *(data+4-i) =  (uint8)((Singal_Value & ((uint32)0xff << (uint8)(8*i))) >> (uint8)(8*i));             
   }
}
  
uint32 Convert_Data (uint8* data) 		         //Convert four bytes and a DLC byte to a uint32 parameter
{
    
   uint32 Signal;
   Signal =   ((uint32)*(data+1)<<24) + ((uint32)*(data+2)<<16) + ((uint32)*(data+3)<<8) + ((uint32)*(data+4)) ; 
   return Signal;
}
 
 /***********************************************************************************************************  
* Remote_Frame
* This function shows how to use the KEA128 to receive a Remote frame and send a quested frame automatically, 
* and how to use KEA128 to send a Remote frame.
* Use msCAN channel 0 buffer 1 in KEA128 to Send a 0x80 Remote frame
* Use msCAN channel 0 buffer 2 in KEA128 to Receive a 0x81 Remote frame and send a quested frame automatically.
* Note : The ID of Remote frame should be equal to the ID of AUTOTXDF Mode buffer
***********************************************************************************************************/
void Remote_Frame(void) {
 
    uint8 err_status;                                //To store the return value of the driver APIs.
    uint8 data_rec[9];                             //The first number of data_rec is the datalenth, the max data length is 8, so the max length of data_rec should be 9
    uint8 buffer_status[2];						             //the array get the required buffer status,the first byte is status,the second byte is ID Ref
    uint8 data[5] = {4, 0x00,0xFF, 0x55, 0xBB};   // the array store the data to be automatically sent out when receiving the Remote frame.
		
    EnableInterrupts;
 
    err_status = Init_CAN(0, FAST);		             //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
  
    err_status = Config_CAN_MB (0, 1, TXRF, 0);    //Configure the msCAN channel 0 buffer 1 to be TXRF mode (transmit Remote frame), "0" means the message to 
                                                   //transmit is the message object 0 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x80,Can be modified; 
    err_status = Config_CAN_MB (0, 2, AUTOTXDF, 1);
    err_status = Load_CAN_MB(0, 2, data);
    
    err_status = Transmit_CAN_MB(0, 1);		         //Transmit the remote request frame of Channel 0 buffer 1;
 
    while (buffer_status[0] != NEWDATA) 			     //Wait for the Receive ISR to finish and change the buffer status, NEWDATA indicates that the buffer has receive a new data
    {	
        err_status = Check_CAN_MB_Status(0, 1, buffer_status);
    }
    
    err_status = Read_CAN_MB_Data(0, 1, data_rec); //Copy the received data in msCAN channel 0 buffer 1 to data_rec; 
    err_status = err_status;                     /*prevent compiling warning*/
 
}


/* Define the Test Case functions pointer to be used by the user in main Function
 * Note: use TEST_CASE_FUNC function in main.c to call the following Test Case functions.
 */

Test_Case_Func_Ptr Test_Case_Func[12] = {
Transmit_DataFrame,
Receive_DataFrame,               
Receive_DataFrame_Random,         
Get_TimeStamp,                    
RemoteFrame_Automatic_Function,   
Get_msCAN_status,                 
Sleep_And_Wakeup,                 
Get_ERR_Counter,                 
Abort_Transmission,               
Callback_Function,               
ReceiveTransmit,    
Remote_Frame    
};
