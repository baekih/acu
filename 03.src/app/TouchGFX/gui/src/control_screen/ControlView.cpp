#include <gui/control_screen/ControlView.hpp>

ControlView::ControlView():
    sliderValueChangedCallback(this, &ControlView::sliderValueChangedCallbackHandler)
{

}

void ControlView::setupScreen()
{
    ControlViewBase::setupScreen();
    HDGT_TGT_SLIDER.setNewValueCallback(sliderValueChangedCallback);
}

void ControlView::tearDownScreen()
{
    ControlViewBase::tearDownScreen();
}

void ControlView::updateHDGTcur(double hdgValue)
{
    if(isTimeInHDG() && isValidHDG()){
        Unicode::snprintf(HDGT_CUR_VALUEBuffer, HDGT_CUR_VALUE_SIZE, "%d", (int)adjustDisplayAngleDegree(GetRound(hdgValue, 1)));
    }
    else {
        Unicode::snprintf(HDGT_CUR_VALUEBuffer, HDGT_CUR_VALUE_SIZE, "---");
    }

    HDGT_CUR_VALUE.invalidate();
}

void ControlView::updateHDGTtgt(double hdgValue)
{
    if(isTimeInHDG() && isValidHDG()){
        Unicode::snprintf(HDGT_TGT_VALUEBuffer, HDGT_TGT_VALUE_SIZE, "%d", (int)adjustDisplayAngleDegree(GetRound(hdgValue, 1)));
    }
    else {
        Unicode::snprintf(HDGT_TGT_VALUEBuffer, HDGT_TGT_VALUE_SIZE, "---");
    }

    HDGT_TGT_VALUE.invalidate();
}

void ControlView::updateSOG(double sogValue)
{
    Unicode::snprintfFloat(SOG_VALUEBuffer, SOG_VALUE_SIZE, "%02.1f", sogValue);

    SOG_VALUE.invalidate();
}

void ControlView::sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value)
{
    if (&src == &HDGT_TGT_SLIDER)
    {
        printf("ControlView:callback\r\n");
        updateHDGTtgt((double)value);
    }
}

/*
void ControlView::handleClickEvent(const ClickEvent& evt)
{
    int x = evt.getX();
    int y = evt.getY();

    if (evt.getType() == ClickEvent::PRESSED)
    {
        pressedX = x;
        pressedY = y;
    }
    else if (evt.getType() == ClickEvent::RELEASED)
    {
        if((pressedX < 100) && (pressedX - x > 20)){
            static_cast<FrontendApplication*>(Application::getInstance())->gotoCompassScreenNoTransition();
        }
    }
}
*/
void ControlView::handleTickEvent()
{
#ifdef SIMULATOR
    static int stw = 0;

    stw++;

    if((stw % 10) == 0){
        updateSTW((double)stw / 100);
        updateWindSpeed((double)stw / 100);
        if(stw >= 2000){
            stw = 0;
        }
    }

    static int count = 0;

    updateHDG(((((double)(count) / 10000) / (2*M_PI)) * 360));

    count++;

    if(count > 62832){
        count = 0;
    }

#else
    updateHDGTcur(getHDGValue());
    updateSOG(getSOGValue(SPEED_UNIT_KNOT));

#endif
}
