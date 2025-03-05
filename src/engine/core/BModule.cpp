#include "BModule.h"

#include <string>

#ifndef ATLAS_BARE_BONES

Atlas::BModule::BModule(std::string const& name, std::string const& category, bool enabled, bool callInitInConstructor)
	: IModule(enabled, callInitInConstructor), mAttributes(ModuleAttributes(name, category, enabled))
{
	this->setup();

	if (enabled && callInitInConstructor) {
		this->init();
	}
}

Atlas::BModule::BModule(std::string const& name, std::string const& category, bool enabled) : BModule(name, category, enabled, ATLAS_CALL_INIT_IN_CONSTRUCTOR)
{
	// Nothing to be done as it is handled in the constructor overload
}

Atlas::BModule::BModule(std::string const& name, std::string const& category) : BModule(name, category, ATLAS_DEFAULT_MODULE_ENABLED, ATLAS_CALL_INIT_IN_CONSTRUCTOR)
{
	// Nothing to be done as it is handled in the constructor overload
}

Atlas::BModule::~BModule()
{
	this->cleanup();
}

void Atlas::BModule::setup()
{
}

void Atlas::BModule::init()
{
}

void Atlas::BModule::update()
{
}

void Atlas::BModule::cleanup()
{
}

const Atlas::BModule::ModuleAttributes& Atlas::BModule::getAttributes() const {
	return this->mAttributes;
}

std::string const& Atlas::BModule::getName() const {
	return this->mAttributes.getName();
}

std::string const& Atlas::BModule::getCategory() const {
	return this->mAttributes.getCategory();
}

bool Atlas::BModule::isEnabled() const {
	return this->mAttributes.isEnabled();
}

#endif

Atlas::BModule::ModuleAttributes::ModuleAttributes()
	: ModuleAttributes("", "", ATLAS_DEFAULT_MODULE_ENABLED)
{
}

Atlas::BModule::ModuleAttributes::ModuleAttributes(std::string const& name, std::string const& category)
	: ModuleAttributes(name, category, ATLAS_DEFAULT_MODULE_ENABLED)
{
}

Atlas::BModule::ModuleAttributes::ModuleAttributes(std::string const& name, std::string const& category, bool enabled)
	: cEnabled(enabled), cName(name), cCategory(category)
{
}

std::string const& Atlas::BModule::ModuleAttributes::getName() const
{
	return this->cName;
}

std::string const& Atlas::BModule::ModuleAttributes::getCategory() const
{
	return this->cCategory;
}

bool Atlas::BModule::ModuleAttributes::isEnabled() const
{
	return this->cEnabled;
}
