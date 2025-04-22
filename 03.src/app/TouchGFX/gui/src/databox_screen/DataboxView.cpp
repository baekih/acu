#include <math.h>
#include <cstdio>

#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#include <gui/databox_screen/DataboxView.hpp>
#include <gui/speed_screen/SpeedView.hpp>
#include <gui/compass_screen/CompassView.hpp>
#include <gui/common/validate_data.h>

DataboxView::DataboxView() :
	speedType(typeSTW)
{
	boxNavi.CreateCanvas(*this);

	for(int i = 0; i < NAVI_WIDGET_MAX_COUNT; i++){
		add(boxNavi.widget[i].NAVI_BOX);
		add(boxNavi.widget[i].NAVI_LONGITUDE_INDI);
		add(boxNavi.widget[i].NAVI_LONGITUDE);
		add(boxNavi.widget[i].NAVI_LATITUDE_INDI);
		add(boxNavi.widget[i].NAVI_LATITUDE);
		add(boxNavi.widget[i].NAVI_TITLE);
	}
}

void DataboxView::setupScreen()
{
    DataboxViewBase::setupScreen();
}

void DataboxView::tearDownScreen()
{
    DataboxViewBase::tearDownScreen();
}

void DataboxView::updateHDG(double hdgValue)
{
    if(isTimeInHDG() && isValidHDG()){
    	Unicode::snprintf(HDG_VALUEBuffer, HDG_VALUE_SIZE, "%d", (int)adjustDisplayAngleDegree(GetRound(hdgValue, 1)));
    }
    else {
    	Unicode::snprintf(HDG_VALUEBuffer, HDG_VALUE_SIZE, "---");
    }

    HDG_VALUE.invalidate();
}

void DataboxView::setSpeedType(int new_type)
{
	if(new_type == typeSTW){
		speedType = new_type;
		Unicode::snprintf(SPEED_TITLEBuffer, SPEED_TITLE_SIZE, "STW");
	}
	else if(new_type == typeSOG){
		speedType = new_type;
		Unicode::snprintf(SPEED_TITLEBuffer, SPEED_TITLE_SIZE, "SOG");
	}

	SPEED_TITLE.invalidate();
}

void DataboxView::updateSTW(double stwValue)
{
	if(speedType == typeSTW){
		updateSpeed(stwValue, 20);
	}
}

void DataboxView::updateSOG(double sogValue)
{
	if(speedType == typeSOG){
		updateSpeed(sogValue, 20);
	}
}

void DataboxView::updateSpeed(double value, int max)
{
	if(value < speedDisplayMin || value > speedDisplayMax){
		Unicode::snprintf(SPEED_VALUEBuffer, SPEED_VALUE_SIZE, SPEED_OUT_OF_RANGE);
	}
	else{
		double speed = GetRound(value, 10.0);

		if(speed < 100){
			Unicode::snprintfFloat(SPEED_VALUEBuffer, SPEED_VALUE_SIZE, "%.1f", speed);
		}
		else {
			Unicode::snprintf(SPEED_VALUEBuffer, SPEED_VALUE_SIZE, "%d", (int)speed);
		}
	}

    SPEED_VALUE.invalidate();
}

void DataboxView::updateDepth(double value)
{
	if(value < depthDisplayMin || value > depthDisplayMeterMax){
		Unicode::snprintf(DEPTH_VALUEBuffer, DEPTH_VALUE_SIZE, OUT_OF_RANGE);
	}
	else {
		double depthValue = GetRound(value, 10.0);

		if(depthValue < 100){
			Unicode::snprintfFloat(DEPTH_VALUEBuffer, DEPTH_VALUE_SIZE, "%.1f", depthValue);
		}
		else{
			Unicode::snprintf(DEPTH_VALUEBuffer, DEPTH_VALUE_SIZE, "%d", (int)depthValue);
		}
	}

	DEPTH_VALUE.invalidate();
}

void DataboxView::updateWTemp(double value)
{
    double tempC = GetRound(value, 10.0);

    const char MAX_TEMP_VALUE[] = "*99.9";
    const char MIN_TEMP_VALUE[] = "-*9.9";

    if(tempC > tempCelsiusDisplayMax){
    	Unicode::snprintf(WTEMP_VALUEBuffer, WTEMP_VALUE_SIZE, MAX_TEMP_VALUE);
    }
    else if(tempC < tempCelsiusDisplayMin){
    	Unicode::snprintf(WTEMP_VALUEBuffer, WTEMP_VALUE_SIZE, MIN_TEMP_VALUE);
    }
    else{
    	Unicode::snprintfFloat(WTEMP_VALUEBuffer, WTEMP_VALUE_SIZE, "%.1f", tempC);
    }

	WTEMP_VALUE.invalidate();
}

void DataboxView::updateWindSpeed(double value)
{
	if(value < speedDisplayMin || value > speedDisplayMax){
		Unicode::snprintf(WIND_VALUEBuffer, WIND_VALUE_SIZE, SPEED_OUT_OF_RANGE);
	}
	else {
		double speed = GetRound(value, 10.0);

		if(speed < 100){
			Unicode::snprintfFloat(WIND_VALUEBuffer, WIND_VALUE_SIZE, "%.1f", speed);
		}
		else{
			Unicode::snprintf(WIND_VALUEBuffer, WIND_VALUE_SIZE, "%d", (int)speed);
		}
	}

	WIND_VALUE.invalidate();
}


void DataboxView::handleClickEvent(const ClickEvent& evt)
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
        if((pressedY > x) && (pressedY - x) > 200){
            static_cast<FrontendApplication*>(Application::getInstance())->gotoCompassScreenNoTransition();
        }
    }
}

void DataboxView::handleDragEvent(const DragEvent& evt)
{

}

void DataboxView::handleTickEvent()
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

	boxNavi.updatePosition(90.0f + (double)count/10000.0, 135.0f + (double)count/10000.0);
#else
	updateHDG(getHDGValue());
	updateSTW(getSTWValue(SPEED_UNIT_KNOT));
	updateSOG(getSOGValue(SPEED_UNIT_KNOT));
	updateDepth(getDepthValue(DEPTH_UNIT_METER));
	updateWTemp(getWTempValue(UNIT_TEMP_CELSIUS));
	updateWindSpeed(getWindSpeedValue(SPEED_UNIT_KNOT));

	boxNavi.updatePosition(getLatitude(), getLongitude());

#endif
}
