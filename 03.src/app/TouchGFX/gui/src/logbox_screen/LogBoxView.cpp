#include <gui/logbox_screen/LogBoxView.hpp>

LogBoxView::LogBoxView()
{

}

void LogBoxView::setupScreen()
{
    LogBoxViewBase::setupScreen();
}

void LogBoxView::tearDownScreen()
{
    LogBoxViewBase::tearDownScreen();
}

void LogBoxView::handleClickEvent(const ClickEvent& evt)
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
        if((pressedX < 100) && (pressedX - x > 20)){
            static_cast<FrontendApplication*>(Application::getInstance())->gotoCompassScreenNoTransition();
        }
    }
}

void LogBoxView::handleTickEvent()
{
#ifdef SIMULATOR
#else
#endif
}
