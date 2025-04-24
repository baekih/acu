/*
 * DataBase.hpp
 *
 *  Created on: 1, 24, 2024
 *      Author: yhk
 */

#ifndef NMEA2000_INC_NMEA_DATABASE_H_
#define NMEA2000_INC_NMEA_DATABASE_H_

//==============================================================================
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private struct ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
//==============================================================================

#ifdef __cplusplus
extern "C" {
#endif


#define SPEED_UNIT_KNOT 0
#define SPEED_UNIT_KMH  1
#define SPEED_UNIT_MPH  2
#define SPEED_UNIT_MPS  3

#define DEPTH_UNIT_METER 	0
#define DEPTH_UNIT_FEET  	1
#define DEPTH_UNIT_FATHOMS  2
#define DEPTH_UNIT_HR  		3
#define DEPTH_UNIT_PB  		4

#define UNIT_TEMP_CELSIUS 		0
#define UNIT_TEMP_FAHRENHEIT  	1

#define MPS_TO_KNOT(mps) 	(mps * 1.943844492440608)

#define KNOT_TO_MPH(knot) 	(knot * 1.15077944802354)
#define KNOT_TO_KMH(knot)	(knot * 1.852)
#define KNOT_TO_MPS(knot)	(knot * 1.15077944802354)

#define MPH_TO_KMH(mph)		(mph * 1.609344)
#define MPH_TO_KNOT(mph)	(mph * 0.86897676241900648)
#define MPH_TO_MPS(mph)		(mph * 0.44704)

#define KMH_TO_MPS(Kmh)		(Kmh * 0.539956803455724 * 0.51444444444)
#define KMH_TO_KNOT(Kmh)	(Kmh * 0.539956803455724)
#define KMH_TO_MPH(Kmh) 	(Kmh * 0.62137119223733)

#define OUT_OF_RANGE		"*99.9"
#define SPEED_OUT_OF_RANGE	"*9.9"

#define INVALID_DEGREE_VALUE (-99999.9)

extern const int typeSTW;
extern const int typeSOG;

extern const double meterToFeetFactor;
extern const double meterToFathom;
extern const double meterToHR;
extern const double meterToPB;

extern const double tempFahrenheitMax;
extern const double tempFahrenheitMin;

extern const double tempCelsiusDisplayMax;
extern const double tempCelsiusDisplayMin;

extern const double tempCelsiusNMEA0183Max;
extern const double tempCelsiusNMEA0183Min;

extern const double speedDisplayMax;
extern const double speedDisplayMin;

extern const double depthDisplayMin;
extern const double depthDisplayFeetMax;
extern const double depthDisplayMeterMax;
extern const double depthDisplayFathomMax;
extern const double depthDisplayPBMax;

double GetRound(double val, double roundFraction);

bool isTempFahrenheitValidScope(double tempF);
bool isTempCelsiusValidScope(double tempC);

bool isTempCelsiusNMEA0183OutputValidScope(double tempC);

double FahrenheitToCelsius(double tempFahrenheit);
double FahrenheitToCelsiusSpan(double tempFahrenheit);

double CelsiusToFahrenheit(double tempCelsius);
double CelsiusToFahrenheitSpan(double tempCelsius);

double KelvinToCelsius(double tempKelvin);
double CelsiusToKelvin(double tempC);
double FahrenheitToKelvin(double tempF);

double GetRadianToDegree360(int radian);
double GetRadianToDegree180(int radian);

double GetCorrection0to360(double degree);

double adjustDisplayAngleDegree(double angle);

void setHDGValue(double hdg);
double getHDGValue();
bool isTimeInHDG();
void setValidHDG(bool valid);
bool isValidHDG();
void setVariation(double variation);
double getVariation();

void setSTWValue(double value, int type);
double getSTWValue(int type);

void setSOGValue(double value, int type);
double getSOGValue(int type);

void setDepthMeterValue(double depthMeter);
double getDepthValue(int type);

void setWTempValue(double wtemp, int type);
double getWTempValue(int type);

void setWindValue(double windSpeed, int windDirection, int windRef);
double getWindSpeedValue(int type);

void setPosition(double latitude, double longitude, int MethodGNSS);
void setPositionRapid(double latitude, double longitude);
double getLatitude();
double getLongitude();

#ifdef __cplusplus
}
#endif

#endif /* NMEA2000_INC_NMEA_DATABASE_H_ */
