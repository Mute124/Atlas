#include "GameUI.h"
#include <raylib.h>
#include <rlgl.h>
#include <string>

#ifdef ATLAS_ENABLE_HTML_UI

#include <RmlUi/Config/Config.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Math.h>
#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Vertex.h>

void Atlas::GameUIRenderer::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation)
{
	rlBegin(RL_TRIANGLES);
		
		for (int i = 0; i < num_vertices; i++)
		{
			rlColor4ub(vertices[i].colour.red, vertices[i].colour.green, vertices[i].colour.blue, vertices[i].colour.alpha);
			rlTexCoord2f(vertices[i].tex_coord.x, vertices[i].tex_coord.y);
			rlVertex2f(vertices[i].position.x + translation.x, vertices[i].position.y + translation.y);
		}

	rlEnd();
}

void Atlas::GameUIRenderer::EnableScissorRegion(bool enable)
{
}

void Atlas::GameUIRenderer::SetScissorRegion(int x, int y, int width, int height)
{
}

Atlas::GameUIFileInterface* Atlas::GameUI::getFileInterface()
{
	return mFileInterface;
}

void Atlas::GameUI::init()
{
	mSystemInterface = new UISystemInterface();
}

#pragma region FileInterfaceFunctions

Atlas::GameUIFileInterface::GameUIFileInterface(const std::string& root) : mRootPath(root)
{
}

Atlas::GameUIFileInterface::~GameUIFileInterface()
{
}

Rml::FileHandle Atlas::GameUIFileInterface::Open(const std::string& path)
{
	// Attempt to open the file relative to the application's root.
	FILE* fp = fopen((mRootPath + path).c_str(), "rb");
	if (fp != nullptr)
		return (Rml::FileHandle)fp;

	// Attempt to open the file relative to the current working directory.
	fp = fopen(path.c_str(), "rb");
	return (Rml::FileHandle)fp;
}

void Atlas::GameUIFileInterface::Close(Rml::FileHandle file)
{
	fclose((FILE*)file);
}

size_t Atlas::GameUIFileInterface::Read(void* buffer, size_t size, Rml::FileHandle file)
{
	return fread(buffer, 1, size, (FILE*)file);
}

bool Atlas::GameUIFileInterface::Seek(Rml::FileHandle file, long offset, int origin)
{
	return fseek((FILE*)file, offset, origin) == 0;
}

size_t Atlas::GameUIFileInterface::Tell(Rml::FileHandle file)
{
	return ftell((FILE*)file);
}

#pragma endregion 

#endif // ATLAS_ENABLE_HTML_UI