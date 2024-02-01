/*
 * validate_data.c
 *
 *  Created on: Jan 31, 2024
 *      Author: DRYOON
 */

#include <validate_data.h>

#define LONG_INT_MAX_VALUE 			(4294967293L)
#define SHORT_MAX_VALID_VALUE		(65532)

#define INTEGER_SPEED_MIN_VALUE		0
#define INTEGER_SPEED_MAX_VALUE 	SHORT_MAX_VALID_VALUE

bool isValidLongInteger(long longInt)
{
	return (longInt >= 0 && longInt < LONG_INT_MAX_VALUE) ? true : false;
}

bool isValidSpeed(int speedValue)
{
	return (speedValue >= INTEGER_SPEED_MIN_VALUE && speedValue <= INTEGER_SPEED_MAX_VALUE) ? true : false;
}

float GetRound(float val, float roundFraction)
{
	return ((float)round(val * roundFraction) / roundFraction);
}
