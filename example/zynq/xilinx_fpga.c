/* 
  * Xilinx FPGA Class.
  *
  * Copyright ZTE, Inc. All rights reserved.
  *
  * Author: Li Hongbo
  *
  * Version: V1.00
  *
  * Date: 2008.1
  */


#include <common.h>

#ifdef CONFIG_ZTEBPLT_XILINX_FPGA

/************************************************************************/
/* ** HEADER FILES							                                                 */ 
/************************************************************************/
#include <command.h>
#include <devices.h>
#include <net.h>

#include <asm/io.h>
#include <asm/arch/cpld.h>
#include <asm/arch/physaddrspace.h>
#include <ztebplt/cmd_zteutils.h>
#include <ztebplt/ztebplt_globals.h>
#include <ztebplt/xilinx_fpga.h>


/************************************************************************/
/* ** GENERIC Initialization Routines                 	                                                 */
/************************************************************************/
int drv_xilinx_fpga_init()
{
    device_t xilinx_fpga_dev;
    int rc;

    debug("[drv_xilinx_fpga_init ... ...]\n");
    
    /* Device initialization */
    memset (&xilinx_fpga_dev, 0, sizeof (xilinx_fpga_dev));

    strcpy (xilinx_fpga_dev.name, "xilinx_fpga");
    xilinx_fpga_dev.ext   = PHYS_IO;                   /* Extensions */
    xilinx_fpga_dev.flags = DEV_FLAGS_SYSTEM; /* System only */
    xilinx_fpga_dev.priv =  NULL;                        /* Private data */

    rc = device_register (&xilinx_fpga_dev);
    
    return (rc == 0) ? 1 : rc;
}


/************************************************************************/
/* ** HELPER Routines                 	                                                                      */
/************************************************************************/
/* Generate Xilinx FPGA clock */
void cpld_xilinx_fpga_generateCLK(uint32_t clkNum)
{
	uint32_t loop;

	for (loop = 0; loop < clkNum; loop ++)
	{
		cpld_set_regbit(CPLD_XILINX_FPGA_REG,1,1);	/* xilinx_fpga clock bit:1 */
		cpld_set_regbit(CPLD_XILINX_FPGA_REG,1,0);	/* xilinx_fpga clock bit:1 */
	}
}

/************************************************************************/
/* ** CMD Routines                     	                                                                      */
/************************************************************************/
/* Write Xilinx FPGA image to FPGA chip */
int xilinx_fpga_imageWrite (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uint32_t arg_flag,i,loop,status;
    ulong filesize, fileaddr;
    uint8_t tmpValue;	
    uint8_t * buffer = NULL;	

    if(argc == 1)
    {
        printf("Usage:%s", cmdtp->usage);
        return(1);
    }

    arg_flag = simple_strtoul(argv[1], NULL, 16);    
    if(arg_flag==1)
    {
    	/* Get Xilinx FPGA file from bootfile */
        filesize =  simple_strtoul( getenv("filesize"), NULL, 16);        
        printf("Xilinx FPGA file size:0x%x\n",filesize);        
        
        buffer = (uint8_t *)malloc(filesize);
        if (!buffer)
        {
            printf("[Error]: Malloc of Temp. Buffer for fpga file failed.\n");
            return (1);
        }

        memcpy_fromio(buffer, load_addr, filesize);
        debug("buffered from 0x%x to 0x%x (sizes0x%x).\n", load_addr, buffer, filesize);      
        printf("Write ram succeed!\n");
    }
    else if(argc == 3)
    {
    	/* Get xilinx fpga file from flash */
        if( get_ffile_info(argv[2], &filesize, &fileaddr) != 0)
        {
            printf("Save the file(%s) to flash before read it out! \n", argv[2]);
            return(1);
        }        
        printf("Xilinx fpga file size:0x%x\n",filesize);

        buffer = (uint8_t *)malloc(filesize);
        if (!buffer)
        {
            printf("[Error]: Malloc of Temp. Buffer for fpga file failed.\n");
            return (1);
        }
        
        printf("Now begin to update ram (dst:0x%x src:0x%x size:%d)\n", buffer, fileaddr, filesize);
        memcpy_fromio(buffer, fileaddr , filesize);
        debug("buffered from 0x%x to 0x%x (sizes0x%x).\n", load_addr, buffer, filesize);           
        printf("Write ram succeed!\n");
    }
    else
    {
        printf("Usage:%s", cmdtp->usage);
        return(1);    
    }

    /* Start to write Xilinx FPGA image file */
    printf("Start to write Xilinx FPGA image file.\n");

    cpld_set_regbit(CPLD_XILINX_FPGA_REG,1,0);	/* xilinx_fpga clock bit:1 */

    cpld_set_regbit(CPLD_XILINX_FPGA_REG,2,0);	/* xilinx_fpga program bit:2 */

    udelay(1*1000*1000);
	
    status=0;
    do
    {
        status = cpld_get_regbit(CPLD_XILINX_FPGA_REG,3);	/* xilinx_fpga status bit:3 */
    }while(status!=0);		
	
    cpld_set_regbit(CPLD_XILINX_FPGA_REG,2,1);	/* xilinx_fpga program bit:2 */

    udelay(1*1000*1000);
	
    do{
        status=cpld_get_regbit(CPLD_XILINX_FPGA_REG,3);	/* xilinx_fpga status bit:3 */
    }while(status!=1);	
	
    udelay(1*1000*1000);

    cpld_xilinx_fpga_generateCLK(100);
	
    for (loop = 0; loop < filesize; loop ++)
    {      
        tmpValue = *(buffer + loop);
        for(i=0;i<8;i++)
        {
            cpld_set_regbit(CPLD_XILINX_FPGA_REG,0,(tmpValue>>i)&0x1);	/* xilinx_fpga data bit:0 */
            cpld_xilinx_fpga_generateCLK(1);
        }
        status=cpld_get_regbit(CPLD_XILINX_FPGA_REG,3);
        if(status==0){
            printf("Xilinx FPGA image write Error!\n");
            break;
        }
    }
    
    cpld_xilinx_fpga_generateCLK(500);
    do
    {
        status=cpld_get_regbit(CPLD_XILINX_FPGA_REG,4);	/* xilinx_fpga done bit:4 */
    }while(status!=1);	

    free(buffer);

    printf("Xilinx FPGA image file write finished.\n");	
}

U_BOOT_CMD(
	xilinx_fpgaImageWrite,	3,	1,	xilinx_fpga_imageWrite,
	"xilinx_fpgaImageWrite     -  Write Xilinx FPGA image to FPGA chip  :D \n",
	NULL
);


/*
	register_shell_cmd("xilinx_fpga_set_reg", (void *)cmd_xilinx_fpga_set_reg);
	register_shell_cmd("xilinx_fpga_addrRead", (void *)xilinx_fpga_addrRead);
	register_shell_cmd("sync_cpu_xilinx_fpga", (void *)sync_cpu_xilinx_fpga);

*/

/*----------------------------------------------------------------------*/


#endif /* CONFIG_ZTEFW_XILINX_FPGA */





