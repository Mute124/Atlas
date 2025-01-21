#include "ApplicationUtils.h"
#include <raylib.h>
#include <Common.h>
#include <conf/Config.h>
#include <dbg/Logging.h>
#include <modding/ScriptingAPI.h>
#include <renderer/Renderer.h>
#include <renderer/window/WindowDecorations.h>
#include <project.h>

using namespace Atlas;

void InitWindow(WindowDecorations& decorations) {

	Log("Decorating window...");
	decorations.title = LookupConfig("Project.cfg", "projectWindowTitle");
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_INTERLACED_HINT);

	Log("Finalizing window initialization...");
	InitWindow(decorations.width, decorations.height, decorations.title);

	Log("Loading and setting window icon...");

	const std::string assetDir = ATLAS_ASSET_DIR; // this is done like this because the macro expands to an expression rather than a string.
	const char* iconPath = TextFormat("%s%s", assetDir.c_str(), decorations.icon);

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
	//Log("Pre-Initializing project...");
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

	InitScripting(scriptingAPI, project);

	Log("Done setting up scripting API.");

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