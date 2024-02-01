#include <gui/compass_screen/CompassView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

/*
#include "stm32f7xx_hal.h"
#include "printf.h"
*/

#define HEADING_LINE_RADIUS  	410
#define HEADING_TEXT_RADIUS  	410

CompassView::CompassView()
{
    Unicode::snprintf(HDG_VALUEBuffer, HDG_VALUE_SIZE, "000");

    for(int deg = 0; deg < 360; deg += 5){

    	int line = deg / 5;

		shapeCompassLine[line].setScale(1.0f, 1.0f);
		shapeCompassLine[line].setAngle(0.0f);
		shapeCompassPainter[line].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
		shapeCompassLine[line].setPainter(shapeCompassPainter[line]);

    	if((deg % 30) == 0){
			shapeCompassLine[line].setPosition(0, 0, 40, 40);
			shapeCompassLine[line].setOrigin(20.0f, 20.0f);
			const touchgfx::AbstractShape::ShapePoint<float> shapePoints[4] = { { -2.0f, -20.0f }, { 2.0f, -20.0f }, { 2.0f, 20.0f }, { -2.0f, 20.0f } };
			shapeCompassLine[line].setShape(shapePoints);
    	}
    	else if((deg % 10) == 0){
    		shapeCompassLine[line].setPosition(0, 0, 20, 20);
    		shapeCompassLine[line].setOrigin(10.0f, 10.0f);
			const touchgfx::AbstractShape::ShapePoint<float> shapePoints[4] = { { -2.0f, -10.0f }, { 2.0f, -10.0f }, { 2.0f, 10.0f }, { -2.0f, 10.0f } };
			shapeCompassLine[line].setShape(shapePoints);
    	}
    	else {
    		shapeCompassLine[line].setPosition(0, 0, 10, 10);
    		shapeCompassLine[line].setOrigin(5.0f, 5.0f);
			const touchgfx::AbstractShape::ShapePoint<float> shapePoints[4] = { { -1.0f, -5.0f }, { 1.0f, -5.0f }, { 1.0f, 5.0f }, { -1.0f, 5.0f } };
			shapeCompassLine[line].setShape(shapePoints);
    	}

    	add(shapeCompassLine[line]);

    	drawBearingLine(0, deg, shapeCompassLine[line]);
    }

    int degreeTextEnum[12] = {
    		T_TEXT_COMPASS_0,
			T_TEXT_COMPASS_30,
		    T_TEXT_COMPASS_60,
		    T_TEXT_COMPASS_90,
		    T_TEXT_COMPASS_120,
		    T_TEXT_COMPASS_150,
		    T_TEXT_COMPASS_180,
		    T_TEXT_COMPASS_210,
		    T_TEXT_COMPASS_240,
		    T_TEXT_COMPASS_270,
		    T_TEXT_COMPASS_300,
		    T_TEXT_COMPASS_330
    };

    for(int text = 0; text < 12; text++){
    	textComassDegree[text].setXY(0, 0);
    	textComassDegree[text].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    	textComassDegree[text].setLinespacing(0);
    	textComassDegree[text].setTypedText(touchgfx::TypedText(degreeTextEnum[text]));
		add(textComassDegree[text]);

		drawBearingText(0, text * 30, textComassDegree[text]);
    }
}

void CompassView::setupScreen()
{
    CompassViewBase::setupScreen();
}

void CompassView::tearDownScreen()
{
    CompassViewBase::tearDownScreen();
}

void CompassView::drawBearingLine(float degree, int offset, touchgfx::Shape<4>& shape)
{
	Point screenEnd = calcPixel.getPointByDistanceBearing(degree + offset);

	int radius;

	if((offset % 30) == 0) radius = HEADING_LINE_RADIUS;
	else if((offset % 10) == 0) radius = HEADING_LINE_RADIUS + 10;
	else radius = HEADING_LINE_RADIUS + 15;

	Point intersectXY = calcPixel.getCrossPointInCircle(radius, screenEnd.x, screenEnd.y);

	shape.moveTo(intersectXY.x - shape.getWidth()/2, intersectXY.y - shape.getHeight()/2);
	shape.setAngle(degree + offset);
	shape.invalidate();
}


void CompassView::drawBearingText(float degree, int offset, touchgfx::TextArea& text)
{
	Point screenEnd = calcPixel.getPointByDistanceBearing(degree + offset);

	Point intersectXY = calcPixel.getCrossPointInCircle(HEADING_TEXT_RADIUS-((text.getWidth() / 2) + 30), screenEnd.x, screenEnd.y);

	text.moveTo(intersectXY.x - (text.getWidth() / 2), intersectXY.y - text.getHeight() / 2);
	text.invalidate();
}

void CompassView::updateBearingLine(float degree)
{
    for(int deg = 0; deg < 360; deg += 5){

    	int line = deg / 5;

    	drawBearingLine(degree * -1, deg, shapeCompassLine[line]);
    }

    for(int text = 0; text < 12; text++){
   		drawBearingText(degree * -1, text * 30, textComassDegree[text]);
    }
}
void CompassView::updateHDG(float hdgValue)
{
	int heading = ((int)(hdgValue)) % 360;

    Unicode::snprintf(HDG_VALUEBuffer, HDG_VALUE_SIZE, "%d", heading);

    HDG_VALUE.invalidate();

    updateBearingLine(hdgValue);

}

void CompassView::handleClickEvent(const ClickEvent& evt)
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
        if(abs(pressedY - y) < 20 && (pressedY - x) > 200){
        	static_cast<FrontendApplication*>(Application::getInstance())->gotoSpeedScreenNoTransition();
        }
    }
}

void CompassView::handleDragEvent(const DragEvent& evt)
{

}

void CompassView::handleTickEvent()
{
/*	static int count = 0;
	static int hdg = 0;
	static int prev = 0;

	count++;
	hdg = count;

	if(prev != hdg) {
    	updateHDG((float)hdg / 10);
    	prev = hdg;
	}*/

//	updateHDG(getHDGValue());
}


