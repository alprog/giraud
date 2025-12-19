export module folder;

import std;
import tree_item;

export class Folder : public TreeItem
{
public:
	Folder(TreeItem* parent, std::string name)
		: TreeItem{ parent->uid + "/" + name }
	{
	}

	std::string name;
};