#pragma once
#include <raylib.h>

#include <string>
#include <vector>


namespace Atlas {

	class JSONFile;

	class GameModel {
	private:

		Model model;

	
		void load(std::string const& name);

		std::string getModelName(JSONFile* data);
		std::string getMaterialPath(JSONFile* data);
	public:

		explicit GameModel(std::string const& name);

	};
}