/*
 * sys.h
 *
 *  Created on: Nov 15, 2023
 *      Author: ihbaek
 */

#ifndef APP_INC_SYS_H_
#define APP_INC_SYS_H_

#include "features.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_ID_ACU15           1
#define BOARD_ID_ACU10           2
#define BOARD_ID_INVAL           0xFF

#define FLASH_START_ADRESS        0x08000000

#define FILE_NAME_LENGTH          ((uint32_t)64)
#define FILE_SIZE_LENGTH          ((uint32_t)16)

#define ABS_RETURN(x,y)           ((x) < (y)) ? ((y)-(x)) : ((x)-(y))
#define FLASH_SECTOR_NUMBER       ((uint32_t)(ABS_RETURN(APP_START_ADDR,FLASH_START_BANK1))>>12)
#define FLASH_PROTECTED_SECTORS   (~(uint32_t)((1 << FLASH_SECTOR_NUMBER) - 1))

#define FLASH_SECTOR_0           ((uint32_t)0U) /*!< Sector Number 0   */
#define FLASH_SECTOR_1           ((uint32_t)1U) /*!< Sector Number 1   */
#define FLASH_SECTOR_2           ((uint32_t)2U) /*!< Sector Number 2   */
#define FLASH_SECTOR_3           ((uint32_t)3U) /*!< Sector Number 3   */
#define FLASH_SECTOR_4           ((uint32_t)4U) /*!< Sector Number 4   */
#define FLASH_SECTOR_5           ((uint32_t)5U) /*!< Sector Number 5   */
#define FLASH_SECTOR_6           ((uint32_t)6U) /*!< Sector Number 6   */
#define FLASH_SECTOR_7           ((uint32_t)7U) /*!< Sector Number 7   */

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 32 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08008000) /* Base @ of Sector 1, 32 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08010000) /* Base @ of Sector 2, 32 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08018000) /* Base @ of Sector 3, 32 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08020000) /* Base @ of Sector 4, 128 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08040000) /* Base @ of Sector 5, 256 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08080000) /* Base @ of Sector 6, 256 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x080C0000) /* Base @ of Sector 7, 256 Kbyte */

/* Define bitmap representing user flash area that could be write protected (check restricted to pages 8-39). */
#define FLASH_SECTOR_TO_BE_PROTECTED (OB_WRP_SECTOR_0 | OB_WRP_SECTOR_1 | OB_WRP_SECTOR_2 | OB_WRP_SECTOR_3 |\
                                      OB_WRP_SECTOR_4 | OB_WRP_SECTOR_5 | OB_WRP_SECTOR_6 | OB_WRP_SECTOR_7 |\
                                      OB_WRP_SECTOR_8 | OB_WRP_SECTOR_9 | OB_WRP_SECTOR_10 | OB_WRP_SECTOR_11)
#define BOOT1_START_ADDR          ADDR_FLASH_SECTOR_0
#define FLASH_DATA_ADDR           ADDR_FLASH_SECTOR_3
#define BOOT2_START_ADDR          ADDR_FLASH_SECTOR_4
#define APP_START_ADDR            ADDR_FLASH_SECTOR_5

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

#define LCD_TST_IMG_DEF         0
#define LCD_TST_IMG_WHITE       1
#define LCD_TST_IMG_RED         2
#define LCD_TST_IMG_GREEN       3
#define LCD_TST_IMG_BLUE        4
#define LCD_TST_IMG_GRAY        5
#define LCD_TST_IMG_CHESS       6
#define LCD_TST_IMG_TS          7

#define TS_INVAL_I2C_ADR            0xFF

#define TS_GT911_I2C_ADR            0x5D
#define TS_GT911_RES_W_REG          0x8048
#define TS_GT911_RES_W_LEN          4
#define TS_GT911_PID_REG            0x8140
#define TS_GT911_PID_LEN            4
#define TS_GT911_RES_REG            0x8146
#define TS_GT911_RES_LEN            4
#define TS_GT911_STAT_REG           0x814E
#define TS_GT911_STAT_LEN           1
#define TS_GT911_STAT_BUF_EN_MSK    0x80
#define TS_GT911_STAT_NUM_MSK       0x0F
#define TS_GT911_X1_REG             0x8150
#define TS_GT911_X1_LEN             2
#define TS_GT911_Y1_REG             0x8152
#define TS_GT911_Y1_LEN             2
#define TS_GT911_X2_REG             0x8158
#define TS_GT911_X2_LEN             2
#define TS_GT911_Y2_REG             0x815A
#define TS_GT911_Y2_LEN             2
#define TS_GT911_X3_REG             0x8160
#define TS_GT911_X3_LEN             2
#define TS_GT911_Y3_REG             0x8162
#define TS_GT911_Y3_LEN             2
#define TS_GT911_X4_REG             0x8168
#define TS_GT911_X4_LEN             2
#define TS_GT911_Y4_REG             0x816A
#define TS_GT911_Y4_LEN             2
#define TS_GT911_X5_REG             0x8170
#define TS_GT911_X5_LEN             2
#define TS_GT911_Y5_REG             0x8172
#define TS_GT911_Y5_LEN             2
#define TS_GT911_XY1_REG           TS_GT911_X1_REG
#define TS_GT911_XY1_LEN           (TS_GT911_X1_LEN + TS_GT911_Y1_LEN)

#define TS_ST1633_I2C_ADR           0x55
#define TS_ST1633_RES_REG           0x4
#define TS_ST1633_RES_LEN           3
#define TS_ST1633_CNT_REG           0x08
#define TS_ST1633_CNT_LEN           1
#define TS_ST1633_XY1_REG           0x12
#define TS_ST1633_XY1_LEN           3

#define SW_BZR_CONT_ON              0
#define SW_BZR_CONT_ONOFF           1
#define SW_BZR_SHOT_ONE             2
#define SW_BZR_SHOT_TWO             3
#define SW_BZR_KEYBEEP_ON           4
#define SW_KEY_PWR                  5
#define SW_KEY_PREV                 6
#define SW_KEY_SEL                  7
#define SW_KEY_UP                   8
#define SW_KEY_DN                   9
#define SW_MAX                      10

#define SW_KEY_PWR_MASK             ((0x0100ULL)<<(SW_KEY_PWR*2))
#define SW_KEY_PREV_MASK            ((0x0100ULL)<<(SW_KEY_PREV*2))
#define SW_KEY_SEL_MASK             ((0x0100ULL)<<(SW_KEY_SEL*2))
#define SW_KEY_UP_MASK              ((0x0100ULL)<<(SW_KEY_UP*2))
#define SW_KEY_DN_MASK              ((0x0100ULL)<<(SW_KEY_DN*2))

#define KEY_PWR                     0
#define KEY_PREV                    1
#define KEY_SEL                     2
#define KEY_UP                      3
#define KEY_DN                      4
#define KEY_MAX                     5

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

typedef struct _IOSAddrClame
{
    uint8_t DevInstance;
    uint8_t SysInstance;
} ISOAdrClame  __attribute__((aligned(1)));

typedef struct _common_dat
{
// 0byte
    uint32_t jmp_adr;
// 4byte
    uint16_t bootver;
    uint16_t appver;
// 8byte
    uint8_t  uniquenum[3];
    uint8_t  lcd_img_idx;
// 12byte
    ISOAdrClame adrclame;
    uint8_t  rsv2[2];
// 16byte
    uint8_t  lcd_bl;
    uint8_t  bzr_vol;
    uint8_t  nmea2k_adr;
    uint8_t  isUpdateFlashIdle;
// 20byte
    uint32_t crc32;
} common_dat __attribute__((aligned(1)));

typedef struct _key_stat
{
    bool prv;
    bool cur;
} key_stat  __attribute__((aligned(1)));

extern UART_HandleTypeDef huart1, huart2;
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;

#if defined (ECO_APP) | defined (ECO_BOOT2)
extern QSPI_HandleTypeDef hqspi;
extern I2C_HandleTypeDef hi2c1;
extern CRC_HandleTypeDef hcrc;
extern LTDC_HandleTypeDef hltdc;
#else
#error ECO_XXX NOT defined!
#endif


extern const common_dat g_common_dat_def;
extern common_dat g_common_dat;
extern uint8_t g_board_id;
extern key_stat g_key_stat[KEY_MAX];
extern uint8_t g_switch_bank[];

void printk(const char* pstr, ...);
uint32_t eraseFlash(uint32_t);
uint32_t writeFlash(uint32_t, uint32_t*, uint32_t);

int32_t eraseFlashApp(void);

int32_t doSwitchBankControl(uint64_t *pdat64);
int32_t setLCDBrightness(uint8_t lcd_bl);
bool getKeyPending(uint8_t idx);
void initTouchSensor(void);
bool getTouchSensor(uint16_t*, uint16_t*);
void initQSPI(void);
void setBuzzer(uint8_t);
void setLCDBL(uint8_t);
void initFlashData(void);
void updateFlashData(void);
void setLCDTestImage(uint8_t);
#if defined (ECO_APP)
void CAN1_SendFrame(uint32_t, uint8_t*, uint8_t);
#endif

#ifdef __cplusplus
}
#endif

#endif /* APP_INC_SYS_H_ */
