module;
#include "imgui.h"
export module tree_panel;

import database_panel;
import tree_item;

// for intellisense

export class TreePanel : public DatabasePanel
{
public:
	using DatabasePanel::DatabasePanel;

	std::string GetName() const override { return "Hierarchy"; }

	void Draw() override
	{
		for (TreeItem* item : db.projects)
		{
			DrawItem(item);
		}
	}

	void DrawItem(TreeItem* item)
	{
		static ImGuiTreeNodeFlags defaultFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		ImGui::SetNextItemOpen(item->opened, ImGuiCond_Always);

		auto flags = defaultFlags;
		if (selection.item == item)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		item->opened = ImGui::TreeNodeEx(item, flags, item->getDisplayText().c_str());

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			selection.item = item;
		}

		if (item->opened)
		{
			if (item->children.empty())
			{
				ImGui::Text("<empty>");
			}
			else
			{
				for (auto child : item->children)
				{
					DrawItem(child);
				}
			}
			ImGui::TreePop();
		}
	}
};