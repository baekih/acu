#include <gui/speed_screen/SpeedView.hpp>
#include <gui/compass_screen/CompassView.hpp>

#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#include <math.h>

#define SPEED_GAUGE_LINE_RADIUS  	257
#define SPEED_GAUGE_TEXT_RADIUS  	257

#define GUAGE_START_DEGREE 			(-120)

SpeedView::SpeedView() :
	type(typeSTW)
{
	setType(type);

    for(int index = 0; index < 41; index++){

		shapeGuageLine[index].setScale(1.0f, 1.0f);
		shapeGuageLine[index].setAngle(0.0f);
		shapeGuagePainter[index].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
		shapeGuageLine[index].setPainter(shapeGuagePainter[index]);

		if((index % 10) == 0){
			shapeGuageLine[index].setPosition(0, 0, 30, 30);
			shapeGuageLine[index].setOrigin(15.0f, 15.0f);
			const touchgfx::AbstractShape::ShapePoint<float> shapePoints[4] = { { -2.0f, -15.0f }, { 2.0f, -15.0f }, { 2.0f, 15.0f }, { -2.0f, 15.0f } };
			shapeGuageLine[index].setShape(shapePoints);
		}
		else if((index % 5) == 0){
			shapeGuageLine[index].setPosition(0, 0, 20, 20);
			shapeGuageLine[index].setOrigin(10.0f, 10.0f);
			const touchgfx::AbstractShape::ShapePoint<float> shapePoints[4] = { { -2.0f, -10.0f }, { 2.0f, -10.0f }, { 2.0f, 10.0f }, { -2.0f, 10.0f } };
			shapeGuageLine[index].setShape(shapePoints);
		}
		else {
			shapeGuageLine[index].setPosition(0, 0, 20, 20);
			shapeGuageLine[index].setOrigin(10.0f, 10.0f);
			const touchgfx::AbstractShape::ShapePoint<float> shapePoints[4] = { { -1.0f, -10.0f }, { 1.0f, -10.0f }, { 1.0f, 10.0f }, { -1.0f, 10.0f } };
			shapeGuageLine[index].setShape(shapePoints);
		}

		add(shapeGuageLine[index]);

		drawSpeedGaugeLine(GUAGE_START_DEGREE, (index * 6), shapeGuageLine[index]);
	}

	int guageTextEnum[12] = {
			T_TEXT_SPEED_0,
			T_TEXT_SPEED_5,
			T_TEXT_SPEED_10,
			T_TEXT_SPEED_15,
			T_TEXT_SPEED_20,
	};

	for(int text = 0; text < 5; text++){
		textGaugeSpeed[text].setXY(0, 0);
		textGaugeSpeed[text].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
		textGaugeSpeed[text].setLinespacing(0);
		textGaugeSpeed[text].setTypedText(touchgfx::TypedText(guageTextEnum[text]));
		add(textGaugeSpeed[text]);

		drawSpeedGaugeText(GUAGE_START_DEGREE, text * 60, textGaugeSpeed[text]);
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


void SpeedView::drawSpeedGaugeLine(float degree, int offset, touchgfx::Shape<4>& shape)
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


void SpeedView::drawSpeedGaugeText(float degree, int offset, touchgfx::TextArea& text)
{
	Point screenEnd = calcPixelGauge.getPointByDistanceBearing(degree + offset);

	Point intersectXY = calcPixelGauge.getCrossPointInCircle(SPEED_GAUGE_TEXT_RADIUS-((text.getWidth() / 2) + 20), screenEnd.x, screenEnd.y);

	text.moveTo(intersectXY.x - (text.getWidth() / 2), intersectXY.y - (text.getHeight() / 2));
	text.invalidate();
}

void SpeedView::drawSpeedGaugeIndicator(float value, int max)
{
	const float MAX_DEGREE = 240.0f;

	float value_ratio = value / (float)max;
	float degree = (float)GUAGE_START_DEGREE + (MAX_DEGREE * value_ratio);

	Point screenEnd = calcPixelIndicator.getPointByDistanceBearing(degree);

	Point intersectXY = calcPixelIndicator.getCrossPointInCircle(SPEED_GAUGE_LINE_RADIUS - (SPEED_INDICATOR.getWidth()/2) + 14, screenEnd.x, screenEnd.y);

	SPEED_INDICATOR.moveTo(intersectXY.x - SPEED_INDICATOR.getWidth()/2, intersectXY.y - SPEED_INDICATOR.getHeight()/2);
	SPEED_INDICATOR.setAngle(degree);
	SPEED_INDICATOR.invalidate();
}

void SpeedView::updateSTW(float stwValue)
{
	if(type == typeSTW){
		updateSpeed(stwValue, 20);
	}
}

void SpeedView::updateSOG(float sogValue)
{
	if(type == typeSOG){
		updateSpeed(sogValue, 20);
	}
}

void SpeedView::updateSpeed(float value, int max)
{
	Unicode::snprintfFloat(SPEED_VALUEBuffer, SPEED_VALUE_SIZE, "%.1f", value);

    SPEED_VALUE.invalidate();

    drawSpeedGaugeIndicator(value, max);
}

void SpeedView::updateDepth(float depthValue)
{
	if(depthValue < 100){
		Unicode::snprintfFloat(DEPTH_VALUEBuffer, DEPTH_VALUE_SIZE, "%.1f", depthValue);
	}
	else{
		Unicode::snprintf(DEPTH_VALUEBuffer, DEPTH_VALUE_SIZE, "%d", (int)depthValue);
	}

	DEPTH_VALUE.invalidate();
}

void SpeedView::updateWTemp(float value)
{
	Unicode::snprintfFloat(WTEMP_VALUEBuffer, WTEMP_VALUE_SIZE, "%.1f", value);

	WTEMP_VALUE.invalidate();
}

void SpeedView::setType(int new_type)
{
	if(new_type == typeSTW){
		type = new_type;
		Unicode::snprintf(SPEED_TITLEBuffer, SPEED_TITLE_SIZE, "STW");
	}
	else if(new_type == typeSOG){
		type = new_type;
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
    	if(abs(pressedY - y) < 40 && (pressedY > x) && (pressedY - x) > 200){
			static_cast<FrontendApplication*>(Application::getInstance())->gotoCompassScreenNoTransition();
		}
		else if(abs(pressedY - y) < 40 && (x > pressedY)  && (x - pressedY) > 200){
			if(type == typeSTW){
				setType(typeSOG);
			}
			else if(type == typeSOG){
				setType(typeSTW);
			}
		}
    }
}


void SpeedView::handleDragEvent(const DragEvent& evt)
{

}

void SpeedView::handleTickEvent()
{
/*	static int stw = 0;

	stw++;

	if((stw % 10) == 0){
		updateSTW((float)stw / 100);

		if(stw >= 2000){
			stw = 0;
		}
	}*/

	updateSTW(getSTWValue(SPEED_UNIT_KNOT));
	updateSOG(getSOGValue(SPEED_UNIT_KNOT));
	updateDepth(getDepthValue(DEPTH_UNIT_METER));
	updateWTemp(getWTempValue(UNIT_TEMP_CELSIUS));
}

