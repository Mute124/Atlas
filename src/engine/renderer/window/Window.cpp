#include "Window.h"
#include <raylib.h>

#include "../../conf/Config.h"
#include "../../Project.h"
#include "../../dbg/Logging.h"
#include "../../Common.h"
#include "WindowDecorations.h"
#include <string>

#include <GLFW/glfw3.h>

Atlas::Window::~Window() { 
	close(); 
}

Atlas::WindowDecorations const& Atlas::Window::GetDecorations() const {
	return mDecorations; 
}

void Atlas::Window::init(WindowDecorations* windowDecorations)
{
	const static std::string configPath = "\\data\\config\\Project.cfg";
	Log("Decorating window...");
	
	std::shared_ptr<RegisteredFile> file = GetAtlasEngine()->getFileSystemRegistry().get()->getFile(configPath);

	windowDecorations->mWindowTitle = LookupConfig(file, "projectWindowTitle");
	windowDecorations->mWindowWidth = LookupConfig(file, "projectWindowWidth");
	windowDecorations->mWindowHeight = LookupConfig(file, "projectWindowHeight");
	windowDecorations->mIconPath = LookupConfig(file, "projectWindowIcon");
	windowDecorations->mTargetFPS = LookupConfig(file, "projectTargetFPS");

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_INTERLACED_HINT);

	Log("Finalizing window initialization...");
	InitWindow(windowDecorations->mWindowWidth, windowDecorations->mWindowHeight, windowDecorations->mWindowTitle);

	Log("Loading and setting window icon...");

	const std::string assetDir = ATLAS_ASSET_DIR; // this is done like this because the macro expands to an expression rather than a string.
	
	FileSystemRegistry* fileSystemRegistry = GetAtlasEngine()->getFileSystemRegistry().get();
	
	file = fileSystemRegistry->getFile(windowDecorations->mIconPath);

	Image icon = file->get<Image>();

	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

	SetWindowIcons(&icon, 1);
	SetTargetFPS(windowDecorations->mTargetFPS);
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

std::any Atlas::Window::getNativePlatformHandle()
{
	return std::any();
}

std::any Atlas::Window::getNativeHandle()
{
	// this has to be void* because we cannot include the Windows.h header
	// due to name conflicts. Fortunately, this allows it to be cross-platform
	// since it's a pointer.
	void* handle = GetWindowHandle();
	
	return std::make_any<void*>(handle);
}

void Atlas::Window::requestAttention()
{
	
}
