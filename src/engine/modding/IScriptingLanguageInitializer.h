#pragma once

namespace Atlas {
	class IScriptingLanguageInitializer abstract {
	public:

		virtual void init(int argc, char* argv[]) = 0;

	};
}