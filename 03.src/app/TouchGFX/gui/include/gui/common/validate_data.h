/*
 * validate_data.h
 *
 *  Created on: Jan 31, 2024
 *      Author: DRYOON
 */

#ifndef LIBS_NMEA2000_INC_VALIDATE_DATA_H_
#define LIBS_NMEA2000_INC_VALIDATE_DATA_H_

#include <stdbool.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

bool isValidLongInteger(long long longInt);
bool isValidSpeed(int speedValue);
bool isValidWaterTemp(int value);
bool isValidPressure(int value);
bool isValidDegree0to360(int radian);
bool isValidDegreeAngle(int value);
bool isAngleValidScope(double angleDegree);
bool isPositionValid(double roundedLatitudeDegree, double roundedLongitudeDegree);

#ifdef __cplusplus
}
#endif

#endif /* LIBS_NMEA2000_INC_VALIDATE_DATA_H_ */
