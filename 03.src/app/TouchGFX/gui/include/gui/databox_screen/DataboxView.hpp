#ifndef DATABOXVIEW_HPP
#define DATABOXVIEW_HPP

#include <gui_generated/databox_screen/DataboxViewBase.hpp>
#include <gui/databox_screen/DataboxPresenter.hpp>

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/databox_screen/DataboxPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/TextArea.hpp>

#include <gui/common/CalcPixel.hpp>
#include <gui/common/ViewCommon.hpp>
#include <gui/data/DataBoxNavi.hpp>

class DataboxView : public DataboxViewBase, protected ViewCommon
{
	int speedType;

	DataBoxNavi databoxNavi;

private :
    void updateSpeed(double value, int max);

public:
    DataboxView();
    virtual ~DataboxView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateHDG(double hdgValue);

    void updateSTW(double value);
    void updateSOG(double value);

    void updateDepth(double value);
    void updateWTemp(double value);

    void updateWindSpeed(double value);

    void setSpeedType(int new_type);
    int getSpeedType();

    void handleTickEvent();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

protected:
    DataBoxNavi boxNavi;

};

#endif // DATABOXVIEW_HPP
