#include <stm32f7xx_hal.h>

#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include <gui/common/Database.hpp>

Model::Model() : modelListener(0)
{
}

void Model::tick()
{
	// Second, notify the currently active Presenter that temperature has changed.
	// The modelListener pointer points to the currently active Presenter.
	if (modelListener != 0)
	{
        if( (HAL_GetTick() - tickTm) >= 250){
        	tickTm = HAL_GetTick();

        	modelListener->notifyHDGValue(getHDGValue());
        	modelListener->notifySTWValue(getSTWValue(SPEED_UNIT_KNOT));
        	modelListener->notifySOGValue(getSOGValue(SPEED_UNIT_KNOT));
        	modelListener->notifyDepthValue(getDepthValue(DEPTH_UNIT_METER));
        	modelListener->notifyWTempValue(getWTempValue(UNIT_TEMP_CELSIUS));
        }
	}
}
