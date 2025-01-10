#include "Project.h"
using namespace Atlas;

WindowDecorations& Atlas::IProject::getWindowDecorations() { return mWindowDecorations; }

void Atlas::IProject::setWindowDecorations(WindowDecorations& windowDecorations) { mWindowDecorations = windowDecorations; }

void Atlas::IProject::init(int argc, char* argv[]) {
	std::cout << "Initializing Project" << std::endl;
	mRenderer.initialize();
	auto resources = AllocatedPhysicsResources();
	PhysicsEngine::Instance().init(resources);

}

void Atlas::IProject::postInit() {}

void Atlas::IProject::initRenderer() {
}

int Atlas::IProject::run(int argc, char* argv[]) {
	return 0;
}
int Atlas::IProject::update()
{
	return 0;
}
int Atlas::IProject::prePhysicsUpdate() { return 0; }

int Atlas::IProject::physicsUpdate() {
	PhysicsEngine::Instance().update(1.0f / 60.0f);
	return 0; 
}

int Atlas::IProject::postPhysicsUpdate() { return 0; }

int Atlas::IProject::preObjectUpdate() { return 0; }

int Atlas::IProject::objectUpdate() { return 0; }

int Atlas::IProject::postObjectUpdate() { return 0; }

int Atlas::IProject::texture() { 
	mRenderer.texture(mRenderer.mCamera);
	return 0; 
}

int Atlas::IProject::render() { 
	mRenderer.render(mRenderer.mCamera);
	return 0;
}

int Atlas::IProject::cleanup(int exitCode) { return exitCode; }

IProject* Atlas::IProject::GetProject() {
	return ProjectReference::Instance().project;
}

/// <summary>
/// Sets the project reference.
/// </summary>
/// <param name="project">The project.</param>
inline void Atlas::IProject::ProjectReference::setProjectReference(IProject* project) {
	if (project != nullptr) {
		this->project = project;
	}
}
