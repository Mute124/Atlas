#include "JSONFile.h"
#include <iostream>
#include "../Common.h"

// Just used for testing. 
#define ATLAS_TEST_PATH static_cast<std::string>("/game/data/assets/TestModel.json")

Atlas::JSONFile::JSONFile(const std::string& path) {
	simdjson::dom::parser parser;
	
	//this->mPaddedJSONString = PaddedString::load(path);
	//this->mJSONDocument = jsonParser.iterate(mPaddedJSONString);
	
	std::string filePath = ATLAS_GAME_DIR;
	filePath += path;

	this->mElement = parser.load(filePath);
}
