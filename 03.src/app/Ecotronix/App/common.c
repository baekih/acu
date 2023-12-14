/*
 * common.c
 *
 *  Created on: Mar 30, 2023
 *      Author: ihbaek
 */

#include "common.h"

#define QSPI_MMAP_LEN_MAX                    (0x0FFFFFFF) //256MB
#define QSPI_PAGE_SIZE                       256

#define QSPI_CMD_WR_EN                       0x06
#define QSPI_CMD_ADR32_EN_SET                0xB7
#define QSPI_CMD_ADR32_EN_CLR                0xE9
#define QSPI_CMD_SECTOR_ERASE                0xD8
#define QSPI_CMD_SECTOR_ERASE_ADR32          0x21

#define QSPI_CMD_RST_EN                      0x66
#define QSPI_CMD_RST_START                   0x99

#define QSPI_CMD_PROG_1_1_1                  0x02
#define QSPI_CMD_PROG_1_1_4                  0x32
#define QSPI_CMD_READ_1_1_1                  0x03
#define QSPI_CMD_READ_1_1_1_DMY              0x0B
#define QSPI_CMD_READ_1_1_4_DMY              0x6C
#define QSPI_CMD_READ_1_4_4_DMY              0xEB

#define QSPI_CMD_STAT1_REG_RD                0x05
#define QSPI_STAT1_REG_WIP                   0x01
#define QSPI_STAT1_REG_WIP_CLR               0x00
#define QSPI_STAT1_REG_WIP_SET               0x01
#define QSPI_STAT1_REG_WEL                   0x02
#define QSPI_STAT1_REG_WEL_CLR               0x00
#define QSPI_STAT1_REG_WEL_SET               0x02

#define EN25QH256A_CMD_STAT2_REG_RD          0x09
#define EN25QH256A_STAT2_REG_ADR4            0x10
#define EN25QH256A_STAT2_REG_ADR4_CLR        0x00
#define EN25QH256A_STAT2_REG_ADR4_SET        0x10

#define EN25QH256A_CMD_STAT3_REG_RD          0x95
#define EN25QH256A_CMD_STAT3_REG_WR          0xC0

/* Default dummy clocks cycles */
#define EN25QH256A_REG_1_1_1_DMY_CYCLE       0x20  //8cycle
#define EN25QH256A_SET_1_1_1_DMY_CYCLE       8
#define EN25QH256A_REG_1_1_4_DMY_CYCLE       0x20  //8cycle
#define EN25QH256A_SET_1_1_4_DMY_CYCLE       8
#define EN25QH256A_REG_1_4_4_DMY_CYCLE       0x00  //6cycle
#define EN25QH256A_SET_1_4_4_DMY_CYCLE       6

#define EN25QH256A_SUBSECTOR_ERASE_MAX_TIME  300

/* QSPI Error codes */
#define QSPI_OK            ((uint8_t)0x00)
#define QSPI_ERROR         ((uint8_t)0x01)
#define QSPI_BUSY          ((uint8_t)0x02)
#define QSPI_NOT_SUPPORTED ((uint8_t)0x04)
#define QSPI_SUSPENDED     ((uint8_t)0x08)

void _putchar(char ch)
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART1 and Loop until the end of transmission */
    if(ch == '\n') osMessageQueuePut(EcoQueueUART1Handle, (uint8_t*)"\r", 0, 0);
    osMessageQueuePut(EcoQueueUART1Handle, (uint8_t*)&ch, 0, 0);
}

void EcoQueuePut(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len)
{
    for(uint8_t i=0; i<len; i++)
    {
        osMessageQueuePut(handle, pbuf + i, 0, 0);
    }
}

void EcoQueueWait(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len)
{
    while(osOK != osMessageQueueGet(handle, pbuf, 0, portMAX_DELAY));

    for(uint8_t i = 1; i < len; i++)
    {
        if(osOK != osMessageQueueGet(handle, pbuf + i, 0, 0))
        {
            Error_Handler();
        }
    }
}

portBASE_TYPE IdleTaskHook(void* p)
{
    if ((int)p) //idle task sched out
    {
    }
    else //idle task sched in
    {
    }
    return pdTRUE;
}

/********************************************************/
/*******************  QuadSPI functions *****************/
/********************************************************/
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

void EcoQSPIInit(void)
{
    /* QSPI memory reset */
    if(QSPI_ResetMemory() != QSPI_OK) Error_Handler();

    /* Set the QSPI memory in 4-bytes address mode */
    if(QSPI_EnterFourBytesAddress() != QSPI_OK) Error_Handler();

    /* Configuration of the dummy cycles on QSPI memory side */
    if (QSPI_DummyCyclesCfg() != QSPI_OK) Error_Handler();

    QSPI_EnableMemoryMappedMode();
}

/********************************************************/
/*****************  QuadSPI functions END ***************/
/********************************************************/

