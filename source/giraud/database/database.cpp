module database;

import issue;

std::vector<TreeItem*> toArray(std::unordered_set<Folder*>& folderSet)
{
	std::vector<TreeItem*> array;
	array.reserve(folderSet.size());
	for (auto folder : folderSet)
	{
		array.push_back(folder);
	}
	sort_ascending(array);
	return array;
}

void Database::CreateSubFolders(TreeItem* item, TFolderNameGetter folderNameGetter)
{
	std::unordered_set<Folder*> folderSet;

	for (auto child : item->children)
	{
		auto folderName = folderNameGetter(child);
		auto folder = this->folders[item->uid + "/" + folderName];
		if (folderSet.insert(folder).second)
		{
			folder->name = folderName;
			folder->children.clear();
		}
		folder->children.push_back(child);
	}

	item->children = toArray(folderSet);
}

void Database::CreateSubFolders(TreeItem* item, EFoldersType type)
{
	if (type == EFoldersType::EpicFolders)
	{
		CreateSubFolders(item, [](TreeItem* item)
			{
				auto issue = dynamic_cast<Issue*>(item);
				if (issue)
				{
					auto teamName = issue->customFields.values[11633];
					if (!teamName.empty())
					{
						return teamName;
					}
				}
				return std::string("{none}");
			}
		);
	}
}