//###########################################################################
//
// FILE:  User_Flash.c	
//
// TITLE: F2833x Flash API Example
//
// NOTE:  This example runs from Flash.  First program the example
//        into flash.  The code will then copy the API's to RAM and 
//        modify the flash. 
//
//
//###########################################################################
// $TI Release: F28335 API Release V1.0 $
// $Release Date: September 14, 2007 $
//###########################################################################


/*---- Flash API include file -------------------------------------------------*/
#include "Flash2833x_API_Library.h"

/*---- example include file ---------------------------------------------------*/
#include "User_Flash.h"

#include "DSP2833x_Device.h"			// Device Headerfile and Examples Include File

/*--- Callback function.  Function specified by defining Flash_CallbackPtr */
//void MyCallbackFunction(void); 
//Uint32 MyCallbackCounter; // Just increment a counter in the callback function
                              
/*--- Global variables used to interface to the flash routines */
FLASH_ST FlashStatus;

/*---------------------------------------------------------------------------
  Data/Program Buffer used for testing the flash API functions
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
  Sector address info
---------------------------------------------------------------------------*/
typedef struct {
     Uint16 *StartAddr;
     Uint16 *EndAddr;
} SECTOR;

#define OTP_START_ADDR  0x380400
#define OTP_END_ADDR    0x3807FF

#define FLASH_START_ADDR  0x300000
#define FLASH_END_ADDR    0x33FFFF

SECTOR Sector[8] = {
         (Uint16 *)0x338000,(Uint16 *)0x33FFFF,
         (Uint16 *)0x330000,(Uint16 *)0x337FFF,
         (Uint16 *)0x328000,(Uint16 *)0x32FFFF,
         (Uint16 *)0x320000,(Uint16 *)0x327FFF,
         (Uint16 *)0x318000,(Uint16 *)0x31FFFF,
         (Uint16 *)0x310000,(Uint16 *)0x317FFF,
         (Uint16 *)0x308000,(Uint16 *)0x30FFFF,
         (Uint16 *)0x300000,(Uint16 *)0x307FFF
};

extern Uint32 Flash_CPUScaleFactor;

//-------------------------------------------------------------------------
//Flash初始化
void User_InitFlash(void)
{
/*------------------------------------------------------------------
 To use the Flash API, the following steps
 must be followed:

      1. Modify Flash2833x_API_Config.h for your targets operating
         conditions.
      2. Include Flash2833x_API_Library.h in the application.
      3. Add the approparite Flash API library to the project.

  The user's code is responsible for the following:

      4. Initalize the PLL to the proper CPU operating frequency.
      5. If required, copy the flash API functions into on-chip zero waitstate
         RAM.  
      6. Initalize the Flash_CPUScaleFactor variable to SCALE_FACTOR
      7. Initalize the callback function pointer or set it to NULL
      8. Optional: Run the Toggle test to confirm proper frequency configuration
         of the API. 
      9. Optional: Unlock the CSM.
     10. Make sure the PLL is not running in limp mode  
     11. Call the API functions: Flash_Erase(), Flash_Program(), Flash_Verify()
         
  The API functions will:
      
       Disable the watchdog
       Check the device PARTID. 
       Disable interrupts during time critical code.  
       Perform the desired operation and return status
------------------------------------------------------------------*/

/*------------------------------------------------------------------
 Initalize the PLLCR value before calling any of the F2833x Flash API 
 functions.
        
     Check to see if the PLL needs to changed
     PLLCR_VALUE is defined in Example_Flash2833x_API.h
     1) Make sure PLL is not in limp mode
     2) Disable missing clock detect logic
     3) Make the change
     4) Wait for the DSP to switch to the PLL clock
        This wait is performed to ensure that the flash API functions 
        will be executed at the correct frequency.
     5) While waiting, feed the watchdog so it will not reset.
     6) Re-enable the missing clock detect logic 
------------------------------------------------------------------*/
   
   // Assuming PLLSTS[CLKINDIV] = 0 (default on XRSn).  If it is not 
   // 0, then the PLLCR cannot be written to. 
   // Make sure the PLL is not running in limp mode
	if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)
	{
		if (SysCtrlRegs.PLLCR.bit.DIV != PLLCR_VALUE)
		{
			EALLOW;
			// Before setting PLLCR turn off missing clock detect
			SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
			SysCtrlRegs.PLLCR.bit.DIV = PLLCR_VALUE;
			EDIS;
			
			// Wait for PLL to lock.
			// During this time the CPU will switch to OSCCLK/2 until
			// the PLL is stable.  Once the PLL is stable the CPU will 
			// switch to the new PLL value. 
			//
			// This time-to-lock is monitored by a PLL lock counter.   
			//   
			// The watchdog should be disabled before this loop, or fed within 
			// the loop.   
			
			EALLOW;
			SysCtrlRegs.WDCR= 0x0068;
			EDIS;
			
			// Wait for the PLL lock bit to be set.  
			// Note this bit is not available on 281x devices.  For those devices
			// use a software loop to perform the required count. 
			
			while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1) { }
			
			EALLOW;
			SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
			EDIS;
		}
	}   

/*------------------------------------------------------------------
    Copy API Functions into SARAM
    
    The flash API functions MUST be run out of internal 
    zero-waitstate SARAM memory.  This is required for 
    the algos to execute at the proper CPU frequency.
    If the algos are already in SARAM then this step
    can be skipped.  
    DO NOT run the algos from Flash
    DO NOT run the algos from external memory
------------------------------------------------------------------*/

    // Copy the Flash API functions to SARAM
    Example_MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);

    // We must also copy required user interface functions to RAM. 
    Example_MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

/*------------------------------------------------------------------
  Initalize Flash_CPUScaleFactor.

   Flash_CPUScaleFactor is a 32-bit global variable that the flash
   API functions use to scale software delays. This scale factor 
   must be initalized to SCALE_FACTOR by the user's code prior
   to calling any of the Flash API functions. This initalization
   is VITAL to the proper operation of the flash API functions.  
   
   SCALE_FACTOR is defined in Example_Flash2833x_API.h as   
     #define SCALE_FACTOR  1048576.0L*( (200L/CPU_RATE) )
     
   This value is calculated during the compile based on the CPU 
   rate, in nanoseconds, at which the algorithums will be run.
------------------------------------------------------------------*/   
   Flash_CPUScaleFactor = SCALE_FACTOR;	

}

/*------------------------------------------------------------------
  Simple memory copy routine to move code out of flash into SARAM
-----------------------------------------------------------------*/
void Example_MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    { 
       *DestAddr++ = *SourceAddr++;
    }
    return;
}

//Flash写
#pragma CODE_SECTION(User_FlashWrite,"ramfuncs");
///////////////////////////////////////////////////////////////////
//函数名称：User_FlashWrite
//函数功能：Flash写函数
//入口参数：lpu16_buf=写入数据，lu16_Len=写入数据长度
//出口参数：无
//备 	注：无
///////////////////////////////////////////////////////////////////
void User_FlashWrite(Uint16 *lpu16_buf, Uint16 lu16_Len)
{
	Uint16  *Flash_ptr;     // Pointer to a location in flash

	//禁止中断
	DINT;
	
	//擦除Flash(FLASHG区)
	Flash_Erase(SECTORG, &FlashStatus);

	//写FLASHG区
	Flash_ptr = Sector[6].StartAddr;
	Flash_Program(Flash_ptr, lpu16_buf, lu16_Len, &FlashStatus);

	//擦除Flash(FLASHH区)
	Flash_Erase(SECTORH, &FlashStatus);

	//写FLASHH区
	Flash_ptr = Sector[7].StartAddr;
	Flash_Program(Flash_ptr, lpu16_buf, lu16_Len, &FlashStatus);

	//使能中断
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM

	//Enable the watchdog
	EALLOW;
	SysCtrlRegs.WDCR = 0x002F;
	EDIS;
}

///////////////////////////////////////////////////////////////////
//函数名称：User_FlashRead
//函数功能：Flash读函数
//入口参数：lpu16_buf=读数据，lu16_Len=读入数据长度
//出口参数：无
//备 	注：无
///////////////////////////////////////////////////////////////////
Uint16 User_FlashRead(Uint16 *lpu16_buf, Uint16 lu16_Len)
{
	Uint16 lu16_i;
	Uint16  *Flash_ptr;
	Uint32 *lu32_ptr;
	Uint32 lu32_Temp;

	//----------------------读第一区数据---------------------
	//读数据
	Flash_ptr = Sector[6].StartAddr;
	for (lu16_i=0;lu16_i<lu16_Len;lu16_i++)
	{
		lpu16_buf[lu16_i] = *(Flash_ptr++);
	}	
	//计算校验和
	lu32_ptr = (Uint32 *)(lpu16_buf);
	lu32_Temp = 0;
	for (lu16_i=0;lu16_i<(lu16_Len / 2 - 1);lu16_i++)
	{
		lu32_Temp += *(lu32_ptr++);
	}
	//判断数据是否正确
	if ((lu32_Temp - *lu32_ptr))
	{
		//----------------------读第二区数据---------------------
		//读数据
		Flash_ptr = Sector[7].StartAddr;
		for (lu16_i=0;lu16_i<lu16_Len;lu16_i++)
		{
			lpu16_buf[lu16_i] = *(Flash_ptr++);
		}	
		//计算校验和
		lu32_ptr = (Uint32 *)(lpu16_buf);
		lu32_Temp = 0;
		for (lu16_i=0;lu16_i<(lu16_Len / 2 - 1);lu16_i++)
		{
			lu32_Temp += *(lu32_ptr++);
		}
		//判断数据是否正确
		if (lu32_Temp - *lu32_ptr)
		{
			//数据错误返回
			return 0;		
		}			
	}
	
	//数据正确返回
	return 1;	
}	



//===========================================================================
// End of file.
//===========================================================================
