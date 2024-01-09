/*
 * feature.h
 *
 *  Created on: Jan 8, 2024
 *      Author: ihbaek
 */

#ifndef APP_INC_FEATURES_H_
#define APP_INC_FEATURES_H_

#include <main.h>

#if !defined(TS_RSTn_Pin)
#define FEATURE_LCD4
#else
#define FEATURE_LCD5
#endif

#endif /* APP_INC_FEATURES_H_ */
