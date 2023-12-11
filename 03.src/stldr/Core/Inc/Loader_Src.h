/*
 * Loader_Src.h
 *
 *  Created on: Dec 5, 2023
 *      Author: wcaba
 */

#ifndef INC_LOADER_SRC_H_
#define INC_LOADER_SRC_H_

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_qspi.h"

#define KeepInCompilation __attribute__((used))

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

extern void SystemClock_Config(void);

//All system initialisation
int Init (uint8_t configureMemoryMappedMode);

//QSPI operation functions
KeepInCompilation int EraseSector (unsigned long adr);
KeepInCompilation int Write(uint32_t Address, uint32_t Size, uint8_t* buffer);
KeepInCompilation uint64_t Verify (uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement);


#endif /* INC_LOADER_SRC_H_ */
