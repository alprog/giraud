module;
#include "imgui.h"
export module gui_panel;

export import std;

// for intellisense

export class GuiPanel
{
public:
	void Render()
	{
		if (isModal)
		{

		}
		else
		{
			ImGui::Begin(GetName().c_str());
			Draw();
			ImGui::End();
		}

	}

protected:
	virtual std::string GetName() const = 0;
	virtual void Draw() = 0;

	bool isModal = false;
};