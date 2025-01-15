#include "GameModel.h"
#include "../fs/FileSystem.h"
#include 
Atlas::GameModel::GameModel(std::string const& name) {
	std::shared_ptr<RegisteredFile> assetDataFile = GetFile("TestModel.json");
}