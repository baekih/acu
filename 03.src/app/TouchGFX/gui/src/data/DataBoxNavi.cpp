/*
 * DataBoxNavi.cpp
 *
 *  Created on: Feb 7, 2024
 *      Author: DRYOON
 */


#include <math.h>
#include <cstdio>

#include <gui_generated/databox_screen/DataboxViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/data/DataBoxNavi.hpp>

NaviWidget::NaviWidget()
{

}

void NaviWidget::createWidget(int pos)
{
    int boxPosition[NAVI_WIDGET_MAX_COUNT][4] = {
        { 0,   0, 800, 120 },
        { 0, 120, 800, 120 },
        { 0, 240, 800, 120 },
        { 0, 360, 800, 120 },
    };

    NAVI_BOX.setPosition(boxPosition[pos][0], boxPosition[pos][1], boxPosition[pos][2], boxPosition[pos][3]);
    NAVI_BOX.setColor(touchgfx::Color::getColorFromRGB(52, 52, 51));
    NAVI_BOX.setBorderColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    NAVI_BOX.setBorderSize(2);

    NAVI_LATITUDE.setPosition(boxPosition[pos][0] + 87, boxPosition[pos][1] - 7, 478, 88);
    NAVI_LATITUDE.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    NAVI_LATITUDE.setLinespacing(0);
    Unicode::snprintf(NAVI_LATITUDEBuffer, NAVI_LATITUDE_SIZE, "%s", touchgfx::TypedText(T_DATABOX_NAVI_LATITUDE).getText());
    NAVI_LATITUDE.setWildcard(NAVI_LATITUDEBuffer);
    NAVI_LATITUDE.setTypedText(touchgfx::TypedText(T_WILDCARD_65PX));

    NAVI_LATITUDE_INDI.setPosition(boxPosition[pos][0] + 565, boxPosition[pos][1] - 7, 58, 81);
    NAVI_LATITUDE_INDI.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    NAVI_LATITUDE_INDI.setLinespacing(0);
    Unicode::snprintf(NAVI_LATITUDE_INDIBuffer, NAVI_LATITUDE_INDI_SIZE, "%s", touchgfx::TypedText(T_DATABOX_LATI_INDI).getText());
    NAVI_LATITUDE_INDI.setWildcard(NAVI_LATITUDE_INDIBuffer);
    NAVI_LATITUDE_INDI.setTypedText(touchgfx::TypedText(T_WILDCARD_65PX));

    NAVI_LONGITUDE.setPosition(boxPosition[pos][0] + 87, boxPosition[pos][1] + 47, 478, 88);
    NAVI_LONGITUDE.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    NAVI_LONGITUDE.setLinespacing(0);
    Unicode::snprintf(NAVI_LONGITUDEBuffer, NAVI_LONGITUDE_SIZE, "%s", touchgfx::TypedText(T_DATABOX_NAVI_LONGITUDE).getText());
    NAVI_LONGITUDE.setWildcard(NAVI_LONGITUDEBuffer);
    NAVI_LONGITUDE.setTypedText(touchgfx::TypedText(T_WILDCARD_65PX));

    NAVI_LONGITUDE_INDI.setPosition(boxPosition[pos][0] + 565, boxPosition[pos][1] + 47, 58, 81);
    NAVI_LONGITUDE_INDI.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    NAVI_LONGITUDE_INDI.setLinespacing(0);
    Unicode::snprintf(NAVI_LONGITUDE_INDIBuffer, NAVI_LONGITUDE_INDI_SIZE, "%s", touchgfx::TypedText(T_DATABOX_LONG_INDI).getText());
    NAVI_LONGITUDE_INDI.setWildcard(NAVI_LONGITUDE_INDIBuffer);
    NAVI_LONGITUDE_INDI.setTypedText(touchgfx::TypedText(T_WILDCARD_65PX));

    NAVI_TITLE.setXY(boxPosition[pos][0] + 5, boxPosition[pos][1] + 10);
    NAVI_TITLE.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    NAVI_TITLE.setLinespacing(0);
    Unicode::snprintf(NAVI_TITLEBuffer, NAVI_TITLE_SIZE, "%s", touchgfx::TypedText(T_DATABOX_NAVI_TITLE).getText());
    NAVI_TITLE.setWildcard(NAVI_TITLEBuffer);
    NAVI_TITLE.resizeToCurrentText();
    NAVI_TITLE.setTypedText(touchgfx::TypedText(T_WILDCARD_25PX));
}

DataBoxNavi::DataBoxNavi()
{

}

void DataBoxNavi::CreateCanvas(DataboxViewBase& view)
{
    for(int i = 0; i < NAVI_WIDGET_MAX_COUNT; i++){
        widget[i].createWidget(i);
    }
}


double DataBoxNavi::GetPositionMinRound(double value){
    return GetRound(value, 1000.0f);
}

void DataBoxNavi::degreeToDM(double degree, int *getDegreeInt, double *getMinute)
{
    degree = fabs(degree);

    int degreeInt = (int)degree;
    double min = GetPositionMinRound( GetRound((degree - degreeInt) * 60, 10000.0) );

    int minOver = (int)(min / 60);
    if(minOver != 0) {
        degreeInt += minOver;
        min -= (int)min;
    }

    *getDegreeInt = abs(degreeInt);
    *getMinute = fabs(min);
}

void DataBoxNavi::GetDegreeToDMString(double degree, touchgfx::Unicode::UnicodeChar* formattedString, int stringSize)
{
    const int BUF_SIZE = 16;

    touchgfx::Unicode::UnicodeChar degreeIntString[BUF_SIZE];
    touchgfx::Unicode::UnicodeChar degreeMinIntString[BUF_SIZE];
    touchgfx::Unicode::UnicodeChar degreeMinDecString[BUF_SIZE];

    int degreeInt = 0;
    double min = 0;

    degreeToDM(degree, &degreeInt, &min);

    Unicode::snprintf(degreeIntString, BUF_SIZE, "%d%s", degreeInt, touchgfx::TypedText(T_DEGREE_65PX).getText());
    Unicode::snprintfFloat(degreeMinIntString, BUF_SIZE, "%02.f", min);
    Unicode::snprintfFloat(degreeMinDecString, BUF_SIZE, "%.3f", min - floor(min));

    Unicode::snprintf(formattedString, stringSize, "%s%s%s'", degreeIntString, degreeMinIntString, degreeMinDecString+1);
}

void DataBoxNavi::updatePosition(double latitudeDegree, double longitudeDegree)
{
    touchgfx::Unicode::UnicodeChar NAVI_LONGITUDE_INDIBuffer[NaviWidget::NAVI_LONGITUDE_INDI_SIZE];
    touchgfx::Unicode::UnicodeChar NAVI_LONGITUDEBuffer[NaviWidget::NAVI_LONGITUDE_SIZE];
    touchgfx::Unicode::UnicodeChar NAVI_LATITUDE_INDIBuffer[NaviWidget::NAVI_LATITUDE_INDI_SIZE];
    touchgfx::Unicode::UnicodeChar NAVI_LATITUDEBuffer[NaviWidget::NAVI_LATITUDE_SIZE];

    if(isPositionValid(latitudeDegree, longitudeDegree) == false){
        Unicode::snprintf(NAVI_LATITUDEBuffer, NaviWidget::NAVI_LATITUDE_SIZE, "--%s--.---'-", touchgfx::TypedText(T_DEGREE_65PX).getText());
        Unicode::snprintf(NAVI_LONGITUDEBuffer, NaviWidget::NAVI_LONGITUDE_SIZE, "---%s--.---'-", touchgfx::TypedText(T_DEGREE_65PX).getText());

        Unicode::snprintf(NAVI_LATITUDE_INDIBuffer, NaviWidget::NAVI_LATITUDE_INDI_SIZE, "");
        Unicode::snprintf(NAVI_LONGITUDE_INDIBuffer, NaviWidget::NAVI_LONGITUDE_INDI_SIZE, "");
    }
    else {
        GetDegreeToDMString(latitudeDegree, NAVI_LATITUDEBuffer, NaviWidget::NAVI_LATITUDE_SIZE);
        GetDegreeToDMString(longitudeDegree, NAVI_LONGITUDEBuffer, NaviWidget::NAVI_LONGITUDE_SIZE);

        Unicode::snprintf(NAVI_LATITUDE_INDIBuffer, NaviWidget::NAVI_LATITUDE_INDI_SIZE, (latitudeDegree < 0) ? "S" : "N");
        Unicode::snprintf(NAVI_LONGITUDE_INDIBuffer, NaviWidget::NAVI_LONGITUDE_INDI_SIZE, (longitudeDegree < 0) ? "W" : "E");
    }

    for(int i = 0; i < NAVI_WIDGET_MAX_COUNT; i++){
        Unicode::strncpy(widget[i].NAVI_LATITUDEBuffer, NAVI_LATITUDEBuffer, NaviWidget::NAVI_LATITUDE_SIZE);
        Unicode::strncpy(widget[i].NAVI_LONGITUDEBuffer, NAVI_LONGITUDEBuffer, NaviWidget::NAVI_LONGITUDE_SIZE);

        Unicode::strncpy(widget[i].NAVI_LATITUDE_INDIBuffer, NAVI_LATITUDE_INDIBuffer, NaviWidget::NAVI_LATITUDE_INDI_SIZE);
        Unicode::strncpy(widget[i].NAVI_LONGITUDE_INDIBuffer, NAVI_LONGITUDE_INDIBuffer, NaviWidget::NAVI_LONGITUDE_INDI_SIZE);

        widget[i].NAVI_LATITUDE.invalidate();
        widget[i].NAVI_LONGITUDE.invalidate();

        widget[i].NAVI_LATITUDE_INDI.invalidate();
        widget[i].NAVI_LONGITUDE_INDI.invalidate();
    }
}
