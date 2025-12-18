export module tree_item;

import std;

export struct TreeItem
{
	bool opened = false;
	std::vector<TreeItem*> children;
};