#pragma once
#include "../engine/Project.h"

#include <objects/GameObject.h>
#include <components/Component.h>

#ifndef ATLAS_BUILD_EXAMPLES
	#define PROJECT_TYPENAME Project
#else
	// TODO: Add samples here
	#include "samples/SamplesProject.h"
	#define PROJECT_TYPENAME SamplesProject
#endif
#include <raylib.h>
#include <components/TextureEventArgs.h>
#include <objects/IGameObject.h>

#include <raymath.h>
using namespace Atlas;
/// <summary>
/// Write your project code here or do what you want with it. However,
/// you MUST implement the BProject interface somehow and set the project reference!
/// See <see cref="Atlas::BProject" /> for more information on this.
/// </summary>
/// <inheritdoc />
class Project final : public Atlas::BProject {
private:
	class CubeRenderer final : public Component {
	public:
		int count = 0;

		explicit CubeRenderer(IGameObject* mOwner) : Component(mOwner) {
		
		}
		CubeRenderer() : Component() {}

		void texture(TextureEventArgs* args) override {
			std::shared_ptr<ObjectHolder> owner = getOwner();
			std::shared_ptr<TransformComponent> transform = owner->obj->getComponent<TransformComponent>();
			DrawCube(transform->pos, 2, 2, 2, GREEN);
		}

		void update(UpdateEventArgs* args) override {
			std::shared_ptr<ObjectHolder> owner = getOwner();
			std::shared_ptr<TransformComponent> transform = owner->obj->getComponent<TransformComponent>();

			//transform->pos.y = sin(count++) * 0.3f;
			transform->pos.y += 0.1f;
			//std::cout << transform->pos.y << std::endl;
			//getOwner()->obj->position.y += 0.1f;
		}
	};

	class RenderComponent : public Component {
	public:
		Model model;

		explicit RenderComponent(IGameObject* mOwner) : Component(mOwner) {
			model = LoadModelFromMesh(GenMeshCone(10, 10, 20));
		}

		RenderComponent() : Component() {
			model = LoadModelFromMesh(GenMeshCone(10, 10, 20));
		}

		void texture(TextureEventArgs* args) override {
			std::shared_ptr<ObjectHolder> owner = getOwner();
			std::shared_ptr<TransformComponent> transform = owner->obj->getComponent<TransformComponent>();

			DrawModel(model, transform->pos, 1.0f, RED);
		}
	};

	class Cube : public GameObject {
	public:

		Cube() : GameObject() {
			addComponent<CubeRenderer>();
		}
	};

	class FPS : public GameObject {
	private:
		class FPSDrawingComponent : public Component {
		public:

			void render(RenderEventArgs* args) override {
				DrawFPS(200, 200);
			}

			void texture(TextureEventArgs* args) override {
				if (IsKeyPressed(KEY_C)) {
					GetAtlasEngine()->getRenderer()->addGameObject(new Cube());
				}
			}
		};
	public:

		FPS() : GameObject() {
			addComponent<FPSDrawingComponent>();
		}


	};
public:
	static inline std::shared_ptr<Project> instance = nullptr;
	Project() : BProject() {
	//	instance = std::make_shared<Project>(*this);
	}
	

	static inline void SpawnCube(InputAction* args) {
		instance->getAtlasEngine()->getRenderer()->addGameObject(new Cube());
	}

	void preInit() override {
		// WARNING: This MUST be called before any operations that use the file registry. The only exceptions is if you are adding load functions and adding extensions
		// that need to be loaded on registration.
		BProject::preInit();
	}

	void postInit() override {
		BProject::postInit();
		DisableCursor();
		getAtlasEngine()->getRenderer()->addGameObject(new FPS());

		//InputAction* action = new InputAction("spawn Object", KeyboardKey::KEY_C, EInputType::INPUT_KEYBOARD, EInputTrigger::INPUT_TRIGGER_PRESSED);
		//instance = std::make_shared<Project>(*this);
		//getAtlasEngine()->getInputRegistry()->registerActionCallback(action, SpawnCube);
		
	}
};
