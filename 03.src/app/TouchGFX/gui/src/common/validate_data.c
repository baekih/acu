/*
 * validate_data.c
 *
 *  Created on: Jan 31, 2024
 *      Author: DRYOON
 */

#include <gui/common/validate_data.h>
#include <gui/common/DataBase.hpp>

#define LONG_INT_MAX_VALUE          (4294967293L)
#define SHORT_MAX_VALID_VALUE       (65532)

#define INTEGER_SPEED_MIN_VALUE     0
#define INTEGER_SPEED_MAX_VALUE     SHORT_MAX_VALID_VALUE

#define INTEGER_DEGREE_MIN_VALUE    0
#define INTEGER_DEGREE_MAX_VALUE    62832

bool isValidLongInteger(long long longInt)
{
    return (longInt >= 0 && longInt < LONG_INT_MAX_VALUE) ? true : false;
}

bool isValidSpeed(int speedValue)
{
    return (speedValue >= INTEGER_SPEED_MIN_VALUE && speedValue <= INTEGER_SPEED_MAX_VALUE) ? true : false;
}

bool isValidWaterTemp(int value)
{
    const int INTEGER_TEMP_MIN_VALUE     = 0;
    const int INTEGER_TEMP_MAX_VALUE     = SHORT_MAX_VALID_VALUE;

    return (value >= INTEGER_TEMP_MIN_VALUE && value <= INTEGER_TEMP_MAX_VALUE) ? true : false;
}

bool isValidPressure(int value)
{
    const int INTEGER_PRESSURE_MIN_VALUE  = 0;
    const int INTEGER_PRESSURE_MAX_VALUE  = SHORT_MAX_VALID_VALUE;

    return (value >= INTEGER_PRESSURE_MIN_VALUE && value <= INTEGER_PRESSURE_MAX_VALUE) ? true : false;
}

bool isValidDegree0to360(int radian)
{
    return isValidDegreeAngle(radian);
}

bool isValidDegreeAngle(int value){
    return (value >= INTEGER_DEGREE_MIN_VALUE && value <= INTEGER_DEGREE_MAX_VALUE) ? true : false;
}

bool isAngleValidScope(double angleDegree){
    double max = 360.0;
    double min = 0.0;

    angleDegree = GetRound(angleDegree, 10);

    if(angleDegree >= max) return false;
    if(angleDegree < min) return false;

    return true;
}

bool isPositionValid(double latitudeDegree, double longitudeDegree){
    if(latitudeDegree > 90 || latitudeDegree < -90){
        return false;
    }

    if(longitudeDegree > 180 || longitudeDegree < -180){
        return false;
    }

    return true;
}
