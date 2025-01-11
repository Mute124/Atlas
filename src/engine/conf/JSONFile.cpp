#include "JSONFile.h"

Atlas::JSONFile::JSONFile(const std::string& path) {
	JSONParser jsonParser;
	
	this->mPaddedJSONString = PaddedString::load(path);
	this->mJSONDocument = jsonParser.iterate(mPaddedJSONString);
}
