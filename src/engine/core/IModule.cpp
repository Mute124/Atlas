#include "IModule.h"

Atlas::IModule::ModuleAttributes::ModuleAttributes() 
	: cName(""), cCategory(""), cEnabled(false) {

}

Atlas::IModule::ModuleAttributes::ModuleAttributes(std::string const& name, std::string const& category)
	: cName(name), cCategory(category), cEnabled(false) {

}

Atlas::IModule::ModuleAttributes::ModuleAttributes(std::string const& name, std::string const& category, bool enabled)
	: cName(name), cCategory(category), cEnabled(enabled) {

}

std::string const& Atlas::IModule::ModuleAttributes::getName() const
{
	return this->cName;
}

std::string const& Atlas::IModule::ModuleAttributes::getCategory() const
{
	return this->cCategory;
}

bool Atlas::IModule::ModuleAttributes::isEnabled() const
{
	return this->cEnabled;
}
