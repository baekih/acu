/*
 * CommonView.hpp
 *
 *  Created on: Feb 2, 2024
 *      Author: DRYOON
 */

#ifndef GUI_INCLUDE_GUI_COMMON_COMMONVIEW_HPP_
#define GUI_INCLUDE_GUI_COMMON_COMMONVIEW_HPP_


#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>

class ViewCommon
{
protected:

    int pressedX;
    int pressedY;

public:
    ViewCommon();
    virtual ~ViewCommon() {}

};


#endif /* GUI_INCLUDE_GUI_COMMON_COMMONVIEW_HPP_ */
