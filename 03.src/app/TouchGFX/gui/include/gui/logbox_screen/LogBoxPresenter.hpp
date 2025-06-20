#ifndef LOGBOXPRESENTER_HPP
#define LOGBOXPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class LogBoxView;

class LogBoxPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    LogBoxPresenter(LogBoxView& v);

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

    virtual ~LogBoxPresenter() {}

private:
    LogBoxPresenter();

    LogBoxView& view;
};

#endif // LOGBOXPRESENTER_HPP
