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

void ControlView::dispHdgtTgtVal(int hdg_tgt_deg)
{
    printf("%s() hdg_tgt_deg[0x%x]\n",__FUNCTION__, hdg_tgt_deg);
    if(hdg_tgt_deg == 0x7FFFFFFF)
    {
        Unicode::snprintf(HDGT_TGT_VALUEBuffer, HDGT_TGT_VALUE_SIZE, "---");
    }
    else
    {
        Unicode::snprintf(HDGT_TGT_VALUEBuffer, HDGT_TGT_VALUE_SIZE, "%d", hdg_tgt_deg);
    }

    HDGT_TGT_VALUE.invalidate();
}

void ControlView::dispHdgtTgtSlider(int hdg_tgt_deg)
{
    printf("%s() hdg_tgt_deg[0x%x]\n",__FUNCTION__, hdg_tgt_deg);
    if(hdg_tgt_deg != 0x7FFFFFFF)
    {
        if(180 < hdg_tgt_deg)
        {
            HDGT_TGT_SLIDER.setValue((int16_t)(hdg_tgt_deg -= 360));
        }
        else
        {
            HDGT_TGT_SLIDER.setValue((int16_t)hdg_tgt_deg);
        }
    }

    HDGT_TGT_SLIDER.invalidate();
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

void ControlView::sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int hdgt_tgt_deg)
{
    if (&src == &HDGT_TGT_SLIDER)
    {
        printf("HDGT_TGT_SLIDER[%d]\r\n", hdgt_tgt_deg);

        if(hdgt_tgt_deg < 0) hdgt_tgt_deg += 360;
        g_boat.heading_target_em4 = (uint16_t)roundDEGtoRADem4(hdgt_tgt_deg);

        dispHdgtTgtVal(hdgt_tgt_deg);
    }
}

void ControlView::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    int hdgt_tgt_deg = 0;

    if(&src == &BTN_STOP)
    {
        printf("BTN_STOP\r\n");
        g_boat.heading_target_em4 = N2K_DATA_NOT_AVAILABLE_UINT16;
        hdgt_tgt_deg = 0x7FFFFFFF;
        dispHdgtTgtVal(hdgt_tgt_deg);
//        dispHdgtTgtSlider(hdgt_tgt_deg);
    }
    else if(&src == &BTN_HDGT_LEFT)
    {
        if(!isRADem4Valid(g_boat.heading_target_em4)) g_boat.heading_target_em4 = 0;

        hdgt_tgt_deg = (int)roundRADem4toDEG(g_boat.heading_target_em4);

        if(--hdgt_tgt_deg == -1) hdgt_tgt_deg = 359;

        g_boat.heading_target_em4 = (uint16_t)roundDEGtoRADem4(hdgt_tgt_deg);

        dispHdgtTgtVal(hdgt_tgt_deg);
        dispHdgtTgtSlider(hdgt_tgt_deg);

        printf("BTN_HDGT_LEFT hdgt_tgt_deg[%d]\r\n", hdgt_tgt_deg);
    }
    else if (&src == &BTN_HDGT_RIGHT)
    {
        if(!isRADem4Valid(g_boat.heading_target_em4)) g_boat.heading_target_em4 = 0;

        hdgt_tgt_deg = (int)roundRADem4toDEG(g_boat.heading_target_em4);

        if(++hdgt_tgt_deg == 360) hdgt_tgt_deg = 0;

        g_boat.heading_target_em4 = (uint16_t)roundDEGtoRADem4(hdgt_tgt_deg);

        dispHdgtTgtVal(hdgt_tgt_deg);
        dispHdgtTgtSlider(hdgt_tgt_deg);

        printf("BTN_HDGT_RIGHT hdgt_tgt_deg[%d]\r\n", hdgt_tgt_deg);
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
    if(isRADem4Valid(g_boat.heading_sensor_reading_em4))
    {
        Unicode::snprintf(HDGT_CUR_VALUEBuffer, HDGT_CUR_VALUE_SIZE, "%d",
                          (uint16_t)roundRADem4toDEG(g_boat.heading_sensor_reading_em4));
    }
    else
    {
        Unicode::snprintf(HDGT_CUR_VALUEBuffer, HDGT_CUR_VALUE_SIZE, "---");
    }
    HDGT_CUR_VALUE.invalidate();

    if(isRotValid(g_boat.rate_of_turn))
    {
        Unicode::snprintfFloat(ROT_VALUEBuffer, ROT_VALUE_SIZE, "%04.2f",
                          roundRotRad2Deg(g_boat.rate_of_turn));
    }
    else
    {
        Unicode::snprintf(ROT_VALUEBuffer, ROT_VALUE_SIZE, "-.--");
    }
    ROT_VALUE.invalidate();

    updateSOG(getSOGValue(SPEED_UNIT_KNOT));
    updateRUDcur(getRUDcurValue());
    updateRUDtgt(getRUDtgtValue());

//    if(0 == cnt%50) printf("[%06ld]ControlView::handleTickEvent()\n", cnt);
//    cnt++;

}
