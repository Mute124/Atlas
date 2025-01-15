#include "ApplicationUtils.h"
#include <raylib.h>
#include <Common.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <conf/Config.h>
#include <dbg/Logging.h>
>>>>>>> main
#include <modding/ScriptingAPI.h>
#include <renderer/Renderer.h>
#include <renderer/WindowDecorations.h>
#include <project.h>

using namespace Atlas;

void InitWindow(WindowDecorations& decorations) {

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

void InitScripting(ScriptingAPI& scriptingAPI, PROJECT_TYPENAME& project) {
	scriptingAPI.initializeScripting(project.getLuaLibraries(), project.getLuaFunctions());
	scriptingAPI.registerLua();
}

void HandleFrame(PROJECT_TYPENAME& project) {
	project.getRenderer().update();

	project.texture();
	project.render();

	// This is here because it reduces CPU consumption
	ATLAS_THREAD_YIELD;
}

/*
class UI : public GameObject {
public:


	Rml::Context* context = nullptr;
	UI(Rml::Context* context) : context(context), GameObject() {
	}
	void render() override {
		context->Render();
	}

};*/

int main(int argc, char* argv[]) {
<<<<<<< HEAD
=======
#include <crtdbg.h>
#include <modding/ScriptingAPI.h>
#include <project.h>
#include <raylib.h>
#include <renderer/Renderer.h>
#include <renderer/WindowDecorations.h>

int main(int argc, char* argv[]) {
#ifdef _DEBUG
	// Enable leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	using namespace Techstorm;

>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
	
	//Log("Pre-Initializing project...");
>>>>>>> main
	PROJECT_TYPENAME project = PROJECT_TYPENAME();
	project.preInit();
	Log("Project pre-initialized.");

	Log("Finishing Initialization...");

	InitializeConfigRegistry();
	ConfigFileRegistry& configRegistry = GetConfigFileRegistry();
	
	Renderer& renderer = project.getRenderer();
	WindowDecorations& decorations = project.getWindowDecorations();
	
	Log("Setting up scripting API...");
	ScriptingAPI scriptingAPI;

<<<<<<< HEAD
	InitScripting(scriptingAPI, project);

<<<<<<< HEAD
	InitWindow(decorations);
=======
	ScriptingAPI scriptingAPI;

	scriptingAPI.InitializeScripting(project.getLuaLibraries(), project.getLuaFunctions());
	scriptingAPI.RegisterLua();

	InitWindow(decorations.width, decorations.height, decorations.title);
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc

	const char* iconPath = TextFormat("%s%s", TS_ASSET_DIR.c_str(), decorations.icon);

	Image icon = LoadImage(iconPath);
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

	SetWindowIcons(&icon, 1);
=======
	Log("Done setting up scripting API.");
>>>>>>> main

	Log("Initializing window...");
	InitWindow(decorations);
	
	Log("Done initializing window.");

	Log("Finishing project's initialization...");
	project.init(argc, argv);
	project.postInit();

	Log("Done finishing project's initialization.");

	Log("Launching threads.");

	Atlas::Application::FrameManager& manager = Atlas::Application::FrameManager::Instance();

	SetTargetFPS(decorations.targetFPS);

	manager.launchThreads(project);
	
	Log("Initialization is now finished, starting main loop.");
	while (!WindowShouldClose()) {
		HandleFrame(project);
	}
	
	Log("Shutting down...");

	Log("Cleaning up...");
	project.cleanup(0);

	manager.killThreads();

	Log("Done cleaning up.");

	CloseWindow();

	Log("Done shutting down. Goodbye!");
	return 0;
}