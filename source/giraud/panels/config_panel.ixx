module;
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
export module config_panel;

import database_panel;
import configuration;

export class ConfigPanel : public GuiPanel
{
public:
	ConfigPanel(Configuration& config)
		: config{ config }
	{
	}

	std::string GetName() const override { return "ConfigPanel"; }

	void Draw() override
	{
		auto& app = config.app;

		ImGui::Text(app.id.c_str());
		ImGui::Text(app.secret.c_str());
		ImGui::Text(app.redirect_uri.c_str());
	}

	Configuration& config;
};