//#include "Project.h"
//#include "physics/Physics.h"
//#include <raylib.h>
//#include "core/Common.h"
//#include "conf/Config.h"
//#include "dbg/Logging.h"
//#include "fs/FileMeta.h"
//#include "fs/FileSystem.h"
//#include "fs/FileSystemRegistry.h"
//#include "GameSettings.h"
//#include "modding/ScriptingAPI.h"
//#include "renderer/Renderer.h"
//#include "renderer/window/IWindow.h"
//#include "renderer/window/Window.h"
//#include "renderer/window/WindowDecorations.h"
//#include <any>
//#include <memory>
//#include <string>
//#include <sol/types.hpp>
//#include "input/Input.h"
//#include <thread>
//#include "core/AtlasEngine.h"
//#include "core/IProject.h"
//#include "core/ThreadSafeVariable.h"
//#include "input/InputRegistry.h"
//#include <stdexcept>
//
//using namespace Atlas;
//
///**
// * @brief A template helper function that is used internally within this file to create a thread safe variable
// * 
// * @tparam T The module type
// * 
// * @return A new thread safe variable of type T*
// * 
// * @since v0.0.9
// */
//template<typename T>
//Atlas::ThreadSafeVariable<T*> CreateThreadSafeVariable() {
//	return ThreadSafeVariable<T*>();
//}
//
//class TestModule : public IModule {
//
//};
//
//Atlas::ThreadSafeVariable<AtlasEngine*> Atlas::BProject::setupAtlas()
//{
//	AtlasEngine* engine = new AtlasEngine();
//
//
//	return ThreadSafeVariable<AtlasEngine*>(engine);
//}
//
//Atlas::BProject::BProject()  : IProject()
//{
//	mProject = std::shared_ptr<BProject>(this);
//
//	if (mAtlas.get() == nullptr) {
//		mAtlas = setupAtlas();
//	}
//}
//
//WindowDecorations& Atlas::BProject::getWindowDecorations() { 
//	return mWindowDecorations;
//}
//
//void Atlas::BProject::setWindowDecorations(WindowDecorations& windowDecorations) {
//	mWindowDecorations = windowDecorations; 
//}
//
//void Atlas::BProject::preInit() {
//
//	getAtlasEngine()->getLogger().get()->init(LoggerConfig());
//	Log("Registering file load functions...");
//	AddFileRegistryLoadFunction("png", [](std::shared_ptr<FileMeta> loadFunc) {
//		Image image = LoadImage(loadFunc->path.c_str());
//
//		if (image.data == nullptr) {
//			if (loadFunc->filename == "null.png") {
//				// This means that the null image was not found, which is a bad bad thing.
//				throw std::runtime_error("Null png image not found!");
//
//				// TODO: Figure out how to handle this.
//				return std::any_cast<Image>(image);
//			}
//
//			std::string nullPath = GetFilePath("null.png");
//
//			image = LoadImage(nullPath.c_str());
//		}
//
//		return std::any_cast<Image>(image);
//	});
//
//	AddFileRegistryLoadFunction("jpg", [](std::shared_ptr<FileMeta> loadFunc) {
//
//		Image image = LoadImage(loadFunc->path.c_str());
//
//		if (image.data == nullptr) {
//			if (loadFunc->filename == "null.jpg") {
//				// This means that the null image was not found, which is a bad bad thing.
//				throw std::runtime_error("Null jpg image not found!");
//
//				// TODO: Figure out how to handle this.
//				return std::any_cast<Image>(image);
//			}
//
//			std::string nullPath = GetFilePath("null.jpg");
//
//			image = LoadImage(nullPath.c_str());
//		}
//
//		return std::any_cast<Image>(image);
//	});
//
//	AddFileRegistryLoadFunction("jpeg", [](std::shared_ptr<FileMeta> loadFunc) {
//
//		Image image = LoadImage(loadFunc->path.c_str());
//
//		if (image.data == nullptr) {
//			if (loadFunc->filename == "null.jpeg") {
//				// This means that the null image was not found, which is a bad bad thing.
//				throw std::runtime_error("Null jpeg image not found!");
//
//				// TODO: Figure out how to handle this.
//				return std::any_cast<Image>(image);
//			}
//			
//			std::string nullPath = GetFilePath("null.jpeg");
//
//			image = LoadImage(nullPath.c_str());
//		}
//
//		return std::any_cast<Image>(image);
//	});
//
//	Log("Done registering file load functions...");
//
//	std::string gameDir = ATLAS_GAME_DIR;
//
//	FileSystemRegistry* registry = getAtlasEngine()->getFileSystemRegistry().get();
//	ConfigFileRegistry* configFileRegistry = getAtlasEngine()->getConfigFileRegistry().get();
//
//	Log("Initializing file system...");
//	registry->init(gameDir.c_str());
//	configFileRegistry->init();
//	getAtlasEngine()->getInputRegistry()->addGroup("working", std::make_shared<InputRegistry::InputActionRegistry>(InputRegistry::InputActionRegistry()));
//
//
//	/*this->mLuaLibraries.push_back(sol::lib::base);
//
//	ScriptingAPI* scriptingAPI = getAtlasEngine()->getScriptingAPI().get();
//
//	scriptingAPI->initializeScripting(this->getLuaLibraries(), this->getLuaFunctions()); // getAtlasEngine()->getLuaLibraries(), getAtlasEngine()->getLuaFunctions()
//	scriptingAPI->registerLua();*/
//
//}
//
//void Atlas::BProject::init(int argc, char* argv[]) {
//
//}
//
//void Atlas::BProject::postInit() {
//	IWindow* window = getAtlasEngine()->getWindow().get();
//	Renderer* renderer = getAtlasEngine()->getRenderer().get();
//	
//	getAtlasEngine()->getInputRegistry()->addGroup("renderer", std::make_shared<InputRegistry::InputActionRegistry>(InputRegistry::InputActionRegistry()));
//	
//	//AllocatedPhysicsResources resources = AllocatedPhysicsResources();
//	//getAtlasEngine()->getPhysicsEngine().get()->init(resources);
//
//	WindowDecorations* windowDecor = new WindowDecorations();
//	window->init(windowDecor);
//	renderer->init();
//}
//
//void Atlas::BProject::initRenderer() {
//}
//
//int Atlas::BProject::run(int argc, char* argv[]) {
//	int code = 0;
//	
//	bool shouldClose = false;
//	while (!shouldClose) {
//		getAtlasEngine()->getInputRegistry().get()->checkAll("renderer");
//		shouldClose = getAtlasEngine()->getWindow().get()->shouldClose();
//
//		draw();
//	}
//
//	// This is here because it reduces CPU usage (by alot!).
//	std::this_thread::yield();
//	return code;
//}
//
//int Atlas::BProject::update()
//{
//	int code = 0;
//
//	getAtlasEngine()->getInputRegistry().get()->checkAll("update");
//	getAtlasEngine()->getRenderer().get()->updateObjects();
//	return code;
//}
//
//int Atlas::BProject::workingUpdate()
//{
//	int code = 0;
//
//	getAtlasEngine()->getInputRegistry().get()->checkAll("working");
//
//	return code;
//}
//
//int Atlas::BProject::prePhysicsUpdate() {
//	int code = 0;
//
//	return code;
//}
//
//int Atlas::BProject::physicsUpdate() {
//
//	//PhysicsEngine* physicsEngine = getAtlasEngine()->getPhysicsEngine().get();
//	//physicsEngine->update(1.0f / 60.0f);
//
//	return 0; 
//}
//
//int Atlas::BProject::postPhysicsUpdate() { 
//	return 0; 
//}
//
//int Atlas::BProject::preObjectUpdate() { 
//	return 0; 
//}
//
//int Atlas::BProject::objectUpdate() { 
//	return 0; 
//}
//
//int Atlas::BProject::postObjectUpdate() { 
//	return 0; 
//}
//
//int Atlas::BProject::texture() {
//	getAtlasEngine()->getRenderer().get()->texture(getAtlasEngine()->getRenderer().get()->mCamera);
//	return 0; 
//}
//
//int Atlas::BProject::render() { 
//	getAtlasEngine()->getRenderer().get()->render(getAtlasEngine()->getRenderer().get()->mCamera);
//	return 0;
//}
//
//int Atlas::BProject::draw()
//{
//
//	getAtlasEngine()->getRenderer().get()->update();
//	texture();
//	render();
//
//	return 0;
//}
//
//int Atlas::BProject::cleanup(int exitCode) { 
//	ThreadSafeVariable<AtlasEngine*> engine = getAtlasEngine();
//
//	engine->getRenderer().get()->cleanup();
//*	engine->getPhysicsEngine()->cleanup();
//	engine->getInputRegistry()->cleanup();
//	engine->getLogger()->cleanup();*/
//
//	return exitCode; 
//}
//
//std::shared_ptr<BProject> Atlas::BProject::GetProject() {
//	
//	return mProject;
//}
//
//ThreadSafeVariable<AtlasEngine*> Atlas::BProject::getAtlasEngine()
//{
//	return mAtlas;
//}
//
//
////---------------------------------------------------------------------------
////							AtlasEngine
////----------------------------------------------------------------------------
//
//ThreadSafeVariable<AtlasEngine*> Atlas::GetAtlasEngine() {
//	return BProject::GetProject()->getAtlasEngine(); 
//}
