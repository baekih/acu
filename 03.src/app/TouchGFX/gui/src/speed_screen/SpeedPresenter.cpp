#include <gui/speed_screen/SpeedView.hpp>
#include <gui/speed_screen/SpeedPresenter.hpp>

SpeedPresenter::SpeedPresenter(SpeedView& v)
    : view(v)
{

}

void SpeedPresenter::activate()
{

}

void SpeedPresenter::deactivate()
{

}

void SpeedPresenter::notifySTWValue(float stwValue)
{
	view.updateSTW(stwValue);
}

void SpeedPresenter::notifySOGValue(float sogValue)
{
	view.updateSOG(sogValue);
}

void SpeedPresenter::notifyDepthValue(float depthValue)
{
	view.updateDepth(depthValue);
}

void SpeedPresenter::notifyWTempValue(float wtempValue)
{
	view.updateWTemp(wtempValue);
}
