module;
#include "imgui.h"
export module table_panel;

import database_panel;
import issue;

export class TablePanel : public DatabasePanel
{
public:
	using DatabasePanel::DatabasePanel;

	std::string GetName() const override { return "Table"; }

	void Draw() override
	{
		if (!selection.item)
			return;

		auto flags = ImGuiTableFlags_Resizable;
		if (ImGui::BeginTable("Table", 3, flags))
		{
			ImGui::TableSetupColumn("KEY", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Summary", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Details", ImGuiTableColumnFlags_WidthFixed);

			for (auto childItem : selection.item->children)
			{
				auto issue = dynamic_cast<Issue*>(childItem);
				if (issue)
				{
					ImGui::TableNextColumn();
					ImGui::Text(issue->key.c_str());


					ImGui::TableNextColumn();
					ImGui::Text(issue->title.c_str());

					ImGui::TableNextColumn();
					ImGui::Text("--");
				}
			}
			ImGui::EndTable();
		}
	}
};