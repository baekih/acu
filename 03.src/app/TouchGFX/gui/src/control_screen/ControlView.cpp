#include <gui/control_screen/ControlView.hpp>

#include "eco.h"

ControlView::ControlView():
    sliderValueChangedCallback(this, &ControlView::sliderValueChangedCallbackHandler),
    buttonCallback(this, &ControlView::buttonCallbackHandler)
{

}

void ControlView::setupScreen()
{
    ControlViewBase::setupScreen();
    HDGT_TGT_SLIDER.setNewValueCallback(sliderValueChangedCallback);
    BTN_STOP.setAction(buttonCallback);
    BTN_HDGT_LEFT.setAction(buttonCallback);
    BTN_HDGT_RIGHT.setAction(buttonCallback);
}

void ControlView::tearDownScreen()
{
    ControlViewBase::tearDownScreen();
}

void ControlView::dispHDGTtgt(int hdg_tgt)
{
    if(true){
        Unicode::snprintf(HDGT_TGT_VALUEBuffer, HDGT_TGT_VALUE_SIZE, "%d", hdg_tgt);
//        Unicode::snprintf(HDGT_TGT_VALUEBuffer, HDGT_TGT_VALUE_SIZE, "%d", (int)adjustDisplayAngleDegree(GetRound(hdgValue, 1)));
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

void ControlView::updateRUDcur(float rud_cur_val)
{
    float rud_cur_deg_val = rud_cur_val*RAD2DEG;

    Unicode::snprintfFloat(RUD_CUR_VALUEBuffer, RUD_CUR_VALUE_SIZE, "%04.1f", rud_cur_deg_val);
    RUD_CUR_VALUE.invalidate();

    RUD_CUR.setValue((int16_t)(round(rud_cur_deg_val)));
    RUD_CUR.invalidate();
}

void ControlView::updateRUDtgt(float rud_tgt_val)
{
    float rud_tgt_deg_val = rud_tgt_val*RAD2DEG;

    Unicode::snprintfFloat(RUD_TGT_VALUEBuffer, RUD_TGT_VALUE_SIZE, "%04.1f", rud_tgt_deg_val);
    RUD_TGT_VALUE.invalidate();

    RUD_TGT.setValue((int16_t)(round(rud_tgt_deg_val)));
    RUD_TGT.invalidate();
}

void ControlView::sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value)
{
    if (&src == &HDGT_TGT_SLIDER)
    {
        printf("HDGT_TGT_SLIDER[%d]\r\n", value);

        if(-180 <= value && value < 0)
        {
            value += 360;
        }

        g_boat.heading_target_em4 = (uint16_t)lround((float)value*DEG2RAD*10000.0);

        dispHDGTtgt(value);
    }
}

void ControlView::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if(&src == &BTN_STOP)
    {
        printf("BTN_STOP\r\n");
    }
    else if(&src == &BTN_HDGT_LEFT)
    {
        int hdgt_tgt = (int)lround((float)g_boat.heading_target_em4/10000.0*RAD2DEG);

        if(--hdgt_tgt == -1) hdgt_tgt = 359;

        g_boat.heading_target_em4 = (uint16_t)lround((float)hdgt_tgt*DEG2RAD*10000.0);

        dispHDGTtgt(hdgt_tgt);

        printf("BTN_HDGT_LEFT hdgt_tgt[%d] em4[%d]\r\n", hdgt_tgt, g_boat.heading_target_em4);

        if(180 < hdgt_tgt)  HDGT_TGT_SLIDER.setValue((int16_t)(hdgt_tgt - 360));
        else                HDGT_TGT_SLIDER.setValue((int16_t)hdgt_tgt);

        HDGT_TGT_SLIDER.invalidate();
    }
    else if (&src == &BTN_HDGT_RIGHT)
    {
        int hdgt_tgt = (int)lround((float)g_boat.heading_target_em4/10000.0*RAD2DEG);

        if(++hdgt_tgt == 360) hdgt_tgt = 0;

        g_boat.heading_target_em4 = (uint16_t)lround((float)hdgt_tgt*DEG2RAD*10000.0);

        dispHDGTtgt(hdgt_tgt);

        printf("BTN_HDGT_RIGHT hdgt_tgt[%d] em4[%d]\r\n", hdgt_tgt, g_boat.heading_target_em4);

        if(180 < hdgt_tgt)  HDGT_TGT_SLIDER.setValue((int16_t)(hdgt_tgt - 360));
        else                HDGT_TGT_SLIDER.setValue((int16_t)hdgt_tgt);

        HDGT_TGT_SLIDER.invalidate();
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
//    static uint32_t cnt = 0;

    // update heading sensor reading
    if(g_boat.heading_sensor_reading_em4 < N2K_OUT_OF_ORDER_UINT16)
    {
        Unicode::snprintf(HDGT_CUR_VALUEBuffer, HDGT_CUR_VALUE_SIZE, "%d",
                          (uint16_t)lround(((float)g_boat.heading_sensor_reading_em4)/10000.0*RAD2DEG));
    }
    else
    {
        Unicode::snprintf(HDGT_CUR_VALUEBuffer, HDGT_CUR_VALUE_SIZE, "---");
    }
    HDGT_CUR_VALUE.invalidate();

    updateSOG(getSOGValue(SPEED_UNIT_KNOT));
    updateRUDcur(getRUDcurValue());
    updateRUDtgt(getRUDtgtValue());

//    if(0 == cnt%50) printf("[%06ld]ControlView::handleTickEvent()\n", cnt);
//    cnt++;

}
