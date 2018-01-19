
#define ADDR_FPGA_config      0x050
#define ADDR_FPGA_cfg_ctl     0x051
#define ADDR_FPGA_cfg_state   0x052
#define ADDR_FPGA_config_dat  0x053
#define ADDR_FPGA_rw_ctrl     0x054
#define ADDR_FPGA_rw_addr     0x055
#define ADDR_FPGA_wr_data     0x056
#define ADDR_FPGA_rd_data     0x057

/* Generate Xilinx FPGA clock */
void cpld_xilinx_fpga_generateCLK(uint32_t clkNum)
{
	uint32_t loop;

	for (loop = 0; loop < clkNum; loop ++)
	{
		PL_REG_WRITE_BIT(ADDR_FPGA_cfg_ctl,0,1);	/*ADDR_FPGA_cfg_ctl BIT 0 写1 */
		PL_REG_WRITE_BIT(ADDR_FPGA_cfg_ctl,0,0);	/*ADDR_FPGA_cfg_ctl BIT 0 写0*/
	}
}


int xilinx_fpga_imageWrite (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uint32_t arg_flag,i,loop,status;
    ulong filesize, fileaddr;
    uint8_t tmpValue;	
    uint8_t * buffer = NULL;	
//////////////////////////////////////////////////////////////////
/////////////从SD卡中读出BIT文件放到内存中///////////////////////
/////////////////////////////////////////////////////////////////
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
 //////////////////////////////////////////////////////////////////
/////////////////////////////////加载FPGA0 ///////////////////////
/////////////////////////////////////////////////////////////////   
    /* Start to write Xilinx FPGA image file */
    printf("Start to write Xilinx FPGA image file.\n");
//启动FPGA加载模式 FPGA_config FPGA_config 地址为0x50*4）为1加载FPGA0，写2加载FPGA1 
    PL_REG_WRITE(ADDR_FPGA_config,1);
//拉低 PROGRAM_B ：写FPGA_cfg_ctl （PL 地址为0x50*4）BIT3为0
//并将时钟位写0：写FPGA_cfg_ctl （PL 地址为0x50*4）BIT0为0
    PL_REG_WRITE(ADDR_FPGA_cfg_ctl,0x6);
//T等待一段时间
    udelay(1*1000*1000);
//	status状态清0
    status=0;
//读取状态FPGA0_init_n 即FPGA_cfg_state的BIT0是否为0    
    do
    {
        status = PL_REG_READ_BIT(ADDR_FPGA_cfg_state,0);	/* xilinx_fpga status bit:3 */
    }while(status!=0);		
	
	//读到FPGA0_init_n 即FPGA_cfg_state的BIT0为0后，拉高 PROGRAM_B，即写FPGA_cfg_ctl （PL 地址为0x50*4）BIT3为0
        PL_REG_WRITE(ADDR_FPGA_cfg_ctl,0x6);
//等待一段时间
    udelay(1*1000*1000);
//读取状态FPGA0_init_n 即FPGA_cfg_state的BIT0是否为1 
    do{
        status = PL_REG_READ_BIT(ADDR_FPGA_cfg_state,0);	/* xilinx_fpga status bit:3 */
    }while(status!=1);	
	
//  读到等待一段时间
    udelay(1*1000*1000);
//生成100个下载时钟（时钟位拉高拉低100次）
    xilinx_fpga_generateCLK(100);
//拉低FPGA0_rw_n和FPGA0_csi_n即FPGA_cfg_ctl的bit1 和 bit2
 PL_REG_WRITE(ADDR_FPGA_cfg_ctl,0x8);
	
//按照BYTE依次下载FPGA的BIT文件,直至文件读完
    for (loop = 0; loop < filesize; loop ++)
    {   //读取Byte   
        tmpValue = *(buffer + loop);
           //加载一个byte数据/
            PL_REG_WRITE(ADDR_FPGA_config_dat,tmpValue);	
            cpld_xilinx_fpga_generateCLK(1); //生成一个时钟
            //查看状态位有没有拉低
        status=PL_REG_READ_BIT(ADDR_FPGA_cfg_state,0)  
        //如果拉低，表明错误，退出,否则继续下载，直到全部写完
        if(status==0){
            printf("Xilinx FPGA image write Error!\n");
            break;
        }
    }
    //生成500个下载时钟
    cpld_xilinx_fpga_generateCLK(500);
    //查看DONE 即FPGA_cfg_state BIT1 状态位是否拉高
    do
    {
        status=PL_REG_READ_BIT(ADDR_FPGA_cfg_state,1);	//读DONE信号（即FPGA_cfg_state BIT1） 状态位是否拉高
    }while(status!=1);	
   // 如果拉高，表明加载完成
    free(buffer);
    
    //写FPGA_cfg_ctl （PL 地址为0x50*4）BIT3为E
           PL_REG_WRITE(ADDR_FPGA_cfg_ctl,0xE);
           
     //写FPGA_config （PL 地址为0x50*4）为0（正常工作模式）
           PL_REG_WRITE(ADDR_FPGA_config,0x0);
                     
    printf("Xilinx FPGA image file write finished.\n");	
}

