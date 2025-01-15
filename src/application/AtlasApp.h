#pragma once
#include "ApplicationUtils.h"
#include <optional>

namespace Atlas::Application {
	class AtlasApp {
	public:
		AtlasApp() {}

	private:

		PROJECT_TYPENAME mProject = PROJECT_TYPENAME();
		ScriptingAPI* mScriptingAPI = nullptr;

		void prepareProject() {
			Log("Pre-Initializing project...");
			mProject.preInit();
			Log("Project pre-initialized.");
			Log("Finishing Initialization...");
		}

		void initConfig() {
			
		}

		void initWindow(WindowDecorations& decorations) {
			Log("Decorating window...");
			decorations.title = LookupConfig("Project.cfg", "projectWindowTitle");
			SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_INTERLACED_HINT);

			Log("Finalizing window initialization...");
			InitWindow(decorations.width, decorations.height, decorations.title);

			Log("Loading and setting window icon...");
			const char* iconPath = TextFormat("%s%s", ATLAS_ASSET_DIR.c_str(), decorations.icon);

			Image icon = LoadImage(iconPath);
			ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

			SetWindowIcons(&icon, 1);
		}

		void initScripting() {
			this->mScriptingAPI = new ScriptingAPI();
			mScriptingAPI->initializeScripting(mProject.getLuaLibraries(), mProject.getLuaFunctions());
			mScriptingAPI->registerLua();
		}

		void handleFrame(PROJECT_TYPENAME& project) {
			project.getRenderer().update();

			project.texture();
			project.render();

			// This is here because it reduces CPU consumption
			ATLAS_THREAD_YIELD;
		}
	};
	
}