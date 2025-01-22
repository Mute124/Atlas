#pragma once
#include <iostream>
#include <memory>

#include "drawing/RenderArgs.h"
#include "drawing/TextureArgs.h"
#include "window/WindowDecorations.h"
namespace Atlas {
	class IDrawable;

	
	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T_DRAWABLE_REGISTRY"></typeparam>
	template<typename T_DRAWABLE_REGISTRY>
	class IRenderer abstract {
	public:

		virtual void initialize(WindowDecorations& decorations) = 0;

		virtual void render(RenderArgs* args) = 0;
		virtual void texture(TextureArgs* args) = 0;

		virtual std::shared_ptr<IDrawable> addDrawable(IDrawable* drawable) = 0;
		virtual void removeDrawable(IDrawable* drawable) = 0;

		virtual int cleanup() = 0;
	};
}