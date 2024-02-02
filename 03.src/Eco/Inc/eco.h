/*
 * eco.h
 *
 *  Created on: Jan 8, 2024
 *      Author: ihbaek
 */

#ifndef APP_INC_ECO_H_
#define APP_INC_ECO_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "cmsis_os.h"

#include "features.h"
#include "sys.h"
#include "printf.h"
#include "common.h"
#if defined (ECO_APP)
#include "app.h"
#elif defined(ECO_BOOT2)
#include "boot2.h"
#endif
//#include "nmea2k.h"
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "images.h"

//#include "stm32f767_fi-din_lcd.h"
#include "stm32f767_fi-din_sdram.h"

#endif /* APP_INC_ECO_H_ */
