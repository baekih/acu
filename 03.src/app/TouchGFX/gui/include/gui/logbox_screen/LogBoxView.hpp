#ifndef LOGBOXVIEW_HPP
#define LOGBOXVIEW_HPP

#include <gui_generated/logbox_screen/LogBoxViewBase.hpp>
#include <gui/logbox_screen/LogBoxPresenter.hpp>

#include <gui/common/ViewCommon.hpp>

class LogBoxView : public LogBoxViewBase, protected ViewCommon
{
    int speedType;

private :
    void updateSpeed(double value, int max);


public:
    LogBoxView();
    virtual ~LogBoxView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateHDGTcurr(double hdgValue);
    void updateSOG(double value);

    void handleTickEvent();

    virtual void handleClickEvent(const ClickEvent& evt);

protected:
};

#endif // LOGBOXVIEW_HPP
