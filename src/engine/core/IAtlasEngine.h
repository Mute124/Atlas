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

#include "ThreadSafeVariable.h"
#include <unordered_map>
#include <unordered_set>

namespace Atlas {
	class IAtlasEngine {
	public:
		virtual ThreadSafeVariable<ConfigFileRegistry*> getConfigFileRegistry() = 0;
		virtual ThreadSafeVariable<ScriptingAPI*> getScriptingAPI() = 0;
		virtual ThreadSafeVariable<IWindow*> getWindow() = 0;
		virtual ThreadSafeVariable<Renderer*> getRenderer() = 0;
		virtual ThreadSafeVariable<GameSettings*> getGameSettings() = 0;
		virtual ThreadSafeVariable<FileSystemRegistry*> getFileSystemRegistry() = 0;
		virtual ThreadSafeVariable<PhysicsEngine*> getPhysicsEngine() = 0;
		virtual ThreadSafeVariable<InputRegistry*> getInputRegistry() = 0;
		virtual ThreadSafeVariable<Logger*> getLogger() = 0;

		virtual void setConfigFileRegistry(ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry) = 0;
		virtual void setScriptingAPI(ThreadSafeVariable<ScriptingAPI*> scriptingAPI) = 0;
		virtual void setWindow(ThreadSafeVariable<IWindow*> window) = 0;
		virtual void setRenderer(ThreadSafeVariable<Renderer*> renderer) = 0;
		virtual void setGameSettings(ThreadSafeVariable<GameSettings*> gameSettings) = 0;
		virtual void setFileSystemRegistry(ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry) = 0;
		virtual void setPhysicsEngine(ThreadSafeVariable<PhysicsEngine*> physicsEngine) = 0;
		virtual void setInputRegistry(ThreadSafeVariable<InputRegistry*> inputRegistry) = 0;
		virtual void setLogger(ThreadSafeVariable<Logger*> logger) = 0;
	};
}