//#include "ApplicationUtils.h"
//
//static inline std::unordered_map<int, std::string> CreateThreadPriorityMap() {
//	std::unordered_map<int, std::string> map = {
//		{0, "Update"},
//		{1, "Physics"},
//		{2, ""}
//	};
//
//	return map;
//}
//
//Atlas::Application::GameThreadInfo Atlas::Application::CalculateGameThreadInfo(GameSettings& gameSettings)
//{
//	GameThreadInfo threadInfo;
//
//	if(gameSettings.cMaxThreads == -1) {
//		int maxThreads = std::thread::hardware_concurrency();
//	} else {
//		//threadInfo.cMaxThreads = gameSettings.threadCount;
//	}
//
//	return GameThreadInfo();
//}
//
//void Atlas::Application::DecorateWindow(WindowDecorations& decorations)
//{
//}
///*
//void Atlas::Application::GameMainThread::run(std::shared_ptr<Project> userProject)
//{
//	/*userProject->preInit();
//	InitializeConfigRegistry();
//	ConfigFileRegistry& configRegistry = GetConfigFileRegistry();
//
//	Renderer& renderer = userProject->getRenderer();
//	WindowDecorations& decorations = userProject->getWindowDecorations();
//
//	decorations.title = LookupConfig("Project.cfg", "projectWindowTitle");
//	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_INTERLACED_HINT);
//	InitWindow(decorations.width, decorations.height, decorations.title);
//
//	const char* iconPath = TextFormat("%s%s", ATLAS_ASSET_DIR.c_str(), decorations.icon);
//
//	Image icon = LoadImage(iconPath);
//	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
//
//	SetWindowIcons(&icon, 1);
//
//	this->idleUntilOthersAreReady();
//
//	while(!this->ShouldExit()) {
//		this->SetExit(!WindowShouldClose());
//		// This is here because it reduces CPU consumption
//		std::this_thread::yield();
//	}
//}
//
//void Atlas::Application::GameWorkThread::run(std::shared_ptr<Project> userProject)
//{
//	this->idleUntilOthersAreReady();
//	while(!this->ShouldExit()) {
//		// This is here because it reduces CPU consumption
//		std::this_thread::yield();
//	}
//}
//
//void Atlas::Application::GameUpdateThread::run(std::shared_ptr<Project> userProject)
//{
//	this->idleUntilOthersAreReady();
//	while(!this->ShouldExit()) {
//		userProject->preObjectUpdate();
//		userProject->objectUpdate();
//		userProject->postObjectUpdate();
//
//		userProject->prePhysicsUpdate();
//		userProject->physicsUpdate();
//		userProject->postPhysicsUpdate();
//
//		// This is here because it reduces CPU consumption
//		std::this_thread::yield();
//	}
//}
//*/