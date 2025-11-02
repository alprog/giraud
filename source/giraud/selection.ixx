export module selection;

import database;

// for intellisense

export class Selection
{
public:
	explicit Selection(const Database& database)
		: db{ database }
	{
	}

	const Database& db;
};