#ifndef CONTROLVIEW_HPP
#define CONTROLVIEW_HPP

#include <gui_generated/control_screen/ControlViewBase.hpp>
#include <gui/control_screen/ControlPresenter.hpp>

#include <gui/common/ViewCommon.hpp>
//#include "printf.h"

class ControlView : public ControlViewBase, protected ViewCommon
{
    int speedType;

private :
    void updateSpeed(double value, int max);

public:
    ControlView();
    virtual ~ControlView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void dispHdgtTgtVal(int hdg_tgt_deg);
    void dispHdgtTgtSlider(int hdg_tgt_deg);
    void updateSOG(double value);
    void updateRUDcur(float rudcurValue);
    void updateRUDtgt(float rudtgtValue);

    void handleTickEvent();

    touchgfx::Callback<ControlView, const touchgfx::Slider&, int> sliderValueChangedCallback;
    touchgfx::Callback<ControlView, const touchgfx::AbstractButton&> buttonCallback;

    void sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int hdgt_tgt_deg);
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

protected:
};

#endif // CONTROLVIEW_HPP
