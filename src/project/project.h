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

using namespace Atlas;

/// <summary>
/// Write your project code here or do what you want with it. However,
/// you MUST implement the BProject interface somehow and set the project reference!
/// See <see cref="Atlas::BProject" /> for more information on this.
/// </summary>
/// <inheritdoc />
class Project final : public Atlas::BProject {
public:



	Project() : BProject() {
	
	}

	void preInit() override {
		// WARNING: This MUST be called before any operations that use the file registry. The only exceptions is if you are adding load functions and adding extensions
		// that need to be loaded on registration.
		BProject::preInit();
	}

	void postInit() override {
		BProject::postInit();
		DisableCursor();
		//getAtlasEngine()->getRenderer()->addGameObject(new Cube());
	}
};
