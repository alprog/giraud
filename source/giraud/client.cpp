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
	, selection{db}
	, api{ db, network }
{
	config.load();

	gui.AddPanel<UserPanel>(db, selection, api);
	gui.AddPanel<TreePanel>(db, selection, api);
	gui.AddPanel<TablePanel>(db, selection, api);
	gui.AddPanel<DetailsPanel>(db, selection, api);

	gui.AddPanel<ControlPanel>(db, selection, api);

	//gui.ShowDemoPanel();
}

void Client::RunEventLoop()
{
	while (window.ProcessEvents())
	{
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

