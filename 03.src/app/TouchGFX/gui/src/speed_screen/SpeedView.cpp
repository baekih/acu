#include <gui/speed_screen/SpeedView.hpp>
#include <gui/compass_screen/CompassView.hpp>

#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#include <math.h>

#include "printf.h"

#define SPEED_GAUGE_LINE_RADIUS     257
#define SPEED_GAUGE_TEXT_RADIUS     257

#define GUAGE_START_DEGREE          (-120)

SpeedView::SpeedView() :
    speedType(typeSOG)
{
    setSpeedType(speedType);

    for(int index = 0; index < 51; index++){

        shapeGuageLine[index].setScale(1.0f, 1.0f);
        shapeGuageLine[index].setAngle(0.0f);
        shapeGuagePainter[index].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
        shapeGuageLine[index].setPainter(shapeGuagePainter[index]);

        if((index % 10) == 0){
            shapeGuageLine[index].setPosition(0, 0, 30, 30);
            shapeGuageLine[index].setOrigin(15.0f, 15.0f);
            const touchgfx::AbstractShape::ShapePoint<double> shapePoints[4] = { { -2.0f, -15.0f }, { 2.0f, -15.0f }, { 2.0f, 15.0f }, { -2.0f, 15.0f } };
            shapeGuageLine[index].setShape(shapePoints);
        }
        else {
            shapeGuageLine[index].setPosition(0, 0, 20, 20);
            shapeGuageLine[index].setOrigin(10.0f, 10.0f);
            const touchgfx::AbstractShape::ShapePoint<double> shapePoints[4] = { { -1.0f, -10.0f }, { 1.0f, -10.0f }, { 1.0f, 10.0f }, { -1.0f, 10.0f } };
            shapeGuageLine[index].setShape(shapePoints);
        }

        add(shapeGuageLine[index]);

        drawSpeedGaugeLine(GUAGE_START_DEGREE, (index * 24 / 5), shapeGuageLine[index]);
    }

    int guageTextEnum[12] = {
            T_TEXT_SPEED_0,
            T_TEXT_SPEED_10,
            T_TEXT_SPEED_20,
            T_TEXT_SPEED_30,
            T_TEXT_SPEED_40,
            T_TEXT_SPEED_50,
    };

    for(int text = 0; text < 6; text++){
        textGaugeSpeed[text].setXY(0, 0);
        textGaugeSpeed[text].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
        textGaugeSpeed[text].setLinespacing(0);
        textGaugeSpeed[text].setTypedText(touchgfx::TypedText(guageTextEnum[text]));
        add(textGaugeSpeed[text]);

        drawSpeedGaugeText(GUAGE_START_DEGREE, text * 48, textGaugeSpeed[text]);
    }

    remove(SPEED_INDICATOR);
    add(SPEED_INDICATOR);

    remove(INDICATOR_GUIDE);
    add(INDICATOR_GUIDE);

    remove(TEXT_LINE);
    add(TEXT_LINE);

    remove(SPEED_TITLE);
    add(SPEED_TITLE);

    remove(SPEED_VALUE);
    add(SPEED_VALUE);
}

void SpeedView::setupScreen()
{
    SpeedViewBase::setupScreen();
}

void SpeedView::tearDownScreen()
{
    SpeedViewBase::tearDownScreen();
}


void SpeedView::drawSpeedGaugeLine(double degree, int offset, touchgfx::Shape<4>& shape)
{
    Point screenEnd = calcPixelGauge.getPointByDistanceBearing(degree + offset);

    int radius;

    if((offset % 20) == 0) radius = SPEED_GAUGE_LINE_RADIUS;
    else radius = SPEED_GAUGE_LINE_RADIUS + 5;

    Point intersectXY = calcPixelGauge.getCrossPointInCircle(radius, screenEnd.x, screenEnd.y);

    shape.moveTo(intersectXY.x - shape.getWidth()/2, intersectXY.y - shape.getHeight()/2);
    shape.setAngle(degree + offset);
    shape.invalidate();
}


void SpeedView::drawSpeedGaugeText(double degree, int offset, touchgfx::TextArea& text)
{
    Point screenEnd = calcPixelGauge.getPointByDistanceBearing(degree + offset);

    Point intersectXY = calcPixelGauge.getCrossPointInCircle(SPEED_GAUGE_TEXT_RADIUS-((text.getWidth() / 2) + 20), screenEnd.x, screenEnd.y);

    text.moveTo(intersectXY.x - (text.getWidth() / 2), intersectXY.y - (text.getHeight() / 2));
    text.invalidate();
}

void SpeedView::drawSpeedGaugeIndicator(double value, int max)
{
    const double MAX_DEGREE = 240.0f;

    double value_ratio = value / (double)max;

    if(value > max) value_ratio = 1;

    double degree = (double)GUAGE_START_DEGREE + (MAX_DEGREE * value_ratio);

    Point screenEnd = calcPixelIndicator.getPointByDistanceBearing(degree);

    Point intersectXY = calcPixelIndicator.getCrossPointInCircle(SPEED_GAUGE_LINE_RADIUS - (SPEED_INDICATOR.getWidth()/2) + 14, screenEnd.x, screenEnd.y);

    SPEED_INDICATOR.moveTo(intersectXY.x - SPEED_INDICATOR.getWidth()/2, intersectXY.y - SPEED_INDICATOR.getHeight()/2);
    SPEED_INDICATOR.setAngle(degree);
    SPEED_INDICATOR.invalidate();
}

void SpeedView::updateSTW(double stwValue)
{
    if(speedType == typeSTW){
        updateSpeed(stwValue, 50);
    }
}

void SpeedView::updateSOG(double sogValue)
{
    if(speedType == typeSOG){
        updateSpeed(sogValue, 50);
    }
}

void SpeedView::updateSpeed(double value, int max)
{
    if(value < speedDisplayMin || value > speedDisplayMax){
        Unicode::snprintf(SPEED_VALUEBuffer, SPEED_VALUE_SIZE, SPEED_OUT_OF_RANGE);

        drawSpeedGaugeIndicator(max, max);
    }
    else{
        double speed = GetRound(value, 10.0);

        if(speed < 100){
            Unicode::snprintfFloat(SPEED_VALUEBuffer, SPEED_VALUE_SIZE, "%.1f", speed);
        }
        else {
            Unicode::snprintf(SPEED_VALUEBuffer, SPEED_VALUE_SIZE, "%d", (int)speed);
        }

        drawSpeedGaugeIndicator(value, max);
    }

    SPEED_VALUE.invalidate();
}

void SpeedView::updateDepth(double value)
{
    if(value < depthDisplayMin || value > depthDisplayMeterMax){
        Unicode::snprintf(DEPTH_VALUEBuffer, DEPTH_VALUE_SIZE, OUT_OF_RANGE);
    }
    else {
        double depthValue = GetRound(value, 10.0);

        if(depthValue < 100){
            Unicode::snprintfFloat(DEPTH_VALUEBuffer, DEPTH_VALUE_SIZE, "%.1f", depthValue);
        }
        else{
            Unicode::snprintf(DEPTH_VALUEBuffer, DEPTH_VALUE_SIZE, "%d", (int)depthValue);
        }
    }

    DEPTH_VALUE.invalidate();
}

void SpeedView::updateWTemp(double value)
{
    double tempC = GetRound(value, 10.0);

    const char MAX_TEMP_VALUE[] = "*99.9";
    const char MIN_TEMP_VALUE[] = "-*9.9";

    if(tempC > tempCelsiusDisplayMax){
        Unicode::snprintf(WTEMP_VALUEBuffer, WTEMP_VALUE_SIZE, MAX_TEMP_VALUE);
    }
    else if(tempC < tempCelsiusDisplayMin){
        Unicode::snprintf(WTEMP_VALUEBuffer, WTEMP_VALUE_SIZE, MIN_TEMP_VALUE);
    }
    else{
        Unicode::snprintfFloat(WTEMP_VALUEBuffer, WTEMP_VALUE_SIZE, "%.1f", tempC);
    }

    WTEMP_VALUE.invalidate();
}

void SpeedView::setSpeedType(int new_type)
{
    if(new_type == typeSTW){
        speedType = new_type;
        Unicode::snprintf(SPEED_TITLEBuffer, SPEED_TITLE_SIZE, "STW");
    }
    else if(new_type == typeSOG){
        speedType = new_type;
        Unicode::snprintf(SPEED_TITLEBuffer, SPEED_TITLE_SIZE, "SOG");
    }

    SPEED_TITLE.invalidate();
}

void SpeedView::handleClickEvent(const ClickEvent& evt)
{
    int x = evt.getX();
    int y = evt.getY();

    if (evt.getType() == ClickEvent::PRESSED)
    {
        pressedX = x;
        pressedY = y;
    }
    else if (evt.getType() == ClickEvent::RELEASED)
    {
        if((pressedX < 100) && (pressedX - x > 20)){
            static_cast<FrontendApplication*>(Application::getInstance())->gotoLogBoxScreenNoTransition();
        }
        else if((200 < x) && (x < 400) && (200 < y) && (y < 400)){
            if(speedType == typeSTW){
                setSpeedType(typeSOG);
            }
            else if(speedType == typeSOG){
                setSpeedType(typeSTW);
            }
        }
    }
}


void SpeedView::handleDragEvent(const DragEvent& evt)
{

}

void SpeedView::handleTickEvent()
{
#ifdef SIMULATOR
    static int stw = 0;

    stw++;

    if((stw % 10) == 0){
        updateSTW((double)stw / 100);

        if(stw >= 2000){
            stw = 0;
        }
    }
#else

    updateSTW(getSTWValue(SPEED_UNIT_KNOT));
    updateSOG(getSOGValue(SPEED_UNIT_KNOT));
    updateDepth(getDepthValue(DEPTH_UNIT_METER));
    updateWTemp(getWTempValue(UNIT_TEMP_CELSIUS));

#endif
}
