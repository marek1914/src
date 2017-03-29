void write_flag(void)
{
/* last 2k page */
	uint32_t last_page = 0x0803f7ff;
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, last_page, 0x55aa55aa);
    HAL_FLASH_Lock();
}
