#pragma once
#include "../engine/Project.h"

#ifndef ATLAS_BUILD_EXAMPLES
	#define PROJECT_TYPENAME Project
#else
	// TODO: Add samples here
	#include "samples/SamplesProject.h"
	#define PROJECT_TYPENAME SamplesProject
#endif

#include <components/Component.h>
#include <objects/IGameObject.h>
#include <objects/GameObject.h>
#include <raylib.h>
#include <components/TextureEventArgs.h>
#include <components/TransformComponent.h>
#include <input/EInputTrigger.h>
#include <input/EInputType.h>
#include <input/InputAction.h>
#include <memory>

using namespace Atlas;

/**
 * @brief Write your project code here or do what you want with it. However,
 * you MUST implement the BProject interface somehow and set the project
 * reference! See BProject for more information on this.
 * 
 * @since v0.0.1
 * 
 * @sa @ref BProject
 */
class Project final : public Atlas::BProject {
public:
	

	Project();

	void preInit() override;

	void postInit() override;

	int update() override;

	int texture() override;
};