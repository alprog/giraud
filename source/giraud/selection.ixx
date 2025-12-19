export module selection;

// for intellisense

import database;
import tree_item;

export class Selection
{
public:
	explicit Selection(const Database& database)
		: db{ database }
	{
	}

	TreeItem* item = nullptr;

	const Database& db;
};