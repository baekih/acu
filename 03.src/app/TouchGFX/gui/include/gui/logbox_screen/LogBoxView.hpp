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

    void updateHDG(double hdgValue);

    void updateSTW(double value);
    void updateSOG(double value);

    void updateDepth(double value);
    void updateWTemp(double value);

    void updateWindSpeed(double value);

    void setSpeedType(int new_type);
    int getSpeedType();

    void handleTickEvent();

    virtual void handleClickEvent(const ClickEvent& evt);

protected:
};

#endif // LOGBOXVIEW_HPP
