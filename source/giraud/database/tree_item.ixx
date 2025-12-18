export module tree_item;

import std;

export struct TreeItem
{
	TreeItem* parent = nullptr;
	bool opened = false;
	std::vector<TreeItem*> children;

	virtual std::string& getDisplayText() = 0;
};