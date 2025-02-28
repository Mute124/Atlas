#pragma once
#include "../objects/IGameObject.h"

namespace Atlas {
	
	class ICuller abstract {
	public:

		ICuller() = default;

		virtual bool shouldCull(IGameObject* gameObject) = 0;
		virtual float getCullDistance() = 0;


	};
}