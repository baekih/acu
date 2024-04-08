#ifndef SPEEDVIEW_HPP
#define SPEEDVIEW_HPP

#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/canvas/Shape.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>

#include <gui_generated/speed_screen/SpeedViewBase.hpp>
#include <gui/speed_screen/SpeedPresenter.hpp>

#include <gui/common/CalcPixel.hpp>
#include <gui/common/ViewCommon.hpp>

class SpeedView : public SpeedViewBase, protected ViewCommon
{
	int speedType;

private :
    touchgfx::Shape<4> shapeGuageLine[41];
	touchgfx::PainterRGB565 shapeGuagePainter[41];

	touchgfx::TextArea textGaugeSpeed[5];

	CalcPixel calcPixelGauge = CalcPixel(290, 288, 800);
	CalcPixel calcPixelIndicator = CalcPixel(290, 290, 800);

private :
    void drawSpeedGaugeLine(double degree, int offset, touchgfx::Shape<4>& line);
    void drawSpeedGaugeText(double degree, int offset, touchgfx::TextArea& text);
    void drawSpeedGaugeIndicator(double value, int max);

    void updateSpeed(double value, int max);
public:
    SpeedView();
    virtual ~SpeedView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateSTW(double value);
    void updateSOG(double value);

    void updateDepth(double value);
    void updateWTemp(double value);

    void setSpeedType(int new_type);
    int getSpeedType();

    void handleTickEvent();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
protected:


};

#endif // SPEEDVIEW_HPP
