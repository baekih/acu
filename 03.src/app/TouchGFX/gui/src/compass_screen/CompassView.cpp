#include <gui/compass_screen/CompassView.hpp>
#include <math.h>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#define FULL_SCREEN_LENGTH 	800

#define HEADING_LINE_RADIUS  	410
#define HEADING_TEXT_RADIUS  	410

#define CENTER_X  	400
#define CENTER_Y 	440

#define toRadians(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define toDegree(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

Point CompassView::getPointByDistanceXYBearing(double distance, double baseX, double baseY, double heading)
{
	double dSetDegree = toRadians(heading);
	Point point;

	point.x = (float)baseX + (float)( distance * sin(dSetDegree)); // 결과 좌표 x
	point.y = (float)baseY - (float)( distance * cos(dSetDegree)); // 결과 좌표 y

	return point;
}

Point CompassView::getPointByDistanceBearing(double distance, double heading)
{
	return getPointByDistanceXYBearing(distance, CENTER_X, CENTER_Y, heading);
}

Point CompassView::getPointByCross(double centerX, double centerY, Point solution[], double originX, double originY, double endX, double endY)
{
	Point result;

	double bigX;
	double smallX;

	double bigY;
	double smallY;

	if(originX > endX) {
		bigX = originX;
		smallX = endX;
	}
	else {
		bigX = endX;
		smallX = originX;
	}

	if(originY > endY) {
		bigY = originY;
		smallY = endY;
	}
	else {
		bigY = endY;
		smallY = originY;
	}

	if(solution[0].x >= smallX && solution[0].x <= bigX &&
			solution[0].y >= smallY && solution[0].y <= bigY ){
		result = solution[0];
	}
	else if(solution[1].x >= smallX && solution[1].x <= bigX &&
			solution[1].y >= smallY && solution[1].y <= bigY ){
		result = solution[1];
	}
	else { // error
		result.x = -1;
		result.y = -1;
	}

	return result;
}

Point CompassView::getCrossPointInCircle(double circleX, double circleY, double radius, double startX, double startY, double endX, double endY)
{
	double m, n;

	Point solution[2] = { {-1, -1}, {-1, -1} };
	Point result = {-1, -1};

	double A, B1, C, D;
	double X, Y;

	if( endX != startX)
	{
		m = (endY - startY)/(endX - startX);
		n = (startY * endX - startX * endY)/(endX - startX);

		A = m*m + 1;
		B1= (m*n-m* circleY - circleX);
		C = (circleX * circleX + circleY * circleY - radius*radius + n*n - 2*n* circleY);
		D = B1*B1 - A*C;

		if( D == 0 ) // error
		{
			X = -B1/A;
			Y = m*X + n;

			solution[0].x = (float)X;
			solution[0].y = (float)Y;

			solution[1].x = (float)X;
			solution[1].y = (float)Y;
		}
		else if( D > 0 )
		{
			X = -(B1 + sqrt(D))/A;
			Y = m*X + n;

			solution[0].x = (float)X;
			solution[0].y = (float)Y;

			X = -(B1 - sqrt(D))/A;
			Y = m*X + n;

			solution[1].x = (float)X;
			solution[1].y = (float)Y;

			result = getPointByCross(circleX, circleY, solution, startX, startY, endX, endY);
		}
	}
	else
	{
		if( startX ==(circleX -radius) || startX ==(circleX +radius) ) // error
		{
			X = startX;
			Y = circleY;

			solution[0].x = (float)X;
			solution[0].y = (float)Y;

			solution[1].x = (float)X;
			solution[1].y = (float)Y;
		}
		else if( startX > (circleX -radius) && startX < (circleX +radius) )
		{
			X = startX;
			double sqrt_X = sqrt(radius * radius - (startX - circleX) * (startX - circleX));

			Y = circleY + sqrt_X;

			solution[0].x = (float)X;
			solution[0].y = (float)Y;

			Y = circleY - sqrt_X;

			solution[1].x = (float)X;
			solution[1].y = (float)Y;

			result = getPointByCross(circleX, circleY, solution, startX, startY, endX, endY);
		}
	}

	return result;
}

void CompassView::drawBearingLine(double degree, int offset, touchgfx::Shape<4>& shape)
{
	Point screenEnd = getPointByDistanceBearing(FULL_SCREEN_LENGTH, degree + offset);

	int radius = HEADING_LINE_RADIUS;

	if((offset % 10) == 0) radius += 10;
	if((offset % 30) == 0) radius -= 10;

	Point intersectXY = getCrossPointInCircle(CENTER_X, CENTER_Y, radius,
			CENTER_X, CENTER_Y, screenEnd.x, screenEnd.y);

	shape.moveTo(intersectXY.x - shape.getWidth()/2, intersectXY.y - shape.getHeight()/2);
	shape.setAngle(degree + offset);
	shape.invalidate();
}


void CompassView::drawBearingText(double degree, int offset, touchgfx::TextArea& text)
{
	Point screenEnd = getPointByDistanceBearing(FULL_SCREEN_LENGTH, degree + offset);

	Point intersectXY = getCrossPointInCircle(CENTER_X, CENTER_Y, HEADING_TEXT_RADIUS-((text.getWidth() / 2) + 30),
			CENTER_X, CENTER_Y, screenEnd.x, screenEnd.y);

	text.moveTo(intersectXY.x - (text.getWidth() / 2), intersectXY.y - text.getHeight() / 2);
	text.invalidate();
}

void CompassView::updateBearingLine(double degree)
{
    for(int line = 0; line < 36; line++){
    	drawBearingLine(degree, line * 10, shapeCompassLine[line]);
    }

    for(int text = 0; text < 12; text++){
   		drawBearingText(degree, text * 30, textComassDegree[text]);
    }
}

CompassView::CompassView()
{
    Unicode::snprintf(HDG_VALUE1Buffer, HDG_VALUE1_SIZE, "0", 0);
    Unicode::snprintf(HDG_VALUE2Buffer, HDG_VALUE2_SIZE, "0", 0);
    Unicode::snprintf(HDG_VALUE3Buffer, HDG_VALUE3_SIZE, "0", 0);

    for(int line = 0; line < 36; line++){
    	if((line % 3) == 0){
			shapeCompassLine[line].setPosition(0, 0, 40, 40);
			shapeCompassLine[line].setOrigin(20.0f, 20.0f);
			shapeCompassLine[line].setScale(1.0f, 1.0f);
			shapeCompassLine[line].setAngle(0.0f);
			shapeCompassPainter[line].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
			shapeCompassLine[line].setPainter(shapeCompassPainter[line]);
			const touchgfx::AbstractShape::ShapePoint<float> shapePoints[4] = { { -2.0f, -20.0f }, { 2.0f, -20.0f }, { 2.0f, 20.0f }, { -2.0f, 20.0f } };
			shapeCompassLine[line].setShape(shapePoints);
			add(shapeCompassLine[line]);
    	}
    	else {

    		shapeCompassLine[line].setPosition(83, 417, 20, 20);
    		shapeCompassLine[line].setOrigin(10.0f, 10.0f);
    		shapeCompassLine[line].setScale(1.0f, 1.0f);
    		shapeCompassLine[line].setAngle(0.0f);
    		shapeCompassPainter[line].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
			shapeCompassLine[line].setPainter(shapeCompassPainter[line]);
			const touchgfx::AbstractShape::ShapePoint<float> shapePoints[4] = { { -2.0f, -10.0f }, { 2.0f, -10.0f }, { 2.0f, 10.0f }, { -2.0f, 10.0f } };
			shapeCompassLine[line].setShape(shapePoints);
			add(shapeCompassLine[line]);
    	}

    	drawBearingLine(30, line*10, shapeCompassLine[line]);
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

		drawBearingText(30, text * 30, textComassDegree[text]);
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

void CompassView::updateHDG(int hdgValue)
{
	int heading = (hdgValue % 360) ;

	int heading1 = (int)(heading / 100);
	int heading2 = (int)((heading / 10) % 10);
	int heading3 = (int)(heading % 10);

    if(heading1 == 0) Unicode::snprintf(HDG_VALUE1Buffer, HDG_VALUE1_SIZE, "");
    else Unicode::snprintf(HDG_VALUE1Buffer, HDG_VALUE1_SIZE, "%d", heading1);

    if(heading1 == 0 && heading2 == 0) Unicode::snprintf(HDG_VALUE2Buffer, HDG_VALUE2_SIZE, "");
    else Unicode::snprintf(HDG_VALUE2Buffer, HDG_VALUE2_SIZE, "%d", heading2);

    Unicode::snprintf(HDG_VALUE3Buffer, HDG_VALUE3_SIZE, "%d", heading3);

    HDG_VALUE3.invalidate();
    HDG_VALUE2.invalidate();
    HDG_VALUE1.invalidate();

    updateBearingLine(hdgValue);

}

void CompassView::handleTickEvent()
{
	static int count = 0;
	static int hdg = 0;
	static int prev = 0;

	count++;
	hdg = count / 10;

	if(prev != hdg) {
    	updateHDG(hdg);
    	prev = hdg;
	}
}


