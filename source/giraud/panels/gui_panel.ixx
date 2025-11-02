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
		ImGui::Begin(GetName().c_str());
		Draw();
		ImGui::End();
	}

	void LabelText(std::string label, std::string text)
	{
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		ImGui::SetCursorPosX(150);
		ImGui::Text(text.c_str());
	}

	template <class... Types>
	inline void Text(std::format_string<Types...> fmt, Types&&... args)
	{
		std::string text = std::format(fmt, std::forward<Types>(args)...);
		ImGui::Text(text.c_str());
	}

protected:
	virtual std::string GetName() const = 0;
	virtual void Draw() = 0;
};