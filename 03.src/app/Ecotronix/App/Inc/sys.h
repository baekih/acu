/*
 * sys.h
 *
 *  Created on: Nov 15, 2023
 *      Author: ihbaek
 */

#ifndef APP_INC_SYS_H_
#define APP_INC_SYS_H_

#include "features.h"

/*  STM32L431RB flash 1bank * 64 block-total * 2048byte per block */
#define FLASH_START_ADRESS        0x08000000

#define FILE_NAME_LENGTH          ((uint32_t)64)
#define FILE_SIZE_LENGTH          ((uint32_t)16)

#define ABS_RETURN(x,y)           ((x) < (y)) ? ((y)-(x)) : ((x)-(y))
#define FLASH_SECTOR_NUMBER       ((uint32_t)(ABS_RETURN(APP_START_ADDR,FLASH_START_BANK1))>>12)
#define FLASH_PROTECTED_SECTORS   (~(uint32_t)((1 << FLASH_SECTOR_NUMBER) - 1))

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 32 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08008000) /* Base @ of Sector 1, 32 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08010000) /* Base @ of Sector 2, 32 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08018000) /* Base @ of Sector 3, 32 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08020000) /* Base @ of Sector 4, 128 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08040000) /* Base @ of Sector 5, 256 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08080000) /* Base @ of Sector 6, 256 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x080C0000) /* Base @ of Sector 7, 256 Kbyte */

/* End of the Flash address */
#define USER_FLASH_END_ADDRESS      (uint32_t)(0x08100000 - 1)
/* Define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

/* Define the address from where user application will be loaded.*/
#define USER_DAT_ADDRESS    ADDR_FLASH_SECTOR_7
#define USER_DAT_SECTOR     FLASH_SECTOR_7

/* Define bitmap representing user flash area that could be write protected (check restricted to pages 8-39). */
#define FLASH_SECTOR_TO_BE_PROTECTED (OB_WRP_SECTOR_0 | OB_WRP_SECTOR_1 | OB_WRP_SECTOR_2 | OB_WRP_SECTOR_3 |\
                                      OB_WRP_SECTOR_4 | OB_WRP_SECTOR_5 | OB_WRP_SECTOR_6 | OB_WRP_SECTOR_7 |\
                                      OB_WRP_SECTOR_8 | OB_WRP_SECTOR_9 | OB_WRP_SECTOR_10 | OB_WRP_SECTOR_11)

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

#define LCD_TST_IMG_CHESS       0
#define LCD_TST_IMG_WHITE       1
#define LCD_TST_IMG_RED         2
#define LCD_TST_IMG_GREEN       3
#define LCD_TST_IMG_BLUE        4
#define LCD_TST_IMG_GRAY        5
#define LCD_TST_IMG_DEF         6

#ifdef FEATURE_LCD5

#define TS_I2C_ADR          0x5D

#define TS_RES_W_REG        0x8048
#define TS_RES_W_LEN        4

#define TS_PID_REG          0x8140
#define TS_PID_LEN          4
#define TS_RES_REG          0x8146
#define TS_RES_LEN          4
#define TS_STAT_REG         0x814E
#define TS_STAT_LEN         1
#define TS_STAT_BUF_EN_MSK  0x80
#define TS_STAT_NUM_MSK     0x0F
#define TS_X1_REG           0x8150
#define TS_X1_LEN           2
#define TS_Y1_REG           0x8152
#define TS_Y1_LEN           2
#define TS_X2_REG           0x8158
#define TS_X2_LEN           2
#define TS_Y2_REG           0x815A
#define TS_Y2_LEN           2
#define TS_X3_REG           0x8160
#define TS_X3_LEN           2
#define TS_Y3_REG           0x8162
#define TS_Y3_LEN           2
#define TS_X4_REG           0x8168
#define TS_X4_LEN           2
#define TS_Y4_REG           0x816A
#define TS_Y4_LEN           2
#define TS_X5_REG           0x8170
#define TS_X5_LEN           2
#define TS_Y5_REG           0x8172
#define TS_Y5_LEN           2
#define TS_PTR1_REG         TS_X1_REG
#define TS_PTR1_LEN         (TS_X1_LEN + TS_Y1_LEN)

#endif // FI_DIN_1_0

enum
{
    FLASHIF_OK = 0,
    FLASHIF_ERASE_ERROR,
    FLASHIF_WRITINGCTRL_ERROR,
    FLASHIF_WRITING_ERROR
};

enum{
    FLASHIF_PROTECTION_NONE         = 0,
    FLASHIF_PROTECTION_PCROPENABLED = 0x1,
    FLASHIF_PROTECTION_WRPENABLED   = 0x2,
    FLASHIF_PROTECTION_RDPENABLED   = 0x4,
};

typedef struct _app_dat
{
    uint8_t lcd_bl;
    uint8_t bzr_vol;
    uint8_t rsv;
    uint8_t chksum;
} app_dat  __attribute__((aligned(1)));

#ifdef FEATURE_LCD5
typedef struct __ts_position
{
    uint16_t x;
    uint16_t y;
} ts_position;
#endif // FEATURE_LCD5

extern UART_HandleTypeDef huart1, huart2;
extern CAN_HandleTypeDef hcan1;
extern QSPI_HandleTypeDef hqspi;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;
extern I2C_HandleTypeDef hi2c1;

extern uint32_t g_val;
extern app_dat g_app_dat;
extern app_dat g_app_dat_org;

uint32_t doFlashErase(void);
uint32_t doFlashWrite(uint32_t, uint32_t*, uint32_t);

#ifdef FEATURE_LCD5
void initTS(void);
void getTS(void);
#endif
void InitQSPI(void);
void setBuzzer(uint8_t);
void setLCDBL(uint8_t);
void setFlashDAT(app_dat);
void setLCDTestImage(uint8_t);

#endif /* APP_INC_SYS_H_ */
