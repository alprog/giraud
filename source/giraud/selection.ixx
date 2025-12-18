export module selection;

import database;
import tree_item;

// for intellisense

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