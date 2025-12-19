export module folder;

import std;
import tree_item;

export class Folder : public TreeItem
{
public:
	std::string name;

	Folder(std::string uid)
		: TreeItem{ uid }
	{
	}

	std::string& getDisplayText() override
	{
		return name;
	}
};