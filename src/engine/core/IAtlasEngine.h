#pragma once
#include "../renderer/Renderer.h"
#include "../renderer/window/IWindow.h"
#include "../modding/ScriptingAPI.h"
#include "../GameSettings.h"

#include "../conf/Config.h"
#include "../physics/Physics.h"
#include "../fs/FileSystemRegistry.h"
#include "../dbg/Logging.h"
#include "../input/InputRegistry.h"

namespace Atlas {
	class IAtlas {
	public:
		virtual ConfigFileRegistry* getConfigFileRegistry() = 0;
		virtual ScriptingAPI* getScriptingAPI() = 0;
		virtual IWindow* getWindow() = 0;
		virtual Renderer* getRenderer() = 0;
		virtual GameSettings* getGameSettings() = 0;
		virtual FileSystemRegistry* getFileSystemRegistry() = 0;
		virtual PhysicsEngine* getPhysicsEngine() = 0;
		virtual InputRegistry* getInputRegistry() = 0;
		virtual Logger* getLogger() = 0;

		virtual void setConfigFileRegistry(ConfigFileRegistry* configFileRegistry) = 0;
		virtual void setScriptingAPI(ScriptingAPI* scriptingAPI) = 0;
		virtual void setWindow(IWindow* window) = 0;
		virtual void setRenderer(Renderer* renderer) = 0;
		virtual void setGameSettings(GameSettings* gameSettings) = 0;
		virtual void setFileSystemRegistry(FileSystemRegistry* fileSystemRegistry) = 0;
		virtual void setPhysicsEngine(PhysicsEngine* physicsEngine) = 0;
		virtual void setInputRegistry(InputRegistry* inputRegistry) = 0;
		virtual void setLogger(Logger* logger) = 0;
	};
}