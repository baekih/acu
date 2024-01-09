/*
 * sys.c
 *
 *  Created on: Nov 15, 2023
 *      Author: ihbaek
 */
#include "eco.h"

#ifdef FEATURE_LCD4
#define  SYS_LCD_WIDTH   (480)
#else
#define  SYS_LCD_WIDTH   (800)
#endif

const app_dat g_app_dat_def = {.lcd_bl = 50, .bzr_vol = 0, .rsv = 0x00, .crc32 = 0xc193313d};
app_dat g_app_dat;

void printk(const char* pstr, ...)
{
    char buf[128] = {0};
    va_list args;

    va_start(args, pstr);
    vsprintf(&buf[0], (const char *)pstr, args);
    va_end(args);

    while(HAL_BUSY == HAL_UART_Transmit(&huart1, (uint8_t*)&buf[0], strlen(buf), 1000)) osDelay(1);

}

#ifdef FEATURE_LCD5
void initTS(void)
{
    uint8_t res[TS_RES_LEN] = {0};
    uint16_t x_res = 0, y_res = 0;

    HAL_GPIO_WritePin(TS_RSTn_GPIO_Port, TS_RSTn_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(TS_RSTn_GPIO_Port, TS_RSTn_Pin, GPIO_PIN_SET);
    HAL_Delay(50);
    if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (TS_I2C_ADR)<<1, TS_RES_REG, 1, &res[0], TS_RES_LEN, 1000)) printk("%d error\r\n",__LINE__);

    x_res = ((((uint16_t)res[0])&0x70)<<4) + res[1];
    y_res = ((((uint16_t)res[0])&0x07)<<8) + res[2];

    printf("xres[%d] yres[%d]\n", x_res, y_res);
}

bool getTS(uint16_t* x, uint16_t* y)
{
    uint8_t xy[TS_XY1_LEN] = {0};
    uint16_t x_cur, y_cur;
    static uint16_t x_prv = 0, y_prv = 0;

    if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (TS_I2C_ADR)<<1, TS_XY1_REG, 1, &xy[0], TS_XY1_LEN, 1000))
    {
        printf("%d i2c read xy1 error\n",__LINE__);
        return false;
    }

    *x = x_cur = ((((uint16_t)xy[0])&0x70)<<4) + xy[1];
    *y = y_cur = ((((uint16_t)xy[0])&0x07)<<8) + xy[2];

    if((x_prv != x_cur)||(y_prv != y_cur))
    {
        printf("x[%d] y[%d]\n", x_cur, y_cur);
    }

    x_prv = x_cur;
    y_prv = y_cur;


    return true;
}
#endif

uint32_t doFlashErase(uint32_t sector)
{
    uint32_t SectorError;
    FLASH_EraseInitTypeDef pEraseInit =
    {
        .TypeErase = TYPEERASE_SECTORS,
        .Sector = sector,
        .NbSectors = 1,
        .VoltageRange = VOLTAGE_RANGE_3
    };

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Clear pending flags (if any) */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_ERSERR);

    if (HAL_FLASHEx_Erase(&pEraseInit, &SectorError) != HAL_OK)
    {
       /* Error occurred while page erase */
       return (FLASHIF_ERASE_ERROR);
    }

    return (FLASHIF_OK);
}

uint32_t doFlashWrite(uint32_t addr, uint32_t* pdata, uint32_t len)
{
    uint32_t i = 0;

    for (i = 0; (i < len) && (addr <= (USER_FLASH_END_ADDRESS-4)); i++)
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

#if 0
static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout)
{
    QSPI_CommandTypeDef scmd;
    QSPI_AutoPollingTypeDef scfg;

    /* Configure automatic polling mode to wait for memory ready ------ */
    scmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    scmd.Instruction       = QSPI_CMD_STAT1_REG_RD;
    scmd.AddressMode       = QSPI_ADDRESS_NONE;
    scmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    scmd.DataMode          = QSPI_DATA_1_LINE;
    scmd.NbData            = 1;
    scmd.DummyCycles       = 0;
    scmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
    scmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    scmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;


    scfg.Match           = QSPI_STAT1_REG_WIP_CLR;
    scfg.Mask            = QSPI_STAT1_REG_WIP;
    scfg.MatchMode       = QSPI_MATCH_MODE_AND;
    scfg.StatusBytesSize = 1;
    scfg.Interval        = 0x10;
    scfg.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    if(HAL_QSPI_AutoPolling(&hqspi, &scmd, &scfg, Timeout) != HAL_OK) return QSPI_ERROR;

    return QSPI_OK;
}
#endif
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
    app_dat *papp_dat = (app_dat*)USER_DAT_ADDRESS;
    uint32_t crc32_val = HAL_CRC_Calculate(&hcrc, (uint32_t *)papp_dat, sizeof(app_dat)/sizeof(uint32_t) - 1);

    if(papp_dat->crc32 != crc32_val)
    {
        g_app_dat = g_app_dat_def;
        printf("%s() Flash re-init with def val. lcd_bl[%d] bzr_vol[%d]\n",__func__, g_app_dat.lcd_bl, g_app_dat.bzr_vol);
        doFlashErase(FLASH_SECTOR_7);
        doFlashWrite(USER_DAT_ADDRESS, (uint32_t*)&g_app_dat, sizeof(app_dat)/sizeof(uint32_t));
    }
    else
    {
        g_app_dat = *papp_dat;
    }

    return;
}

void updateFlashData(void)
{
    uint32_t crc32_val = HAL_CRC_Calculate(&hcrc, (uint32_t *)&g_app_dat, sizeof(app_dat)/sizeof(uint32_t) - 1);

    if(g_app_dat.crc32 != crc32_val)
    {
        printf("%s() Flash update lcd_bl[%d] bzr_vol[%d]\n", __func__, g_app_dat.lcd_bl, g_app_dat.bzr_vol);
        g_app_dat.crc32 = crc32_val;
        doFlashErase(FLASH_SECTOR_7);
        doFlashWrite(USER_DAT_ADDRESS, (uint32_t*)&g_app_dat, sizeof(app_dat)/sizeof(uint32_t));
    }

    return;
}

void setLCDTestImage(uint8_t img_sel)
{
    uint16_t *pbuf = (uint16_t*)0xC0000000;

    switch(img_sel)
    {
    case LCD_TST_IMG_CHESS:
        for(uint32_t y=0; y<480; y++)
        {
            for(uint32_t x=0; x<SYS_LCD_WIDTH; x++)
            {
                if(0<=x && x<160)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                }
                else if(160<=x && x<320)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                }
                else if(320<=x && x<480)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                }
#ifdef FEATURE_LCD5
                else if(480<=x && x<640)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                }
                else if(640<=x && x<SYS_LCD_WIDTH)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                }
#endif
            }
        }
        printf("chess\n");
        break;
    case LCD_TST_IMG_WHITE:
        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++) *(pbuf+i) = 0xFFFF;
        printf("white\n");
        break;
    case LCD_TST_IMG_RED:
        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++) *(pbuf+i) = 0x00F8;
        printf("red\n");
        break;
    case LCD_TST_IMG_GREEN:
        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++) *(pbuf+i) = 0xE007;
        printf("green\n");
        break;
    case LCD_TST_IMG_BLUE:
        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++) *(pbuf+i) = 0x1F00;
        printf("blue\n");
        break;
    case LCD_TST_IMG_GRAY:
    {
        uint32_t x, y;

        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++)
        {
            x = i/SYS_LCD_WIDTH;
            y = i%SYS_LCD_WIDTH;

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
#ifdef FEATURE_LCD4
        memcpy((uint32_t*)0xC0000000, &image_compass_480x480[0], 480*480*2);
        printf("compass\n");
#else
        memcpy((uint32_t*)0xC0000000, &image_autopilot_800x480[0], 800*480*2);
        printf("autopilot\n");
#endif
        break;
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//    printf("%s() called...\r\n",__FUNCTION__);
    CAN_RxHeaderTypeDef RxHeader;
    RxProtocol RxPacket;

    /* Get CAN1 RX message */
    if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxPacket.dat) != HAL_OK) Error_Handler();

    RxPacket.canid = RxHeader.ExtId;
    RxPacket.len = RxHeader.DLC;

    for(uint8_t i=0; i < sizeof(RxProtocol); i++)
    {
        if(osOK != osMessageQueuePut(EcoQueueNMEA2KRX1Handle, (uint8_t*)(&RxPacket) + i, 0, 0)) Error_Handler();
    }
}

#ifdef FEATURE_LCD4
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint8_t image_sel = 0;
//    printf("%s() Enter...\n",__FUNCTION__);
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_PREV_GPIO_Port, KEY_PREV_Pin))
    {
        if(g_switch_bank[0] == 0) g_switch_bank[0] = 1;
        else                      g_switch_bank[0] = 0;
    }

    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin))
    {
        setLCDTestImage(image_sel++);
        if(LCD_TST_IMG_DEF < image_sel) image_sel = LCD_TST_IMG_CHESS;
    }
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_SEL_GPIO_Port, KEY_SEL_Pin)) printf("KEY_SEL pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_DN_GPIO_Port, KEY_DN_Pin)) printf("KEY_DN pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_PWR_GPIO_Port, KEY_PWR_Pin)) printf("KEY_PWR pressed\n");
}
#endif
