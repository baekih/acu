/*
 * CalcPixel.hpp
 *
 *  Created on: Jan 30, 2024
 *      Author: DRYOON
 */

#ifndef GUI_INCLUDE_GUI_COMMON_CALCPIXEL_HPP_
#define GUI_INCLUDE_GUI_COMMON_CALCPIXEL_HPP_

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>

class CalcPixel
{
	int centerX;
	int centerY;

	int outerWidth;
private :
	Point getPointByDistanceXYBearing(float distance, float baseX, float baseY, float heading);
	Point getPointByCross(float centerX, float centerY, Point solution[], float originX, float originY, float endX, float endY);

public:	CalcPixel(int x, int y, int width);
    virtual ~CalcPixel() {}
    float adjustAngleDegree(float angle);

    Point getPointByDistanceBearing( float heading);
    Point getCrossPointInCircle(float radius, float endX, float endY);
};
#endif /* GUI_INCLUDE_GUI_COMMON_CALCPIXEL_HPP_ */
