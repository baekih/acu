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

double CalcPixel::adjustAngleDegree(double angle)
{
	double res = angle;

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

Point CalcPixel::getPointByDistanceXYBearing(double distance, double baseX, double baseY, double heading)
{
	double dSetDegree = toRadians(heading);
	Point point;

	point.x = (double)baseX + (double)( distance * sin(dSetDegree)); // 결과 좌표 x
	point.y = (double)baseY - (double)( distance * cos(dSetDegree)); // 결과 좌표 y

	return point;
}

Point CalcPixel::getPointByCross(double centerX, double centerY, Point solution[], double originX, double originY, double endX, double endY)
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

Point CalcPixel::getPointByDistanceBearing(double heading)
{
	return getPointByDistanceXYBearing(outerWidth, centerX, centerY, heading);
}


Point CalcPixel::getCrossPointInCircle( double radius, double endX, double endY)
{
	double m, n;

	double circleX = centerX;
	double circleY = centerY;

	double startX = centerX;
	double startY = centerY;

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
#ifdef SIMULATOR
			touchgfx_printf("error1\n");
#endif
			X = -B1/A;
			Y = m*X + n;

			solution[0].x = (double)X;
			solution[0].y = (double)Y;

			solution[1].x = (double)X;
			solution[1].y = (double)Y;
		}
		else if( D > 0 )
		{
			X = -(B1 + sqrt(D))/A;
			Y = m*X + n;

			solution[0].x = (double)X;
			solution[0].y = (double)Y;

			X = -(B1 - sqrt(D))/A;
			Y = m*X + n;

			solution[1].x = (double)X;
			solution[1].y = (double)Y;

			result = getPointByCross(circleX, circleY, solution, startX, startY, endX, endY);
		}
	}
	else
	{
		if( startX ==(circleX -radius) || startX ==(circleX +radius) ) // error
		{
#ifdef SIMULATOR
			touchgfx_printf("error2\n");
#endif
			X = startX;
			Y = circleY;

			solution[0].x = (double)X;
			solution[0].y = (double)Y;

			solution[1].x = (double)X;
			solution[1].y = (double)Y;
		}
		else if( startX > (circleX -radius) && startX < (circleX +radius) )
		{
			X = startX;
			double sqrt_X = sqrt(radius * radius - (startX - circleX) * (startX - circleX));

			Y = circleY + sqrt_X;

			solution[0].x = (double)X;
			solution[0].y = (double)Y;

			Y = circleY - sqrt_X;

			solution[1].x = (double)X;
			solution[1].y = (double)Y;

			result = getPointByCross(circleX, circleY, solution, startX, startY, endX, endY);
		}
	}

	return result;
}
