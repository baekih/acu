/*
 * Dev_Inf.c
 *
 *  Created on: Dec 5, 2023
 *      Author: ihbaek
 */

#include "Dev_Inf.h"

struct StorageInfo const StorageInfo  =  {
   "EN25QH256A_STM32F767G-FIDIN",                             // Device Name + version number
   NOR_FLASH,                                       // Device Type
   0x90000000,                                      // Device Start Address
   0x2000000,                                       // Device Size in Bytes (32MBytes)
   0x100,                                           // Programming Page Size 256Bytes
   0xFF,                                            // Initial Content of Erased Memory
// Specify Size and Address of Sectors (view example below)
   {{0x00002000, 0x00001000},                       // Sector Num : 8192 ,Sector Size: 4KBytes
    {0x00000000, 0x00000000}}
};
