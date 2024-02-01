#include <gui/common/FrontendApplication.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <gui/compass_screen/CompassView.hpp>
#include <gui/compass_screen/CompassPresenter.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{
}

void FrontendApplication::gotoCompassScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoCompassScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoCompassScreenNoTransitionImpl()
{
    touchgfx::makeTransition<CompassView, CompassPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
