#pragma once

#include <vector>
#include <memory>
#include <mutex>


#include "../core/Core.h"
#include "../core/Module.h"


#include "window/Window.h"
#include "backend/RenderingBackend.h"

namespace Atlas {
	
	class IRenderer {
	public:
		IGameWindow* mainGameWindow = nullptr;
		IRenderingBackend* renderingBackend = nullptr;

		IRenderer(IGameWindow* gameWindow, IRenderingBackend* backend) : mainGameWindow(gameWindow), renderingBackend(backend) {}

		virtual void init() = 0;
		virtual void update() = 0;
	};

#ifdef ATLAS_RENDERER_3D

#elif ATLAS_RENDERER_2D

#else

	#ifndef ATLAS_SUPPRESS_UNKOWN_RENDERER_ERROR
		#error "Unsupported renderer"
	#endif
#endif
}

