export module database_panel;

export import gui_panel;

export import database;
export import edit_api;

// for intellisense

export class DatabasePanel : public GuiPanel
{
public:
	DatabasePanel(const Database& db, EditAPI& api)
		: db{ db }
		, api{ api }
	{
	}

	const Database& db;
	EditAPI& api;
};