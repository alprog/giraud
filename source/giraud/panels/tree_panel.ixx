module;
#include "imgui.h"
export module tree_panel;

import database_panel;

// for intellisense

export class TreePanel : public DatabasePanel
{
public:
	using DatabasePanel::DatabasePanel;

	std::string GetName() const override { return "Hierarchy"; }

	void Draw() override
	{
		for (auto& project : db.projects)
		{
			ImGui::Text(project.name.c_str());
		}
	}
};