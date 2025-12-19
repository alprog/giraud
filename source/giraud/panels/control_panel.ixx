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

		if (ImGui::CollapsingHeader("User"))
		{
			const User& user = db.myself;
			LabelText("Id", user.accountId);
			LabelText("Name", user.displayName);
			LabelText("Email", user.emailAddress);
			LabelText("Avatar16", user.avatarUrls._16x16);
			LabelText("Avatar24", user.avatarUrls._24x24);
			LabelText("Avatar32", user.avatarUrls._32x32);
			LabelText("Avatar48", user.avatarUrls._48x48);
			user.displayName;
		}

		DrawCommands();
	}

	void DrawCommands()
	{
		const bool loggedIn = api.GetNetwork().IsLoggedIn();
		ImGui::BeginDisabled(!loggedIn);

		if (ImGui::Button("Update Myself"))
		{
			api.UpdateMyself();
		}

		if (ImGui::Button("Update Projects"))
		{
			api.UpdateProjectList();
		}

		if (ImGui::Button("Update Epics"))
		{
			api.UpdateEpics();
		}

		if (ImGui::Button("Update SubTasks"))
		{
			api.UpdateSubTasks();
		}

		ImGui::EndDisabled();
	}
};