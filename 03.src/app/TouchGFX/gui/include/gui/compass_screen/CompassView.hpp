#ifndef COMPASSVIEW_HPP
#define COMPASSVIEW_HPP

#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/canvas/Shape.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>

#include <gui_generated/compass_screen/CompassViewBase.hpp>
#include <gui/compass_screen/CompassPresenter.hpp>

class CompassView : public CompassViewBase
{
public:
    CompassView();
    virtual ~CompassView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateHDG(int hdgValue);

    Point getPointByDistanceXYBearing(double distance, double baseX, double baseY, double heading);
    Point getPointByDistanceBearing(double distance, double heading);
    Point getPointByCross(double centerX, double centerY, Point solution[], double originX, double originY, double endX, double endY);
    Point getCrossPointInCircle(double circleX, double circleY, double radius, double startX, double startY, double endX, double endY);

    void drawBearingLine(double degree, int offset, touchgfx::Shape<4>& line);
    void drawBearingText(double degree, int offset, touchgfx::TextArea& text);
    void updateBearingLine(double degree);

    void handleTickEvent();
protected:
    touchgfx::Shape<4> shapeCompassLine[36];
    touchgfx::PainterRGB565 shapeCompassPainter[36];

    touchgfx::TextArea textComassDegree[12];
};

#endif // COMPASSVIEW_HPP
