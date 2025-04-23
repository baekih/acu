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
    Point getPointByDistanceXYBearing(double distance, double baseX, double baseY, double heading);
    Point getPointByCross(double centerX, double centerY, Point solution[], double originX, double originY, double endX, double endY);

public:    CalcPixel(int x, int y, int width);
    virtual ~CalcPixel() {}
    double adjustAngleDegree(double angle);

    Point getPointByDistanceBearing( double heading);
    Point getCrossPointInCircle(double radius, double endX, double endY);
};
#endif /* GUI_INCLUDE_GUI_COMMON_CALCPIXEL_HPP_ */
