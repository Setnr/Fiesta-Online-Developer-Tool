#include "EditMode.h"

NiImplementRootRTTI(EditMode);

void EditMode::Draw() 
{
	for (int i = 0; i < ScreenElements.size(); i++)
	{
		if (!ScreenElements.at(i)->Draw())
		{
			ScreenElements.erase(std::remove(ScreenElements.begin(), ScreenElements.end(), ScreenElements.at(i)), ScreenElements.end());
			i--;
		}
	}
}
void EditMode::Update(float fTime) 
{

}