/*
 * nmea_database.c
 *
 *  Created on: 1, 24, 2024
 *      Author: yhk
 */


/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "gui/common/DataBase.hpp"

const float meterToFeetFactor = 3.28083989501312;
const float meterToFathom = 0.546806649168854;
const float meterToHR = 0.65997888;
const float meterToPB = 0.617283950617284;

const float tempFahrenheitMax = 211.8;
const float tempFahrenheitMin = 14.2;

const float tempCelsiusDisplayMax = 99.9;
const float tempCelsiusDisplayMin = -9.9;

const float tempCelsiusNMEA0183Max = 99.99;
const float tempCelsiusNMEA0183Min = -9.99;

float hdgValue = 0;

float stwValues[4];
float sogValues[4];
float depthValues[5];
float wtempValues[2];

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
	return ((tempFahrenheit - 32) * ((double) 5 / (double) 9));
}

inline double FahrenheitToCelsiusSpan(double tempFahrenheit) {
	return (tempFahrenheit * ((double) 5 / (double) 9));
}

inline bool CelsiusToFahrenheit(double tempCelsius){
	return ((tempCelsius * 9.0 / 5.0) + 32);
}

inline bool CelsiusToFahrenheitSpan(double tempCelsius){
	return (tempCelsius * 9.0 / 5.0);
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

void setHDGValue(float hdg)
{
	hdgValue =  hdg / (2*M_PI) * 360;


}

float getHDGValue()
{
	return hdgValue;
}

void setSTWValue(float value, int type)
{
	if(type == SPEED_UNIT_MPS){
		float knot = MPS_TO_KNOT(value);

		stwValues[SPEED_UNIT_KNOT] = knot;
		stwValues[SPEED_UNIT_KMH] = KNOT_TO_KMH(knot);
		stwValues[SPEED_UNIT_MPH] = KNOT_TO_MPH(knot);
		stwValues[SPEED_UNIT_MPS] = value;
	}
}

float getSTWValue(int type)
{
	return stwValues[type];
}

void setSOGValue(float value, int type)
{
	if(type == SPEED_UNIT_MPS){
		float knot = MPS_TO_KNOT(value);

		sogValues[SPEED_UNIT_KNOT] = knot;
		sogValues[SPEED_UNIT_KMH] = KNOT_TO_KMH(knot);
		sogValues[SPEED_UNIT_MPH] = KNOT_TO_MPH(knot);
		sogValues[SPEED_UNIT_MPS] = value;
	}
}

float getSOGValue(int type)
{
	return sogValues[type];
}

void setDepthMeterValue(float depthMeter)
{
    depthValues[0] = depthMeter;                        // meter
    depthValues[1] = depthMeter * meterToFeetFactor;    // feet
    depthValues[2] = depthMeter * meterToFathom;        // fathoms
    depthValues[3] = depthMeter * meterToHR;            // HR
    depthValues[4] = depthMeter * meterToPB;            // PB
}

float getDepthValue(int type)
{
	return depthValues[type];
}

void setWTemp(float wtemp, int type)
{

}

float getWTempValue(int type)
{
	return wtempValues[type];
}


