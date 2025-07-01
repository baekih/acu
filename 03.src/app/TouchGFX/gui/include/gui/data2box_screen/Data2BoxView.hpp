#ifndef DATA2BOXVIEW_HPP
#define DATA2BOXVIEW_HPP

#include <gui_generated/data2box_screen/Data2BoxViewBase.hpp>
#include <gui/data2box_screen/Data2BoxPresenter.hpp>

class Data2BoxView : public Data2BoxViewBase
{
public:
    Data2BoxView();
    virtual ~Data2BoxView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);

protected:
};

#endif // DATA2BOXVIEW_HPP
