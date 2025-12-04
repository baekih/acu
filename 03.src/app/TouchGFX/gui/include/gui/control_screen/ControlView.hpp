#ifndef CONTROLVIEW_HPP
#define CONTROLVIEW_HPP

#include <gui_generated/control_screen/ControlViewBase.hpp>
#include <gui/control_screen/ControlPresenter.hpp>

#include <gui/common/ViewCommon.hpp>

#define K_IDX_MAX               (14)
#define TOUCH_GLITCH_TIME_MS    70

class ControlView : public ControlViewBase, protected ViewCommon
{

private :
    float k_hdge_gain[K_IDX_MAX] = {0.0, 0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0};
    float k_rote_gain[K_IDX_MAX] = {0.0, 0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0};
    float k_hdgi_gain[K_IDX_MAX] = {0.0, 0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0};
    unsigned int k_hdge_idx = 8;
    unsigned int k_rote_idx = 8;
    unsigned int k_hdgi_idx = 0;

public:
    ControlView();
    virtual ~ControlView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

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
