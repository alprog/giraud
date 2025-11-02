module;
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <format>
export module control_panel;

import database_panel;
import configuration;
import network;
import session;

// for intellisense

export class ControlPanel : public DatabasePanel
{
public:
	using DatabasePanel::DatabasePanel;

	std::string GetName() const override { return "ControlPanel"; }

	void Draw() override
	{
		if (ImGui::CollapsingHeader("Config"))
		{
			const Configuration& config = api.GetConfig();
			LabelText("Jira App Id", config.app.id);
			LabelText("Secret", config.app.secret);
			LabelText("Redirect URI", config.app.redirect_uri);
		}

		if (ImGui::CollapsingHeader("Session"))
		{
			const Session& session = api.GetNetwork().GetSession();
			LabelText("Access Token", session.accessToken);
			LabelText("Refresh Token", session.refreshToken);

			int secondsLeft = session.expirationTime - Timestamp::now();
			if (secondsLeft > 0)
			{
				std::string expirationText = std::format("Expires in: {}", secondsLeft);
				ImGui::Text(expirationText.c_str());
			}
			else
			{
				ImGui::Text("expired");
			}

			if (ImGui::Button("Refresh"))
			{
				api.GetNetwork().RefreshTokens();
			}
		}

		DrawCommands();
	}

	void DrawCommands()
	{
		const bool loggedIn = api.GetNetwork().IsLoggedIn();

		ImGui::BeginDisabled(!loggedIn);
		if (ImGui::Button("Update Projects"))
		{
			api.UpdateProjectList();
		}
		ImGui::EndDisabled();
	}
};