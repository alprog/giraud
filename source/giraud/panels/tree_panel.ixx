module;
#include "imgui.h"
export module tree_panel;

import database_panel;

export class TreePanel : public DatabasePanel
{
public:
	using DatabasePanel::DatabasePanel;

	std::string GetName() const override { return "TreePanel"; }

	void Draw() override
	{
		const bool loggedIn = api.GetNetwork().IsLoggedIn();

		ImGui::BeginDisabled(!loggedIn);
		if (ImGui::Button("Update Projects"))
		{
			api.UpdateProjectList();
		}
		ImGui::EndDisabled();

		for (auto& project : db.projects)
		{
			ImGui::Text(project.name.c_str());
		}
	}
};