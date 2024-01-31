#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

int Model::curHDG = 0;
int Model::prevHDG = 0;

Model::Model() : modelListener(0)
{
}

void Model::tick()
{
	// Second, notify the currently active Presenter that temperature has changed.
	// The modelListener pointer points to the currently active Presenter.
	if (modelListener != 0)
	{
		if(prevHDG != curHDG){
			modelListener->notifyHDGValue(curHDG);
			prevHDG = curHDG;
		}
	}
}
