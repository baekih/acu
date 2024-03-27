/*
 * sys.c
 *
 *  Created on: Nov 15, 2023
 *      Author: ihbaek
 */
#include "eco.h"

const common_dat g_common_dat_def = {.lcd_bl = 50, .bzr_vol = 0, .jmp_adr = APP_START_ADDR, .rsv1 = 0, .rsv2 = {0, 0}};
common_dat g_common_dat;

uint8_t g_lcd_img_idx = 0;

key_stat g_key_stat[KEY_MAX];
uint32_t sys_lcd_width;
uint8_t g_ts_i2c_adr = 0xFF;
uint8_t g_board_id = BOARD_ID_INVAL;

void printk(const char* pstr, ...)
{
    char buf[128] = {0};
    va_list args;

    va_start(args, pstr);
    vsprintf(&buf[0], (const char *)pstr, args);
    va_end(args);

    while(HAL_BUSY == HAL_UART_Transmit(&huart1, (uint8_t*)&buf[0], strlen(buf), 1000)) osDelay(1);

}

void initTouchSensor(void)
{
    uint8_t res[4] = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(TS_RSTn_GPIO_Port, TS_RSTn_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(TS_INT_GPIO_Port, TS_INT_Pin, GPIO_PIN_RESET);
    HAL_Delay(1); // >100us after TS_INT set to low.
    HAL_GPIO_WritePin(TS_RSTn_GPIO_Port, TS_RSTn_Pin, GPIO_PIN_SET);
    HAL_Delay(60);

    GPIO_InitStruct.Pin = TS_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TS_INT_GPIO_Port, &GPIO_InitStruct);

    if(HAL_OK == HAL_I2C_Mem_Read(&hi2c1, (TS_ST1633_I2C_ADR)<<1, TS_ST1633_RES_REG, 1, &res[0], TS_ST1633_RES_LEN, 10))
    {
        g_ts_i2c_adr = TS_ST1633_I2C_ADR;
        g_board_id = BOARD_ID_DIN15;
    }
    else if(HAL_OK == HAL_I2C_Mem_Read(&hi2c1, (TS_GT911_I2C_ADR)<<1, TS_GT911_RES_REG, 2, &res[0], TS_GT911_RES_LEN, 10))
    {
        g_ts_i2c_adr = TS_GT911_I2C_ADR;
        g_board_id = BOARD_ID_DIN15;
    }
    else
    {
        g_ts_i2c_adr = TS_INVAL_I2C_ADR;
        g_board_id = BOARD_ID_DIN10;
        printk("no TS detected.\r\n");
        return;
    }
}

bool getTouchSensor(uint16_t* x, uint16_t* y)
{
    uint8_t xy[4] = {0};
    uint8_t stat = 0;

    switch(g_ts_i2c_adr)
    {
    case TS_ST1633_I2C_ADR:
        if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (g_ts_i2c_adr)<<1, TS_ST1633_XY1_REG, 1, &xy[0], TS_ST1633_XY1_LEN, 1000))
        {
            printf("%s():%d ST1633 i2c read xy1 error\n",__FUNCTION__, __LINE__);
            return false;
        }

        *x = ((((uint16_t)xy[0])&0x70)<<4) + xy[1];
        *y = ((((uint16_t)xy[0])&0x07)<<8) + xy[2];

        break;
    case TS_GT911_I2C_ADR:
        if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (g_ts_i2c_adr)<<1, TS_GT911_STAT_REG, 2, &stat, TS_GT911_STAT_LEN, 1000))
        {
            printf("%s():%d GT911 i2c read stat error\n",__FUNCTION__, __LINE__);
            return false;
        }

        if(!(stat&TS_GT911_STAT_BUF_EN_MSK))
        {
            return false;
        }

        if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (g_ts_i2c_adr)<<1, TS_GT911_XY1_REG, 2, &xy[0], TS_GT911_XY1_LEN, 1000))
        {
            printf("%s():%d GT911 i2c read xy error\n",__FUNCTION__, __LINE__);
            return false;
        }

        *x = (xy[1]<<8) + xy[0];
        *y = (xy[3]<<8) + xy[2];
        stat = 0;

        if(HAL_OK != HAL_I2C_Mem_Write(&hi2c1, (g_ts_i2c_adr)<<1, TS_GT911_STAT_REG, 2, &stat, TS_GT911_STAT_LEN, 1000))
        {
            printf("%s():%d GT911 i2c write stat error\n",__FUNCTION__, __LINE__);
            return false;
        }
//        printf("%s():%d GT911 xy[%03d:%03d] OK\n",__FUNCTION__, __LINE__, *x, *y);

        break;
    default:
        return false;
        break;
    }

    return true;
}

uint32_t eraseFlash(uint32_t addr)
{
    uint32_t SectorError;

    FLASH_EraseInitTypeDef EraseInit =
    {
        .TypeErase = TYPEERASE_SECTORS,
        .NbSectors = 1,
        .VoltageRange = VOLTAGE_RANGE_3
    };

    if     ((addr < ADDR_FLASH_SECTOR_1) && (addr >= ADDR_FLASH_SECTOR_0)) EraseInit.Sector = FLASH_SECTOR_0;
    else if((addr < ADDR_FLASH_SECTOR_2) && (addr >= ADDR_FLASH_SECTOR_1)) EraseInit.Sector = FLASH_SECTOR_1;
    else if((addr < ADDR_FLASH_SECTOR_3) && (addr >= ADDR_FLASH_SECTOR_2)) EraseInit.Sector = FLASH_SECTOR_2;
    else if((addr < ADDR_FLASH_SECTOR_4) && (addr >= ADDR_FLASH_SECTOR_3)) EraseInit.Sector = FLASH_SECTOR_3;
    else if((addr < ADDR_FLASH_SECTOR_5) && (addr >= ADDR_FLASH_SECTOR_4)) EraseInit.Sector = FLASH_SECTOR_4;
    else if((addr < ADDR_FLASH_SECTOR_6) && (addr >= ADDR_FLASH_SECTOR_5)) EraseInit.Sector = FLASH_SECTOR_5;
    else if((addr < ADDR_FLASH_SECTOR_7) && (addr >= ADDR_FLASH_SECTOR_6)) EraseInit.Sector = FLASH_SECTOR_6;
    else if(addr >= ADDR_FLASH_SECTOR_7) EraseInit.Sector = FLASH_SECTOR_7;

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Clear pending flags (if any) */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_ERSERR);

    if (HAL_FLASHEx_Erase(&EraseInit, &SectorError) != HAL_OK)
    {
       /* Error occurred while page erase */
       return (FLASHIF_ERASE_ERROR);
    }

    return (FLASHIF_OK);
}

uint32_t writeFlash(uint32_t addr, uint32_t* pdata, uint32_t len)
{
    uint32_t i = 0;

    if(!((FLASH_DATA_ADDR <= addr)&&(addr < (FLASH_DATA_ADDR + 32*1024))))
    {
        return (FLASHIF_WRITING_ERROR);
    }

    for (i = 0; i < len; i++)
    {
      /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
         be done by word */
      if (HAL_FLASH_Program(TYPEPROGRAM_WORD, addr, *(uint32_t*)(pdata+i)) == HAL_OK)
      {
       /* Check the written value */
        if (*(uint32_t*)addr != *(uint32_t*)(pdata+i))
        {
          /* Flash content doesn't match SRAM content */
          return(FLASHIF_WRITINGCTRL_ERROR);
        }
        /* Increment FLASH destination address */
        addr += 4;
      }
      else
      {
        /* Error occurred while writing data in Flash memory */
        return (FLASHIF_WRITING_ERROR);
      }
    }

    return (FLASHIF_OK);
}

static uint8_t QSPI_WriteEnable(void)
{
    QSPI_CommandTypeDef scmd;
    QSPI_AutoPollingTypeDef scfg;

    /* Enable write operations ------------------------------------------ */
    scmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    scmd.Instruction       = QSPI_CMD_WR_EN;
    scmd.AddressMode       = QSPI_ADDRESS_NONE;
    scmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    scmd.DataMode          = QSPI_DATA_NONE;
    scmd.DummyCycles       = 0;
    scmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
    scmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    scmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;

    /* Configure automatic polling mode to wait for write enabling ---- */
    scmd.Instruction    = QSPI_CMD_STAT1_REG_RD;
    scmd.DataMode       = QSPI_DATA_1_LINE  ;
    scmd.NbData         = 1;

    scfg.Match           = QSPI_STAT1_REG_WEL;
    scfg.Mask            = QSPI_STAT1_REG_WEL_SET;
    scfg.MatchMode       = QSPI_MATCH_MODE_AND;
    scfg.StatusBytesSize = 1;
    scfg.Interval        = 0x10;
    scfg.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    if(HAL_QSPI_AutoPolling(&hqspi, &scmd, &scfg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;

    return QSPI_OK;
}

static uint8_t QSPI_ResetMemory(void)
{
    QSPI_CommandTypeDef scmd;
    QSPI_AutoPollingTypeDef scfg;

    // EN25QH256A reset enable.
    scmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    scmd.Instruction       = QSPI_CMD_RST_EN;
    scmd.AddressMode       = QSPI_ADDRESS_NONE;
    scmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    scmd.DataMode          = QSPI_DATA_NONE;
    scmd.DummyCycles       = 0;
    scmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
    scmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    scmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;

    // EN25QH256A reset start.
    scmd.Instruction       = QSPI_CMD_RST_START;
    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;

    // EN25QH256A reset done check.
    scmd.Instruction    = QSPI_CMD_STAT1_REG_RD;
    scmd.DataMode       = QSPI_DATA_1_LINE;
    scmd.NbData            = 1;

    scfg.Match           = QSPI_STAT1_REG_WIP;
    scfg.Mask            = QSPI_STAT1_REG_WIP_CLR;
    scfg.MatchMode       = QSPI_MATCH_MODE_AND;
    scfg.StatusBytesSize = 1;
    scfg.Interval        = 0x10;
    scfg.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    if(HAL_QSPI_AutoPolling(&hqspi, &scmd, &scfg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;

    return QSPI_OK;
}

static uint8_t QSPI_EnterFourBytesAddress(void)
{
    QSPI_CommandTypeDef scmd;
    QSPI_AutoPollingTypeDef scfg;
    /* Enable 32-Bit address mode */
    scmd.Instruction       = QSPI_CMD_ADR32_EN_SET;
    scmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    scmd.AddressMode       = QSPI_ADDRESS_NONE;
    scmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    scmd.DummyCycles       = 0;
    scmd.DataMode          = QSPI_DATA_NONE;
    scmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
    scmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    scmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &scmd, 0x10);

    scmd.Instruction       = EN25QH256A_CMD_STAT2_REG_RD;
    scmd.DataMode          = QSPI_DATA_1_LINE  ;
    scmd.NbData            = 1;

    scfg.Match           = EN25QH256A_STAT2_REG_ADR4;
    scfg.Mask            = EN25QH256A_STAT2_REG_ADR4_SET;
    scfg.MatchMode       = QSPI_MATCH_MODE_AND;
    scfg.StatusBytesSize = 1;
    scfg.Interval        = 0x10;
    scfg.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    if(HAL_QSPI_AutoPolling(&hqspi, &scmd, &scfg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;

    return QSPI_OK;
}

static uint8_t QSPI_DummyCyclesCfg(void)
{
    QSPI_CommandTypeDef scmd;
    uint8_t reg;

    /* Read Volatile Configuration register --------------------------- */
    scmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    scmd.Instruction       = EN25QH256A_CMD_STAT3_REG_RD;
    scmd.AddressMode       = QSPI_ADDRESS_NONE;
    scmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    scmd.DataMode          = QSPI_DATA_1_LINE;
    scmd.DummyCycles       = 0;
    scmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
    scmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    scmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    scmd.NbData            = 1;

    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;
    if(HAL_QSPI_Receive(&hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;

    /* Enable write operations ---------------------------------------- */
    if(!QSPI_WriteEnable()) return 0;

    /* Write Volatile Configuration register (with new dummy cycles) -- */

    reg &= ~(EN25QH256A_REG_1_4_4_DMY_CYCLE);
    reg |= EN25QH256A_REG_1_4_4_DMY_CYCLE;

    scmd.Instruction = EN25QH256A_CMD_STAT3_REG_WR;

    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;
    if(HAL_QSPI_Transmit(&hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return QSPI_ERROR;

    return QSPI_OK;
}

uint8_t QSPI_EnableMemoryMappedMode(void)
{
    QSPI_CommandTypeDef scmd;
    QSPI_MemoryMappedTypeDef smmapcfg = {.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE};

    scmd.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    scmd.Instruction        = QSPI_CMD_READ_1_4_4_DMY;
    scmd.AddressMode        = QSPI_ADDRESS_4_LINES;
    scmd.AddressSize        = QSPI_ADDRESS_32_BITS;
    scmd.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
    scmd.DataMode           = QSPI_DATA_4_LINES;
    scmd.DummyCycles        = EN25QH256A_SET_1_4_4_DMY_CYCLE;
    scmd.DdrMode            = QSPI_DDR_MODE_DISABLE;
    scmd.DdrHoldHalfCycle   = QSPI_DDR_HHC_ANALOG_DELAY;
    scmd.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_MemoryMapped(&hqspi, &scmd, &smmapcfg) != HAL_OK) return QSPI_ERROR;

    return QSPI_OK;
}

void InitQSPI(void)
{
    /* QSPI memory reset */
    if(QSPI_ResetMemory() != QSPI_OK) Error_Handler();

    /* Set the QSPI memory in 4-bytes address mode */
    if(QSPI_EnterFourBytesAddress() != QSPI_OK) Error_Handler();

    /* Configuration of the dummy cycles on QSPI memory side */
    if (QSPI_DummyCyclesCfg() != QSPI_OK) Error_Handler();

    QSPI_EnableMemoryMappedMode();
}

bool getKeyPending(uint8_t idx)
{
    bool ret = g_key_stat[idx].cur;

    g_key_stat[idx].cur = false;

    return ret;
}

void setBuzzer(uint8_t bzr_vol)
{
    static uint8_t bzr_vol_prev = 0;

    TIM_OC_InitTypeDef sConfigOC = {TIM_OCMODE_PWM1, 0, TIM_OCPOLARITY_HIGH, TIM_OCFAST_DISABLE, 0, 0};

    if(100 < bzr_vol) bzr_vol = 100;
    if(bzr_vol_prev == bzr_vol) return;
    bzr_vol_prev = bzr_vol;

    sConfigOC.Pulse = (125*bzr_vol)/100;
    printf("set bzr_vol[%d] Pulse[%d]\n", bzr_vol, sConfigOC.Pulse);

    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

    return;
}

void setLCDBL(uint8_t lcd_bl)
{
    static uint8_t lcd_bl_prev = 0;
    TIM_OC_InitTypeDef sConfigOC = {TIM_OCMODE_PWM1, 0, TIM_OCPOLARITY_HIGH, TIM_OCFAST_DISABLE, 0, 0};

    if(100 < lcd_bl) lcd_bl = 100;
    if(lcd_bl_prev == lcd_bl) return;
    lcd_bl_prev = lcd_bl;

    sConfigOC.Pulse = (100*lcd_bl)/100;
    printf("set lcd_bl[%d] Pulse[%d]\n", lcd_bl, sConfigOC.Pulse);

    HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
    return;
}

void initFlashData(void)
{
    common_dat *pcommon_dat = (common_dat*)FLASH_DATA_ADDR;
    uint32_t crc32 = HAL_CRC_Calculate(&hcrc, (uint32_t *)pcommon_dat, sizeof(common_dat)/sizeof(uint32_t) - 1);

    // if no common data exist or invalid value, reset common data to default value.
    if(pcommon_dat->crc32 != crc32)
    {
        g_common_dat = g_common_dat_def;
        g_common_dat.crc32 = crc32;
        printf("%s() Flash re-init with def val. lcd_bl[%d] bzr_vol[%d]\n",__func__, g_common_dat.lcd_bl, g_common_dat.bzr_vol);
        eraseFlash(FLASH_DATA_ADDR);
        writeFlash(FLASH_DATA_ADDR, (uint32_t*)&g_common_dat, sizeof(common_dat)/sizeof(uint32_t));
    }
    else
    {
        g_common_dat = *pcommon_dat;
    }

    printf("Init common_dat[%d:%d:0x%08x:0x%08x]\n",
           g_common_dat.bzr_vol,
           g_common_dat.lcd_bl,
           g_common_dat.jmp_adr,
           g_common_dat.crc32);

    return;
}

void updateFlashData(void)
{
    uint32_t crc32 = HAL_CRC_Calculate(&hcrc, (uint32_t *)&g_common_dat, sizeof(common_dat)/sizeof(uint32_t) - 1);

    if(g_common_dat.crc32 != crc32)
    {
        printf("%s() Flash update lcd_bl[%d] bzr_vol[%d]\n", __func__, g_common_dat.lcd_bl, g_common_dat.bzr_vol);
        g_common_dat.crc32 = crc32;
        eraseFlash(FLASH_DATA_ADDR);
        writeFlash(FLASH_DATA_ADDR, (uint32_t*)&g_common_dat, sizeof(common_dat)/sizeof(uint32_t));
    }

    return;
}

void setLCDTestImage(uint8_t img_sel)
{
    uint16_t *pbuf = (uint16_t*)0xC0000000;

    sys_lcd_width = hltdc.Init.AccumulatedActiveW - hltdc.Init.AccumulatedHBP;

    switch(img_sel)
    {
    case LCD_TST_IMG_CHESS:
        for(uint32_t y=0; y<480; y++)
        {
            for(uint32_t x=0; x<sys_lcd_width; x++)
            {
                if(0<=x && x<160)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*sys_lcd_width) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*sys_lcd_width) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*sys_lcd_width) = 0xFFFF;
                    }
                }
                else if(160<=x && x<320)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*sys_lcd_width) = 0x0000;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*sys_lcd_width) = 0xFFFF;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*sys_lcd_width) = 0x0000;
                    }
                }
                else if(320<=x && x<480)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*sys_lcd_width) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*sys_lcd_width) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*sys_lcd_width) = 0xFFFF;
                    }
                }

                if(sys_lcd_width == 800)
                {
                    if(480<=x && x<640)
                    {
                        if(0<=y && y<160)
                        {
                            *(pbuf+x+y*sys_lcd_width) = 0x0000;
                        }
                        else if(160<=y && y<320)
                        {
                            *(pbuf+x+y*sys_lcd_width) = 0xFFFF;
                        }
                        else if(320<=y && y<480)
                        {
                            *(pbuf+x+y*sys_lcd_width) = 0x0000;
                        }
                    }
                    else if(640<=x && x<sys_lcd_width)
                    {
                        if(0<=y && y<160)
                        {
                            *(pbuf+x+y*sys_lcd_width) = 0xFFFF;
                        }
                        else if(160<=y && y<320)
                        {
                            *(pbuf+x+y*sys_lcd_width) = 0x0000;
                        }
                        else if(320<=y && y<480)
                        {
                            *(pbuf+x+y*sys_lcd_width) = 0xFFFF;
                        }
                    }
                }
            }
        }
        printf("chess\n");
        break;
    case LCD_TST_IMG_WHITE:
        for(uint32_t i=0; i<(sys_lcd_width*480); i++) *(pbuf+i) = 0xFFFF;
        printf("white\n");
        break;
    case LCD_TST_IMG_RED:
        for(uint32_t i=0; i<(sys_lcd_width*480); i++) *(pbuf+i) = 0x00F8;
        printf("red\n");
        break;
    case LCD_TST_IMG_GREEN:
        for(uint32_t i=0; i<(sys_lcd_width*480); i++) *(pbuf+i) = 0xE007;
        printf("green\n");
        break;
    case LCD_TST_IMG_BLUE:
        for(uint32_t i=0; i<(sys_lcd_width*480); i++) *(pbuf+i) = 0x1F00;
        printf("blue\n");
        break;
    case LCD_TST_IMG_GRAY:
    {
        uint32_t x, y;

        for(uint32_t i=0; i<(sys_lcd_width*480); i++)
        {
            x = i/sys_lcd_width;
            y = i%sys_lcd_width;

            if((x&0x00000001)==0)
            {
                if((y&0x00000001)==0) *(pbuf+i) = 0x0000;
                else                  *(pbuf+i) = 0xFFFF;
            }
            else
            {
                if((y&0x00000001)==0) *(pbuf+i) = 0xFFFF;
                else                  *(pbuf+i) = 0x0000;
            }
        }
        printf("gray\n");
        break;
    }
    case LCD_TST_IMG_DEF:
        if(g_board_id == BOARD_ID_DIN15)
        {
            memcpy((uint32_t*)0xC0000000, &image_autopilot_800x480[0], 800*480*2);
            printf("autopilot\n");
        }
        else
        {
//            memcpy((uint32_t*)0xC0000000, &image_compass_480x480[0], 480*480*2);
            printf("compass\n");
        }

        break;
    }
}

int32_t opSwitchBankControl(uint64_t *prxdat64)
{
    g_common_dat.bzr_vol = (uint8_t)(*prxdat64 & 0xFF);

    if     ((*prxdat64 & SW_KEY_UP_MASK) != 0)
    {
        g_lcd_img_idx == 6 ? g_lcd_img_idx = 0 : g_lcd_img_idx++;
        *prxdat64 &= (~(SW_KEY_UP_MASK));
    }
    else if((*prxdat64 & SW_KEY_DN_MASK) != 0)
    {
        g_lcd_img_idx == 0 ? g_lcd_img_idx = 6 : g_lcd_img_idx--;
        *prxdat64 &= (~(SW_KEY_DN_MASK));
    }

    return 0;
}

int32_t opLCDBrightness(uint8_t lcd_bl)
{
    g_common_dat.lcd_bl = lcd_bl;

    return 0;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//    printf("%s() called...\r\n",__FUNCTION__);
    CAN_RxHeaderTypeDef RxHeader;
    RxProtocol RxPacket;

    /* Get CAN1 RX message */
    if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxPacket.dat) != HAL_OK) Error_Handler();

#if defined (ECO_BOOT2)
    RxPacket.canid = RxHeader.ExtId;
    RxPacket.len = RxHeader.DLC;

    for(uint8_t i=0; i < sizeof(RxProtocol); i++)
    {
        if(osOK != osMessageQueuePut(EcoQueueNMEA2KRX1Handle, (uint8_t*)(&RxPacket) + i, 0, 0)) Error_Handler();
    }
#else
    g_RxCan[rxCanLastIndex].canid = RxHeader.ExtId;
    g_RxCan[rxCanLastIndex].len = RxHeader.DLC;
    memcpy(g_RxCan[rxCanLastIndex].dat, RxPacket.dat, sizeof(RxPacket.dat));

    rxCanLastIndex++;
    rxCanLastIndex %= CAN_RX_BUF_MAX;

    if(rxCanLastIndex == rxCanFirstIndex)
    {
        rxCanFirstIndex++;
        rxCanFirstIndex %= CAN_RX_BUF_MAX;
    }
#endif
}

#if !defined (ECO_BOOT2)
void CAN1_SendFrame(uint32_t rawCanId,  uint8_t *buf, uint8_t len)
{
    uint16_t count = 100;

    while(HAL_CAN_IsTxMessagePending(&hcan1, g_TxCan[txCanBufferCount].TxMailbox) == 1)
     {
        osDelay(1);

       if(count-- <= 0)
       {
         printf("HAL_CAN_IsTxMessagePending over 100ms !!\r\n");
         break;
       }
     }

#if 0
    for(uint8_t i = 0; i < 8; i++) printf("%02X ", buf[i]);
    printf("\n");
#endif

    g_TxCan[txCanBufferCount].TxHeader.ExtId = rawCanId;
    g_TxCan[txCanBufferCount].TxHeader.IDE = CAN_ID_EXT;
    g_TxCan[txCanBufferCount].TxHeader.DLC = len;

    memcpy(g_TxCan[txCanBufferCount].TxData, buf, len);

    if(HAL_CAN_AddTxMessage(&hcan1, &g_TxCan[txCanBufferCount].TxHeader,
            g_TxCan[txCanBufferCount].TxData, &g_TxCan[txCanBufferCount].TxMailbox) != HAL_OK)
    {
        printf(" Error HAL_CAN_AddTxMessage hcan1 !!\r\n");
        osDelay(1);
    }
}
#endif
