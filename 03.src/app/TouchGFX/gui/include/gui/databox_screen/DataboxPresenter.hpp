#ifndef DATABOXPRESENTER_HPP
#define DATABOXPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DataboxView;

class DataboxPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DataboxPresenter(DataboxView& v);

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

    virtual ~DataboxPresenter() {}

private:
    DataboxPresenter();

    DataboxView& view;
};

#endif // DATABOXPRESENTER_HPP
