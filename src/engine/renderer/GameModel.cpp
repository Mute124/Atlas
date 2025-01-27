#include "GameModel.h"
#include "../fs/FileSystem.h"
#include "../conf/JSONFile.h"
#include <simdjson.h>
#include <iostream>

void Atlas::GameModel::load(std::string const& name)
{

}

std::string Atlas::GameModel::getModelName(JSONFile* data)
{
	std::string_view modelNameStrView = data->GetElement()["model"];
	std::string modelName = static_cast<std::string>(modelNameStrView);

	return modelName;
}

std::string Atlas::GameModel::getMaterialPath(JSONFile* data)
{
	return std::string();
}

Atlas::GameModel::GameModel(std::string const& name) {

	std::shared_ptr<RegisteredFile> assetDataFile = GetFile(name);
	JSONFile* assetData = assetDataFile->get<JSONFile*>();

	if (assetData == nullptr) {
		return;
	}

	//std::string modelName = getModelName(assetData);
	//std::string materialPath = getMaterialPath(assetData);
}