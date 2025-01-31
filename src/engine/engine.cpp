// Do not delete this file!
#include "engine.h"

Atlas::EngineComp<Atlas::Renderer> Atlas::TechstormEngine::getRenderer()
{
    return mRenderer;
}

Atlas::EngineComp<Atlas::ConfigFileRegistry> Atlas::TechstormEngine::getConfigFileRegistry()
{
    return mConfigFileRegistry;
}

Atlas::EngineComp<Atlas::WindowDecorations> Atlas::TechstormEngine::getWindowDecorations()
{
    return mWindowDecorations;
}

Atlas::EngineComp<Atlas::ScriptingAPI> Atlas::TechstormEngine::getScriptingAPI()
{
    return mScriptingAPI;
}

Atlas::EngineComp<Atlas::FileSystemRegistry> Atlas::TechstormEngine::getFileSystemRegistry()
{
    return mFileSystemRegistry;
}

void Atlas::TechstormEngine::preInit()
{
}

void Atlas::TechstormEngine::init(int argc, char* argv[])
{
}

void Atlas::TechstormEngine::postInit()
{
}
