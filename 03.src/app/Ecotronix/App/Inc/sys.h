/*
 * sys.h
 *
 *  Created on: Nov 15, 2023
 *      Author: ihbaek
 */

#ifndef APP_INC_SYS_H_
#define APP_INC_SYS_H_

#include "common.h"

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
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08100000) /* Base @ of Sector 8, 256 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x08140000) /* Base @ of Sector 9, 256 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x08180000) /* Base @ of Sector 10, 256 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x081C0000) /* Base @ of Sector 11, 256 Kbyte */

/* End of the Flash address */
#define USER_FLASH_END_ADDRESS      (uint32_t)0x0800FFFF
/* Define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

/* Define the address from where user application will be loaded.
 Note: the 1st sector 0x08000000-0x08003FFF is reserved for the IAP code */
#define APPLICATION_ADDRESS        (uint32_t)0x08008000

/* Define bitmap representing user flash area that could be write protected (check restricted to pages 8-39). */
#define FLASH_SECTOR_TO_BE_PROTECTED (OB_WRP_SECTOR_0 | OB_WRP_SECTOR_1 | OB_WRP_SECTOR_2 | OB_WRP_SECTOR_3 |\
                                      OB_WRP_SECTOR_4 | OB_WRP_SECTOR_5 | OB_WRP_SECTOR_6 | OB_WRP_SECTOR_7 |\
                                      OB_WRP_SECTOR_8 | OB_WRP_SECTOR_9 | OB_WRP_SECTOR_10 | OB_WRP_SECTOR_11)

enum
{
    FLASHIF_OK = 0,
    FLASHIF_ERASEKO,
    FLASHIF_WRITINGCTRL_ERROR,
    FLASHIF_WRITING_ERROR
};

enum{
    FLASHIF_PROTECTION_NONE         = 0,
    FLASHIF_PROTECTION_PCROPENABLED = 0x1,
    FLASHIF_PROTECTION_WRPENABLED   = 0x2,
    FLASHIF_PROTECTION_RDPENABLED   = 0x4,
};

#define LCD_TST_IMG_CHESS       0
#define LCD_TST_IMG_WHITE       1
#define LCD_TST_IMG_RED         2
#define LCD_TST_IMG_GREEN       3
#define LCD_TST_IMG_BLUE        4
#define LCD_TST_IMG_GRAY        5
#define LCD_TST_IMG_KITTEN      6

uint32_t doFlashErase(void);
uint32_t doFlashWrite(uint32_t, uint64_t*, uint32_t);

void setBuzzer(uint8_t);
void setLCDBL(uint8_t);
void setLCDTestImage(uint32_t);

#endif /* APP_INC_SYS_H_ */
