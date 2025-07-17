#ifndef CONTROLVIEW_HPP
#define CONTROLVIEW_HPP

#include <gui_generated/control_screen/ControlViewBase.hpp>
#include <gui/control_screen/ControlPresenter.hpp>

#include <gui/common/ViewCommon.hpp>

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

    void updateHDGTcurr(double hdgValue);
    void updateSOG(double value);

    void handleTickEvent();

//    virtual void handleClickEvent(const ClickEvent& evt);

protected:
};

#endif // CONTROLVIEW_HPP
