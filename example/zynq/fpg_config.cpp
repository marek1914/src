
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
		PL_REG_WRITE_BIT(ADDR_FPGA_cfg_ctl,0,1);	/*ADDR_FPGA_cfg_ctl BIT 0 д1 */
		PL_REG_WRITE_BIT(ADDR_FPGA_cfg_ctl,0,0);	/*ADDR_FPGA_cfg_ctl BIT 0 д0*/
	}
}


int xilinx_fpga_imageWrite (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uint32_t arg_flag,i,loop,status;
    ulong filesize, fileaddr;
    uint8_t tmpValue;	
    uint8_t * buffer = NULL;	
//////////////////////////////////////////////////////////////////
/////////////��SD���ж���BIT�ļ��ŵ��ڴ���///////////////////////
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
/////////////////////////////////����FPGA0 ///////////////////////
/////////////////////////////////////////////////////////////////   
    /* Start to write Xilinx FPGA image file */
    printf("Start to write Xilinx FPGA image file.\n");
//����FPGA����ģʽ FPGA_config FPGA_config ��ַΪ0x50*4��Ϊ1����FPGA0��д2����FPGA1 
    PL_REG_WRITE(ADDR_FPGA_config,1);
//���� PROGRAM_B ��дFPGA_cfg_ctl ��PL ��ַΪ0x50*4��BIT3Ϊ0
//����ʱ��λд0��дFPGA_cfg_ctl ��PL ��ַΪ0x50*4��BIT0Ϊ0
    PL_REG_WRITE(ADDR_FPGA_cfg_ctl,0x6);
//T�ȴ�һ��ʱ��
    udelay(1*1000*1000);
//	status״̬��0
    status=0;
//��ȡ״̬FPGA0_init_n ��FPGA_cfg_state��BIT0�Ƿ�Ϊ0    
    do
    {
        status = PL_REG_READ_BIT(ADDR_FPGA_cfg_state,0);	/* xilinx_fpga status bit:3 */
    }while(status!=0);		
	
	//����FPGA0_init_n ��FPGA_cfg_state��BIT0Ϊ0������ PROGRAM_B����дFPGA_cfg_ctl ��PL ��ַΪ0x50*4��BIT3Ϊ0
        PL_REG_WRITE(ADDR_FPGA_cfg_ctl,0x6);
//�ȴ�һ��ʱ��
    udelay(1*1000*1000);
//��ȡ״̬FPGA0_init_n ��FPGA_cfg_state��BIT0�Ƿ�Ϊ1 
    do{
        status = PL_REG_READ_BIT(ADDR_FPGA_cfg_state,0);	/* xilinx_fpga status bit:3 */
    }while(status!=1);	
	
//  �����ȴ�һ��ʱ��
    udelay(1*1000*1000);
//����100������ʱ�ӣ�ʱ��λ��������100�Σ�
    xilinx_fpga_generateCLK(100);
//����FPGA0_rw_n��FPGA0_csi_n��FPGA_cfg_ctl��bit1 �� bit2
 PL_REG_WRITE(ADDR_FPGA_cfg_ctl,0x8);
	
//����BYTE��������FPGA��BIT�ļ�,ֱ���ļ�����
    for (loop = 0; loop < filesize; loop ++)
    {   //��ȡByte   
        tmpValue = *(buffer + loop);
           //����һ��byte����/
            PL_REG_WRITE(ADDR_FPGA_config_dat,tmpValue);	
            cpld_xilinx_fpga_generateCLK(1); //����һ��ʱ��
            //�鿴״̬λ��û������
        status=PL_REG_READ_BIT(ADDR_FPGA_cfg_state,0)  
        //������ͣ����������˳�,����������أ�ֱ��ȫ��д��
        if(status==0){
            printf("Xilinx FPGA image write Error!\n");
            break;
        }
    }
    //����500������ʱ��
    cpld_xilinx_fpga_generateCLK(500);
    //�鿴DONE ��FPGA_cfg_state BIT1 ״̬λ�Ƿ�����
    do
    {
        status=PL_REG_READ_BIT(ADDR_FPGA_cfg_state,1);	//��DONE�źţ���FPGA_cfg_state BIT1�� ״̬λ�Ƿ�����
    }while(status!=1);	
   // ������ߣ������������
    free(buffer);
    
    //дFPGA_cfg_ctl ��PL ��ַΪ0x50*4��BIT3ΪE
           PL_REG_WRITE(ADDR_FPGA_cfg_ctl,0xE);
           
     //дFPGA_config ��PL ��ַΪ0x50*4��Ϊ0����������ģʽ��
           PL_REG_WRITE(ADDR_FPGA_config,0x0);
                     
    printf("Xilinx FPGA image file write finished.\n");	
}

