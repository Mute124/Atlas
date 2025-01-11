#pragma once
#include <raylib.h>
#include <string>
namespace Atlas {
	class GameModel {
	private:
		Model model;

	public:

		explicit GameModel(std::string const& name);

	};
}