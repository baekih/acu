/*
 * nmea_database.c
 *
 *  Created on: 1, 24, 2024
 *      Author: yhk
 */


/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "gui/common/DataBase.hpp"
#include "gui/common/validate_data.h"

#include "printf.h"

#ifndef SIMULATOR
#include "stm32f7xx_hal.h"
#endif

#define NON_RECEIVE_NMEA_TIMEOUT_30_SEC  30000 //ms
#define NON_RECEIVE_NMEA_TIMEOUT_5_SEC   5000 //ms

const int typeSTW = 0;
const int typeSOG = 1;

const double meterToFeetFactor      = 3.28083989501312;
const double meterToFathom          = 0.546806649168854;
const double meterToHR              = 0.65997888;
const double meterToPB              = 0.617283950617284;

const double tempFahrenheitMax      = 130.0;
const double tempFahrenheitMin      = 0.0;

const double tempCelsiusDisplayMax  = 50.0;
const double tempCelsiusDisplayMin  = -20.0;

const double tempCelsiusNMEA0183Max = 99.99;
const double tempCelsiusNMEA0183Min = -9.99;

const double speedDisplayMax        = 999.0;
const double speedDisplayMin        = 0.0;

const double depthDisplayMin        = 0.0;
const double depthDisplayFeetMax    = 4921.0;
const double depthDisplayMeterMax   = 1500.0;
const double depthDisplayFathomMax  = 820.0;
const double depthDisplayPBMax      = 926.0;

const double headingDisplayMin      = 0.0;
const double headingDisplayMax      = 360.0;

float rudcurValue = NAN;
float rudtgtValue = NAN;

double cogValue = INVALID_DEGREE_VALUE;
bool validCOGValue = false;

double Variation = INVALID_DEGREE_VALUE;

#ifndef SIMULATOR
uint32_t lastReceiveHDGcurValue = 0;
#endif

double stwValues[4];
double sogValues[4];
double depthValues[5];
double headingValues[2];
double wtempValues[2];

double windSpeedValues[4];

double latitudeDegree = INVALID_DEGREE_VALUE;
double longitudeDegree = INVALID_DEGREE_VALUE;

XTE g_XTE = {.dat = 0, .mode = 0};

double GetRound(double val, double roundFraction)
{
    return ((double)round(val * roundFraction) / roundFraction);
}

inline bool isTempFahrenheitValidScope(double tempF){
    return (tempF >= tempFahrenheitMin && tempF <= tempFahrenheitMax);
}

inline bool isTempCelsiusValidScope(double tempC){
    return (tempC >= tempCelsiusDisplayMin && tempC <= tempCelsiusDisplayMax);
}

inline bool isTempCelsiusNMEA0183OutputValidScope(double tempC){
    return (tempC >= tempCelsiusNMEA0183Min && tempC <= tempCelsiusNMEA0183Max);
}

inline double FahrenheitToCelsius(double tempFahrenheit) {
    return ((tempFahrenheit - 32) / 1.8);
}

inline double FahrenheitToCelsiusSpan(double tempFahrenheit) {
    return (tempFahrenheit / 1.8);
}

inline double CelsiusToFahrenheit(double tempCelsius){
    return ((tempCelsius * 1.8) + 32);
}

inline double CelsiusToFahrenheitSpan(double tempCelsius){
    return (tempCelsius * 1.8);
}
inline double KelvinToCelsius(double tempKelvin){
    return (tempKelvin - 273.15);
}

inline double CelsiusToKelvin(double tempC){
    return 273.15 + tempC;
}

inline double FahrenheitToKelvin(double tempF){
    return 273.15 + FahrenheitToCelsius(tempF);
}

double GetRadianToDegree360(double radian){
    return ((((double)radian / 10000) / (2*M_PI)) * 360);
}

double GetRadianToDegree180(double radian){
    return (((double)((short)radian)/10000) / M_PI) * 180;
}

double GetCorrection0to360(double degree){
    degree = GetRound(degree, 10.0);

    if(degree >= 360.0 ) degree -= 360;
    if(degree < 0) degree += 360;

    return degree;
}


double adjustDisplayAngleDegree(double angle) {
    double res = angle;

    if (res >= 360.0) {
        res = res - 360.0;
    } else if (res <= 0.0) {
        res = 360.0 + res;
    }

    if ((double)round(res * 10) / 10 == 360.0) {
        res = 0.0;
    }

    return res;
}

void setCOGValue(double cog)
{
    cogValue = GetRadianToDegree360(cog);
#ifndef SIMULATOR
    lastReceiveHDGcurValue = HAL_GetTick();
#endif
    validCOGValue = true;
}

double getCOGValue()
{
    return cogValue;
}

void setVariation(double variation)
{
    Variation = GetRadianToDegree360(variation);
}

double getVariation()
{
    return Variation;
}

void setXTE(double xte, unsigned char xte_mode)
{
    g_XTE.dat = xte;
    g_XTE.mode = xte_mode;
}

XTE getXTE()
{
    return g_XTE;
}

void setSTWValue(double value, int type)
{
    if(type == SPEED_UNIT_MPS){
        double knot = MPS_TO_KNOT(value);

        stwValues[SPEED_UNIT_KNOT] = knot;
        stwValues[SPEED_UNIT_KMH] = KNOT_TO_KMH(knot);
        stwValues[SPEED_UNIT_MPH] = KNOT_TO_MPH(knot);
        stwValues[SPEED_UNIT_MPS] = value;
    }
}

double getSTWValue(int type)
{
    return stwValues[type];
}

void setSOGValue(double value, int type)
{
    if(type == SPEED_UNIT_MPS){
        double knot = MPS_TO_KNOT(value);

        sogValues[SPEED_UNIT_KNOT] = knot;
        sogValues[SPEED_UNIT_KMH] = KNOT_TO_KMH(knot);
        sogValues[SPEED_UNIT_MPH] = KNOT_TO_MPH(knot);
        sogValues[SPEED_UNIT_MPS] = value;
    }
}

double getSOGValue(int type)
{
    return sogValues[type];
}

void setWindValue(double windSpeed, int windDirection, int windRef)
{
    if(isValidSpeed(windSpeed)){
        double mps = windSpeed / 100;
        double knot = MPS_TO_KNOT(mps);

        windSpeedValues[SPEED_UNIT_KNOT] = knot;
        windSpeedValues[SPEED_UNIT_KMH] = KNOT_TO_KMH(knot);
        windSpeedValues[SPEED_UNIT_MPH] = KNOT_TO_MPH(knot);
        windSpeedValues[SPEED_UNIT_MPS] = mps;
    }
}

double getWindSpeedValue(int type)
{
    return windSpeedValues[type];
}

void setDepthMeterValue(double depthMeter)
{
    depthValues[0] = depthMeter;                        // meter
    depthValues[1] = depthMeter * meterToFeetFactor;    // feet
    depthValues[2] = depthMeter * meterToFathom;        // fathoms
    depthValues[3] = depthMeter * meterToHR;            // HR
    depthValues[4] = depthMeter * meterToPB;            // PB
}

double getDepthValue(int type)
{
    return depthValues[type];
}

void setWTempValue(double wtemp, int type)
{
    if(type == UNIT_TEMP_CELSIUS){
        wtempValues[UNIT_TEMP_CELSIUS] = KelvinToCelsius(wtemp);
        wtempValues[UNIT_TEMP_FAHRENHEIT] = CelsiusToFahrenheit(wtempValues[UNIT_TEMP_CELSIUS]);
    }
}

double getWTempValue(int type)
{
    return wtempValues[type];
}

void setPosition(double latitude, double longitude, int MethodGNSS)
{
    if ((MethodGNSS >= 1 && MethodGNSS <= 5) || MethodGNSS == 8){
        if(isPositionValid(latitude, longitude)){
            latitudeDegree = latitude;
            longitudeDegree = longitude;
        }
    }
}

void setPositionRapid(double latitude, double longitude)
{
    if(isPositionValid(latitude, longitude))
    {
        latitudeDegree = latitude;
        longitudeDegree = longitude;
    }
}

double getLatitude()
{
    return latitudeDegree;
}

double getLongitude()
{
    return longitudeDegree;
}
