#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent()
    {
        model.tick();
        FrontendApplicationBase::handleTickEvent();
    }
private:

public :
    // Speed
    void gotoSpeedScreenNoTransition();
    void gotoCompassScreenNoTransition();
    void gotoControlScreenNoTransition();

protected:
    touchgfx::Callback<FrontendApplication> transitionCallback;

    // Compass
    void gotoSpeedScreenNoTransitionImpl();
    void gotoCompassScreenNoTransitionImpl();
    void gotoControlScreenNoTransitionImpl();
};

#endif // FRONTENDAPPLICATION_HPP
