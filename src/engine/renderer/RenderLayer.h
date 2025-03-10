#pragma once
#include "ERenderLayerDrawTime.h"
#include "../objects/GameObjectRegistry.h"
#include <string>
namespace Atlas {

	using Depth = int;


	/*
	class GameObjectsList : public DoublyLinkedList<GameObject*> {
	public:
		ATLAS_DBL_LNK_LST_FUNC_OVERRIDE_DECL(GameObject*)

		void draw(GameObject* object);
	};

	class RenderLayer {

		void addGameObject(GameObject* gameObject) {
			if (gameObject != nullptr && gameObject->depth == this->mDepth) {
				mGameObjects.append(gameObject);
				mObjectsInLayer++;
			}
		}

		void draw();

	private:

		void drawGameObjectTraversal(GameObject* obj);

		Depth mDepth; // Same thing as an int
		int mObjectsInLayer = 0;
		DoublyLinkedList<GameObject*> mGameObjects;
	};
	*/

	class RenderLayer {
	private:
		const Depth mDepth;
		const float mCullDistance;
		const std::string mLayerName;

	public:

		
	};
}