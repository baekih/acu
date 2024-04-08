#ifndef DATABOXNAVI_HPP
#define DATABOXNAVI_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/databox_screen/DataboxPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/TextArea.hpp>

#include <gui_generated/databox_screen/DataboxViewBase.hpp>
#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>

#define NAVI_WIDGET_MAX_COUNT 	4

class NaviWidget {
public :
	touchgfx::BoxWithBorder NAVI_BOX;
	touchgfx::TextAreaWithOneWildcard NAVI_LONGITUDE_INDI;
	touchgfx::TextAreaWithOneWildcard NAVI_LONGITUDE;
	touchgfx::TextAreaWithOneWildcard NAVI_LATITUDE_INDI;
	touchgfx::TextAreaWithOneWildcard NAVI_LATITUDE;
	touchgfx::TextAreaWithOneWildcard NAVI_TITLE;

	static const uint16_t NAVI_LONGITUDE_INDI_SIZE = 2;
	static const uint16_t NAVI_LATITUDE_INDI_SIZE = 2;
	static const uint16_t NAVI_LONGITUDE_SIZE = 16;
	static const uint16_t NAVI_LATITUDE_SIZE = 16;
	static const uint16_t NAVI_TITLE_SIZE = 16;

	touchgfx::Unicode::UnicodeChar NAVI_LONGITUDE_INDIBuffer[NAVI_LONGITUDE_INDI_SIZE];
	touchgfx::Unicode::UnicodeChar NAVI_LONGITUDEBuffer[NAVI_LONGITUDE_SIZE];
	touchgfx::Unicode::UnicodeChar NAVI_LATITUDE_INDIBuffer[NAVI_LATITUDE_INDI_SIZE];
	touchgfx::Unicode::UnicodeChar NAVI_LATITUDEBuffer[NAVI_LATITUDE_SIZE];
	touchgfx::Unicode::UnicodeChar NAVI_TITLEBuffer[NAVI_TITLE_SIZE];

	NaviWidget();
	virtual ~NaviWidget(){};

public :
	void createWidget(int pos);
};

class DataBoxNavi : protected touchgfx::View<DataboxPresenter> {

public :
	DataBoxNavi();
	virtual ~DataBoxNavi(){};

	double GetPositionMinRound(double value);
	void degreeToDM(double degree, int *getDegreeInt, double *getMinute);
	void GetDegreeToDMString(double degree, touchgfx::Unicode::UnicodeChar* formattedString, int stringSize);

	void updatePosition(double latitudeDegree, double longitudeDegree);

public:

	NaviWidget widget[NAVI_WIDGET_MAX_COUNT];

public:
	void CreateCanvas(DataboxViewBase& view);

};

#endif // DATABOXNAVI_HPP
