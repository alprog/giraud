export module database_panel;

export import gui_panel;

export import database;
export import selection;
export import edit_api;

// for intellisense

export class DatabasePanel : public GuiPanel
{
public:
	DatabasePanel(const Database& db, Selection& selection, EditAPI& api)
		: db{ db }
		, selection{ selection }
		, api{ api }
	{
	}

	const Database& db;
	Selection& selection;
	EditAPI& api;
};