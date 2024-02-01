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

class SpeedView : public SpeedViewBase
{
public :
	enum {
		typeSTW,
		typeSOG,
	} SpeedType;

private :
	int type;

    touchgfx::Shape<4> shapeGuageLine[41];
	touchgfx::PainterRGB565 shapeGuagePainter[41];

	touchgfx::TextArea textGaugeSpeed[5];

	CalcPixel calcPixelGauge = CalcPixel(290, 288, 800);
	CalcPixel calcPixelIndicator = CalcPixel(290, 288, 800);

private :
    void drawSpeedGaugeLine(float degree, int offset, touchgfx::Shape<4>& line);
    void drawSpeedGaugeText(float degree, int offset, touchgfx::TextArea& text);
    void drawSpeedGaugeIndicator(float value, int max);

    void updateSpeed(float value, int max);
public:
    SpeedView();
    virtual ~SpeedView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateSTW(float stwValue);
    void updateSOG(float sogValue);

    void updateDepth(float depthValue);
    void updateWTemp(float wTempValue);

    void setType(int new_type);

    void handleTickEvent();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
protected:

    int pressedX;
    int pressedY;

};

#endif // SPEEDVIEW_HPP
