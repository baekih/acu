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

#include <gui/common/CalcPixel.hpp>
#include <gui/common/ViewCommon.hpp>

class CompassView : public CompassViewBase, protected ViewCommon
{
    touchgfx::Shape<4> shapeCompassLine[72];
    touchgfx::PainterRGB565 shapeCompassPainter[72];

    touchgfx::TextArea textComassDegree[12];

    CalcPixel calcPixel = CalcPixel(400, 440, 800);

public:
    CompassView();
    virtual ~CompassView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateHDG(double hdgValue);

    void drawBearingLine(double degree, int offset, touchgfx::Shape<4>& line);
    void drawBearingText(double degree, int offset, touchgfx::TextArea& text);
    void updateBearingLine(double degree);

    void hideCompassLine();

    void handleTickEvent();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
protected:

};

#endif // COMPASSVIEW_HPP
