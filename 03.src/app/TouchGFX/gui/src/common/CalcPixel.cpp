#include <gui/common/CalcPixel.hpp>
#include <math.h>

#define toRadians(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define toDegree(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

CalcPixel::CalcPixel(int x, int y, int width) :
	centerX(x),
	centerY(y),
	outerWidth(width)
{

}

float CalcPixel::adjustAngleDegree(float angle)
{
	float res = angle;

	if (res >= 360.0) {
		res = res - 360.0;
	} else if (res < 0) {
		res = 360.0 + res;
	}

	if (res == 360) {
		res = 0;
	}

	return res;
}

Point CalcPixel::getPointByDistanceXYBearing(float distance, float baseX, float baseY, float heading)
{
	float dSetDegree = toRadians(heading);
	Point point;

	point.x = (float)baseX + (float)( distance * sin(dSetDegree)); // 결과 좌표 x
	point.y = (float)baseY - (float)( distance * cos(dSetDegree)); // 결과 좌표 y

	return point;
}

Point CalcPixel::getPointByCross(float centerX, float centerY, Point solution[], float originX, float originY, float endX, float endY)
{
	Point result;

	float bigX;
	float smallX;

	float bigY;
	float smallY;

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

Point CalcPixel::getPointByDistanceBearing(float heading)
{
	return getPointByDistanceXYBearing(outerWidth, centerX, centerY, heading);
}


Point CalcPixel::getCrossPointInCircle( float radius, float endX, float endY)
{
	float m, n;

	float circleX = centerX;
	float circleY = centerY;

	float startX = centerX;
	float startY = centerY;

	Point solution[2] = { {-1, -1}, {-1, -1} };
	Point result = {-1, -1};

	float A, B1, C, D;
	float X, Y;

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
			float sqrt_X = sqrt(radius * radius - (startX - circleX) * (startX - circleX));

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
