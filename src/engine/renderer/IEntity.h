#pragma once
#include "IGameObject.h"

namespace Atlas {

	class IEntity abstract : public IGameObject {
	public:
		/// <summary>
		/// Updates this instance and is called every frame before texturing/rendering. 
		/// \note Keep in mind that this will run on the update thread, so make sure what you are doing is <b>thread safe</b>.
		/// </summary>
		/// <inheritdoc />
		virtual void update() = 0;

		/// <summary>
		/// Called right after the update function, but before texturing/rendering. 
		/// \note Keep in mind that this will run on the update thread, so make sure what you are doing is <b>thread safe</b>.
		/// </summary>
		/// <inheritdoc />
		virtual void postUpdate() = 0;
	};

}