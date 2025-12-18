module client;

import control_panel;
import user_panel;
import tree_panel;
import table_panel;
import details_panel;

Client& Client::GetInstance()
{
	static Client instance;
	return instance;
}

Client::Client()
	: window{}
	, renderer{ window }
	, gui{ window, renderer }
	, config{}
	, network{ config }
	, db{}
	, selection{ db }
	, api{ db, selection, network }
{
	config.load();

	gui.AddPanel<UserPanel>(db, selection, api);
	gui.AddPanel<TreePanel>(db, selection, api);
	gui.AddPanel<TablePanel>(db, selection, api);
	gui.AddPanel<DetailsPanel>(db, selection, api);

	gui.AddPanel<ControlPanel>(db, selection, api);
}

void Client::RunEventLoop()
{
	while (window.ProcessEvents())
	{
		update();

		gui.NewFrame();

		gui.PrepareDraw();

		renderer.StartFrame();
		gui.Draw();
		renderer.EndFrame();

		gui.RenderPlatformWindows();

		renderer.Present();
	}

	renderer.WaitForLastSubmittedFrame();
}

void Client::update()
{
	if (network.IsLoggedIn())
	{
		if (network.GetSession().IsExpired())
		{
			network.RefreshTokens();
		}
		else if (!db.myself.isValid())
		{
			api.UpdateMyself();
		}
		else if (db.projects.empty())
		{
			api.UpdateProjectList();
		}
	}
}