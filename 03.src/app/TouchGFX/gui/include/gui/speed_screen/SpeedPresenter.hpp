#ifndef SPEEDPRESENTER_HPP
#define SPEEDPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SpeedView;

class SpeedPresenter : public touchgfx::Presenter, public ModelListener
{

public:
    SpeedPresenter(SpeedView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~SpeedPresenter() {}

    virtual void notifySTWValue(float stwValue);
    virtual void notifySOGValue(float sogValue);
    virtual void notifyDepthValue(float depthValue);
    virtual void notifyWTempValue(float wtempValue);

private:
    SpeedPresenter();

    SpeedView& view;
};

#endif // SPEEDPRESENTER_HPP
