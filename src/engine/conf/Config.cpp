#include "../fs/FileSystem.h"
#include "Config.h"
#include "libconfig.h++"
#include <any>
#include <memory>
#include <string>
#include "../dbg/Logging.h"

std::any LoadConfigFile(std::shared_ptr<Atlas::FileMeta> fileMeta) {
	using namespace Atlas;

	return std::make_any<uint16_t>(3);
}

Atlas::ConfigFileRegistry::ConfigFileRegistry()
{


}

Atlas::ConfigFileRegistry::~ConfigFileRegistry()
{
}

void Atlas::ConfigFileRegistry::init() {
	Log("Initializing config registry.");

	// only show this in the log file
	Log("Setting config file load functions.", ELogLevel::TRACE);
	AddFileRegistryLoadFunction("cfg", [](std::shared_ptr<FileMeta> fileMeta) {
		Log("Loading config file: " + fileMeta->path, ELogLevel::TRACE);
		libconfig::Config* conf = new libconfig::Config();
		conf->readFile(fileMeta->path);
		return std::make_any<libconfig::Config*>(conf);
	});
}

void Atlas::ConfigFileRegistry::readConfigFiles()
{
}

void Atlas::ConfigFileRegistry::readConfigFile(const std::string& name)
{
	std::shared_ptr<RegisteredFile> file = GetFile(name);
}

void Atlas::ConfigFileRegistry::writeConfigFiles()
{
}

void Atlas::ConfigFileRegistry::registerConfigFiles(const std::string& searchPath)
{
}

void Atlas::ConfigFileRegistry::registerConfigFile(const std::string& name, const std::string& path)
{
}

void Atlas::ConfigFileRegistry::unregisterConfigFiles()
{
}

void Atlas::ConfigFileRegistry::unregisterConfigFile(const std::string& name)
{
}

libconfig::Setting& Atlas::ConfigFileRegistry::lookup(const std::string& fileName, const std::string& lookupTarget)
{
	libconfig::Config const* conf = GetFile(fileName).get()->get<libconfig::Config*>();
	libconfig::Setting& setting = conf->lookup(lookupTarget);

	Log("Looking up value in :" + fileName + " for key: " + lookupTarget + " and found value: " + setting.c_str(), ELogLevel::TRACE);
	
	return setting;
	
}

/*
libconfig::Config& Atlas::ConfigFileRegistry::operator[](const std::string& name)
{
	// TODO: insert return statement here
}
*/

Atlas::ConfigFileRegistry& Atlas::GetConfigFileRegistry() { return ConfigFileRegistry::Instance(); }

void Atlas::InitializeConfigRegistry() { ConfigFileRegistry::Instance().init(); }

libconfig::Setting& Atlas::LookupConfig(const std::string& fileName, const std::string& lookupTarget) { return ConfigFileRegistry::Instance().lookup(fileName, lookupTarget); }

const char* Atlas::GetConfigString(std::string fileName, std::string lookupTarget) { return LookupConfig(fileName, lookupTarget).c_str(); }
