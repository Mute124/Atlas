#pragma once
#include "../core/Common.h"
namespace Atlas {

	class Mod {
	private:
		string mName;
		string mVersion;
		string mAuthor;
		string mDescription;
	
	public:
		Mod();
		~Mod();
	};
}