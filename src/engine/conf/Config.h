#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <libconfig.h++>
#include "../utils/Singleton.h"

namespace Atlas {
	class ConfigFileRegistry : public Singleton<ConfigFileRegistry> {
	public:
		ConfigFileRegistry();
		~ConfigFileRegistry();

		void init();
		void readConfigFiles();
		void readConfigFile(const std::string& name);
		void writeConfigFiles(); // Unimplemented

		void registerConfigFiles(const std::string& searchPath);
		void registerConfigFile(const std::string& name, const std::string& path);

		void unregisterConfigFiles();
		void unregisterConfigFile(const std::string& name);

		libconfig::Setting& lookup(const std::string& fileName, const std::string& lookupTarget);


	private:
		
	};

	ConfigFileRegistry& GetConfigFileRegistry();
	void InitializeConfigRegistry();
	libconfig::Setting& LookupConfig(const std::string& fileName, const std::string& lookupTarget);

	const char* GetConfigString(std::string fileName, std::string lookupTarget);
}
