#pragma once
class MapMenu 
{
	bool ShowLoadMenu = false;
    
public:
    void ShowMenuBar();
	void RenderMenu();
	void HideMenu() { ShowLoadMenu = false; }
	void ShowMenu() { ShowLoadMenu = true; }
};