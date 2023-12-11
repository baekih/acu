/*
 * Loader_Src.c
 *
 *  Created on: Dec 5, 2023
 *      Author: ihbaek
 */

#include <stdio.h>
#include <string.h>

#include "gpio.h"
#include "quadspi.h"
#include "usart.h"

#include "Loader_Src.h"

char uart_buf[128];

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    return HAL_OK;
}

void HAL_Delay(__IO uint32_t Delay)
{
    for(int i=0; i<(Delay*216000); i++);
}

int QSPI_AutoPollingMemReady(uint32_t Timeout)
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

    if(HAL_QSPI_AutoPolling(&hqspi, &scmd, &scfg, Timeout) != HAL_OK) return 0;

    return 1;
}

int QSPI_WriteEnable(void)
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

    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

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

    if(HAL_QSPI_AutoPolling(&hqspi, &scmd, &scfg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

    return 1;
}

int EN25QH256A_ADR32ModeSet(void)
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

    if(HAL_QSPI_AutoPolling(&hqspi, &scmd, &scfg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

    return 1;
}


int QSPI_DummyCyclesCfg(void)
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

    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;
    if(HAL_QSPI_Receive(&hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

    /* Enable write operations ---------------------------------------- */
    if(!QSPI_WriteEnable()) return 0;

    /* Write Volatile Configuration register (with new dummy cycles) -- */

    reg &= ~(EN25QH256A_REG_1_4_4_DMY_CYCLE);
    reg |= EN25QH256A_REG_1_4_4_DMY_CYCLE;

    scmd.Instruction = EN25QH256A_CMD_STAT3_REG_WR;

    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;
    if(HAL_QSPI_Transmit(&hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

    return 1;
}

int QSPI_MappedMode(void)
{
    QSPI_CommandTypeDef scmd;
    QSPI_MemoryMappedTypeDef smmapcfg = {.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE};

    if(QSPI_DummyCyclesCfg() != 1) return 0;

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

    if(HAL_QSPI_MemoryMapped(&hqspi, &scmd, &smmapcfg) != HAL_OK) return 0;

    return 1;
}

int EN25QH256A_FlashReset(void)
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
    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

    // EN25QH256A reset start.
    scmd.Instruction       = QSPI_CMD_RST_START;
    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

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

    if(HAL_QSPI_AutoPolling(&hqspi, &scmd, &scfg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

    return 1;
}

uint32_t CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal)
{
  uint8_t missalignementAddress = StartAddress%4;
  uint8_t missalignementSize = Size;
  int cnt;
  uint32_t Val;

  StartAddress-=StartAddress%4;
  Size += (Size%4==0)?0:4-(Size%4);

  for(cnt=0; cnt<Size ; cnt+=4)
  {
    Val = *(uint32_t*)StartAddress;
    if(missalignementAddress)
    {
      switch (missalignementAddress)
      {
        case 1:
          InitVal += (uint8_t) (Val>>8 & 0xff);
          InitVal += (uint8_t) (Val>>16 & 0xff);
          InitVal += (uint8_t) (Val>>24 & 0xff);
          missalignementAddress-=1;
          break;
        case 2:
          InitVal += (uint8_t) (Val>>16 & 0xff);
          InitVal += (uint8_t) (Val>>24 & 0xff);
          missalignementAddress-=2;
          break;
        case 3:
          InitVal += (uint8_t) (Val>>24 & 0xff);
          missalignementAddress-=3;
          break;
      }
    }
    else if((Size-missalignementSize)%4 && (Size-cnt) <=4)
    {
      switch (Size-missalignementSize)
      {
        case 1:
          InitVal += (uint8_t) Val;
          InitVal += (uint8_t) (Val>>8 & 0xff);
          InitVal += (uint8_t) (Val>>16 & 0xff);
          missalignementSize-=1;
          break;
        case 2:
          InitVal += (uint8_t) Val;
          InitVal += (uint8_t) (Val>>8 & 0xff);
          missalignementSize-=2;
          break;
        case 3:
          InitVal += (uint8_t) Val;
          missalignementSize-=3;
          break;
      }
    }
    else
    {
      InitVal += (uint8_t) Val;
      InitVal += (uint8_t) (Val>>8 & 0xff);
      InitVal += (uint8_t) (Val>>16 & 0xff);
      InitVal += (uint8_t) (Val>>24 & 0xff);
    }
    StartAddress+=4;
  }

  return (InitVal);
}

int QSPI_WritePage(unsigned long adr, unsigned long sz, unsigned char *buf)
{
    QSPI_CommandTypeDef scmd;

    if(QSPI_WriteEnable() != 1) return 0;

    /* Writing Sequence ------------------------------------------------ */
    scmd.Instruction       = QSPI_CMD_PROG_1_1_4;
    scmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    scmd.Address           = adr & QSPI_MMAP_LEN_MAX;
    scmd.AddressMode       = QSPI_ADDRESS_1_LINE;
    scmd.AddressSize       = QSPI_ADDRESS_32_BITS;
    scmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    scmd.NbData            = sz;
    scmd.DataMode          = QSPI_DATA_4_LINES;
    scmd.DummyCycles       = 0;
    scmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
    scmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    scmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

    if(HAL_QSPI_Transmit(&hqspi, buf, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return 0;

    /* Configure automatic polling mode to wait for end of program ----- */
    if(QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != 1) return 0;

    return 1;
}

KeepInCompilation int Init(uint8_t configureMemoryMappedMode)
{
    SystemInit();

    SCB_EnableICache();
    SCB_EnableDCache();

    HAL_DeInit();

    hqspi.Instance = QUADSPI;
    HAL_QSPI_DeInit(&hqspi);
    huart1.Instance = USART1;
    HAL_UART_DeInit(&huart1);

    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_QUADSPI_Init();
    MX_USART1_UART_Init();


    EN25QH256A_FlashReset();

    EN25QH256A_ADR32ModeSet();

    if(!configureMemoryMappedMode)
    {
      if(QSPI_MappedMode()!=1) return 0;
    }

    sprintf(uart_buf, "%s() mmap_mode[%d]\r\n",__FUNCTION__, configureMemoryMappedMode);
    HAL_UART_Transmit(&huart1, (const uint8_t*)uart_buf, strlen(uart_buf), HAL_UART_TIMEOUT_VALUE);

    return 1;
}

KeepInCompilation int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress)
{
    QSPI_CommandTypeDef scmd = {0};

    sprintf(uart_buf, "%s() Enter. adr[0x%08lx:0x%08lx]\r\n",__FUNCTION__, EraseStartAddress, EraseEndAddress);
    HAL_UART_Transmit(&huart1, (const uint8_t*)uart_buf, strlen(uart_buf), HAL_UART_TIMEOUT_VALUE);

    EraseStartAddress = EraseStartAddress - (EraseStartAddress % 0x1000);

    while(EraseEndAddress >= EraseStartAddress)
    {

      /* Erasing Sequence -------------------------------------------------- */
      scmd.Instruction       = QSPI_CMD_SECTOR_ERASE_ADR32;
      scmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
      scmd.Address           = EraseStartAddress & QSPI_MMAP_LEN_MAX;
      scmd.AddressMode       = QSPI_ADDRESS_1_LINE;
      scmd.AddressSize       = QSPI_ADDRESS_32_BITS;
      scmd.DataMode          = QSPI_DATA_NONE;
      scmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
      scmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
      scmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

      if(QSPI_WriteEnable() != 1) return 0;

      if(HAL_OK !=  HAL_QSPI_Command(&hqspi, &scmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)) return 0;

      if(QSPI_AutoPollingMemReady(EN25QH256A_SUBSECTOR_ERASE_MAX_TIME) != 1) return 0;

      EraseStartAddress += 0x1000;
    }

    return 1;
}

KeepInCompilation int Write(uint32_t Address, uint32_t Size, uint8_t* buffer)
{
    uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
    uint32_t   QSPI_DataNum = 0;

    sprintf(uart_buf, "%s() Enter.\r\n",__FUNCTION__);
    HAL_UART_Transmit(&huart1, (const uint8_t*)uart_buf, strlen(uart_buf), HAL_UART_TIMEOUT_VALUE);

    Addr = Address % QSPI_PAGE_SIZE;
    count = QSPI_PAGE_SIZE - Addr;
    NumOfPage =  Size / QSPI_PAGE_SIZE;
    NumOfSingle = Size % QSPI_PAGE_SIZE;

    if (Addr == 0) /*!< Address is QSPI_PAGESIZE aligned  */
    {
      if (NumOfPage == 0) /*!< NumByteToWrite < QSPI_PAGESIZE */
      {
        QSPI_DataNum = Size;
        QSPI_WritePage(Address, QSPI_DataNum, buffer);
      }
      else /*!< Size > QSPI_PAGESIZE */
      {
        while (NumOfPage--)
        {
          QSPI_DataNum = QSPI_PAGE_SIZE;
          QSPI_WritePage(Address, QSPI_DataNum, buffer);
          Address +=  QSPI_PAGE_SIZE;
          buffer += QSPI_PAGE_SIZE;
        }

        QSPI_DataNum = NumOfSingle;
        if(QSPI_DataNum > 0)
          QSPI_WritePage(Address, QSPI_DataNum, buffer);
      }
    }
    else /*!< Address is not QSPI_PAGESIZE aligned  */
    {
      if (NumOfPage == 0) /*!< Size < QSPI_PAGESIZE */
      {
        if (NumOfSingle > count) /*!< (Size + Address) > QSPI_PAGESIZE */
        {
          temp = NumOfSingle - count;
          QSPI_DataNum = count;
          QSPI_WritePage(Address, QSPI_DataNum, buffer);
          Address +=  count;
          buffer += count;
          QSPI_DataNum = temp;
          QSPI_WritePage(Address, QSPI_DataNum, buffer);
        }
        else
        {
          QSPI_DataNum = Size;
          QSPI_WritePage(Address, QSPI_DataNum, buffer);
        }
      }
      else /*!< Size > QSPI_PAGESIZE */
      {
        Size -= count;
        NumOfPage =  Size / QSPI_PAGE_SIZE;
        NumOfSingle = Size % QSPI_PAGE_SIZE;
        QSPI_DataNum = count;
        QSPI_WritePage(Address, QSPI_DataNum, buffer);
        Address +=  count;
        buffer += count;

        while (NumOfPage--)
        {
          QSPI_DataNum = QSPI_PAGE_SIZE;
          QSPI_WritePage(Address, QSPI_DataNum, buffer);
          Address +=  QSPI_PAGE_SIZE;
          buffer += QSPI_PAGE_SIZE;
        }

        if (NumOfSingle != 0)
        {
          QSPI_DataNum = NumOfSingle;
          QSPI_WritePage(Address, QSPI_DataNum, buffer);
        }
      }
    }

    return 1;
}

KeepInCompilation uint64_t Verify(uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement)
{
    uint32_t VerifiedData = 0, InitVal = 0;
    uint64_t checksum;
    Size*=4;

    sprintf(uart_buf, "%s()\r\n",__FUNCTION__);
    HAL_UART_Transmit(&huart1, (const uint8_t*)uart_buf, strlen(uart_buf), HAL_UART_TIMEOUT_VALUE);

    if(QSPI_MappedMode()!=1) return 0;

    checksum = CheckSum((uint32_t)MemoryAddr + (missalignement & 0xF), Size - ((missalignement >> 16) & 0xF), InitVal);
    while (Size>VerifiedData)
    {
      if(*(uint8_t*)MemoryAddr++ != *((uint8_t*)RAMBufferAddr + VerifiedData)) return ((checksum<<32) + (MemoryAddr + VerifiedData));

      VerifiedData++;
    }

    return (checksum<<32);
}
