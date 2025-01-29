#include "Window.h"
#include <raylib.h>

#include "../../conf/Config.h"
#include "../../Project.h"
#include "../../dbg/Logging.h"
#include "../../Common.h"
#include "WindowDecorations.h"
#include <string>

Atlas::Window::~Window() { close(); }

void Atlas::Window::init(WindowDecorations* windowDecorations)
{
	const static std::string configPath = "\\data\\config\\Project.cfg";
	Log("Decorating window...");
	
	std::shared_ptr<RegisteredFile> file = GetAtlasEngine()->getFileSystemRegistry()->getFile(configPath);

	windowDecorations->title = LookupConfig(file, "projectWindowTitle");
	windowDecorations->width = LookupConfig(file, "projectWindowWidth");
	windowDecorations->height = LookupConfig(file, "projectWindowHeight");
	windowDecorations->icon = LookupConfig(file, "projectWindowIcon");
	windowDecorations->targetFPS = LookupConfig(file, "projectTargetFPS");

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_INTERLACED_HINT);

	Log("Finalizing window initialization...");
	InitWindow(windowDecorations->width, windowDecorations->height, windowDecorations->title);

	Log("Loading and setting window icon...");

	const std::string assetDir = ATLAS_ASSET_DIR; // this is done like this because the macro expands to an expression rather than a string.
	
	FileSystemRegistry* fileSystemRegistry = GetAtlasEngine()->getFileSystemRegistry();
	
	file = fileSystemRegistry->getFile(windowDecorations->icon);

	Image icon = file->get<Image>();
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

	SetWindowIcons(&icon, 1);
	SetTargetFPS(windowDecorations->targetFPS);
}

void Atlas::Window::update()
{
	
}

bool Atlas::Window::shouldClose()
{
    return WindowShouldClose();
}

void Atlas::Window::close()
{
	CloseWindow();
}