#include <gui/compass_screen/CompassView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#include <gui/common/validate_data.h>

#ifndef SIMULATOR
#include "printf.h"
#endif

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
			const touchgfx::AbstractShape::ShapePoint<double> shapePoints[4] = { { -2.0f, -20.0f }, { 2.0f, -20.0f }, { 2.0f, 20.0f }, { -2.0f, 20.0f } };
			shapeCompassLine[line].setShape(shapePoints);
    	}
    	else if((deg % 10) == 0){
    		shapeCompassLine[line].setPosition(0, 0, 20, 20);
    		shapeCompassLine[line].setOrigin(10.0f, 10.0f);
			const touchgfx::AbstractShape::ShapePoint<double> shapePoints[4] = { { -2.0f, -10.0f }, { 2.0f, -10.0f }, { 2.0f, 10.0f }, { -2.0f, 10.0f } };
			shapeCompassLine[line].setShape(shapePoints);
    	}
    	else {
    		shapeCompassLine[line].setPosition(0, 0, 10, 10);
    		shapeCompassLine[line].setOrigin(5.0f, 5.0f);
			const touchgfx::AbstractShape::ShapePoint<double> shapePoints[4] = { { -1.0f, -5.0f }, { 1.0f, -5.0f }, { 1.0f, 5.0f }, { -1.0f, 5.0f } };
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

    remove(NORTH_MARK);
    add(NORTH_MARK);
}

void CompassView::setupScreen()
{
    CompassViewBase::setupScreen();
}

void CompassView::tearDownScreen()
{
    CompassViewBase::tearDownScreen();
}

void CompassView::drawBearingLine(double degree, int offset, touchgfx::Shape<4>& shape)
{
	Point screenEnd = calcPixel.getPointByDistanceBearing(degree + offset);

	int radius;

	if((offset % 30) == 0) radius = HEADING_LINE_RADIUS;
	else if((offset % 10) == 0) radius = HEADING_LINE_RADIUS + 10;
	else radius = HEADING_LINE_RADIUS + 15;

	Point intersectXY = calcPixel.getCrossPointInCircle(radius, screenEnd.x, screenEnd.y);

	shape.setAlpha(255);
	shape.moveTo(intersectXY.x - shape.getWidth()/2, intersectXY.y - shape.getHeight()/2);
	shape.setAngle(degree + offset);
	shape.invalidate();
}


void CompassView::drawBearingText(double degree, int offset, touchgfx::TextArea& text)
{
	Point screenEnd = calcPixel.getPointByDistanceBearing(degree + offset);

	Point intersectXY = calcPixel.getCrossPointInCircle(HEADING_TEXT_RADIUS-((text.getWidth() / 2) + 30), screenEnd.x, screenEnd.y);

	text.setAlpha(255);
	text.moveTo(intersectXY.x - (text.getWidth() / 2), intersectXY.y - text.getHeight() / 2);
	text.invalidate();
}

void CompassView::hideCompassLine()
{
	for(int line = 0; line < 72; line ++){
		shapeCompassLine[line].setAlpha(0);
		shapeCompassLine[line].invalidate();
	}

	for(int text = 0; text < 12; text++){
		textComassDegree[text].setAlpha(0);
		textComassDegree[text].invalidate();
	}
}

void CompassView::updateBearingLine(double degree)
{
    for(int deg = 0; deg < 360; deg += 5){

    	int line = deg / 5;

    	drawBearingLine(degree * -1, deg, shapeCompassLine[line]);
    }

    for(int text = 0; text < 12; text++){
   		drawBearingText(degree * -1, text * 30, textComassDegree[text]);

    }
}
void CompassView::updateHDG(double hdgValue)
{
    if(isTimeInHDG() && isValidHDG()){
    	Unicode::snprintf(HDG_VALUEBuffer, HDG_VALUE_SIZE, "%d", (int)adjustDisplayAngleDegree(GetRound(hdgValue, 1)));
    	updateBearingLine(hdgValue);
    }
    else {
    	Unicode::snprintf(HDG_VALUEBuffer, HDG_VALUE_SIZE, "---");
    	hideCompassLine();
    }

    HDG_VALUE.invalidate();
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
    	if((pressedY > x) && (pressedY - x) > 200){
        	static_cast<FrontendApplication*>(Application::getInstance())->gotoSpeedScreenNoTransition();
        }
    }
}

void CompassView::handleDragEvent(const DragEvent& evt)
{

}

void CompassView::handleTickEvent()
{
#ifdef SIMULATOR
//	touchgfx_printf("Application is running through simulator! \n");

	static int count = 698;

	updateHDG(((((double)(count) / 10000) / (2*M_PI)) * 360));

	count++;

	if(count > 1047){
		count = 698;
	}
#else
	updateHDG(getHDGValue());

#endif
}


