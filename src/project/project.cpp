#include "project.h"
#include <raylib.h>
#include <components/Component.h>
#include <components/TextureEventArgs.h>
#include <components/TransformComponent.h>
#include <input/EInputTrigger.h>
#include <input/EInputType.h>
#include <input/InputAction.h>
#include <objects/GameObject.h>
#include <objects/IGameObject.h>
#include <Project.h>
#include <memory>

using namespace Atlas;

class Block : public GameObject {
public:
	class BlockRendererComponent : public Component {
	public:
		//std::shared_ptr<TransformComponent> transform;
		Model model;

		explicit BlockRendererComponent(Transform const& transform) : Component() {
			model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
		}

		void texture(TextureEventArgs* args) override {
			std::shared_ptr<TransformComponent> transform = getOwner()->obj->getComponent<TransformComponent>();

			DrawModel(model, transform->mTransform.translation, 1.0f, WHITE);
		}
	};

	Block(Transform const& transform) : GameObject() {
		addComponent<BlockRendererComponent>(BlockRendererComponent(transform));
		getComponent<TransformComponent>()->mTransform = transform;
	}

};
Project::Project() : BProject() {

}

void Project::preInit() {
	// WARNING: This MUST be called before any operations that use the file registry. The only exceptions is if you are adding load functions and adding extensions
	// that need to be loaded on registration.
	BProject::preInit();
}

void Project::postInit() {
	BProject::postInit();

	DisableCursor();
	float max = 10.0f;
	for(float x = 0.0f; x < max; x++) {
		for(float z = 0.0f; z < max; z++) {

			Vector3 pos = {x, 0, z};
			Transform transform = {
				pos,
				{0, 0, 0, 0},
				{1, 1, 1}
			};

			Block* block = new Block(transform);

			getAtlasEngine()->getRenderer().get()->addGameObject(block);
		}
	}
	
	InputAction* action = new InputAction("test", "renderer", KEY_R, EInputType::INPUT_KEYBOARD, EInputTrigger::INPUT_TRIGGER_PRESSED);
	getAtlasEngine()->getInputRegistry()->registerActionCallback(action, [](InputAction* inputAction) {
		Transform transform = {
			{0, 5, 0},
			{0, 0, 0, 0},
			{1, 1, 1}
		};
		Block* block = new Block(transform);
		GetAtlasEngine()->getRenderer()->addGameObject(block);
	});

}

int Project::update()
{
	BProject::update();
	return 0;
}

int Project::texture()
{
	BProject::texture();
	return 0;
}
