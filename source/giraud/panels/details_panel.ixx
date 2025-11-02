export module details_panel;

import database_panel;

export class DetailsPanel : public DatabasePanel
{
public:
	using DatabasePanel::DatabasePanel;

	std::string GetName() const override { return "Details"; }

	void Draw() override
	{

	}
};