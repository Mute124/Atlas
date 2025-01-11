#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <libconfig.h++>
#include <simdjson.h>

#include "../utils/Singleton.h"
#include "../dbg/ELogLevel.h"
#include "../dbg/Logging.h"
#include "../fs/FileSystem.h"
#include <any>
#include <memory>

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

		libconfig::Setting& configLookup(const std::string& fileName, const std::string& lookupTarget);


	private:
		static inline std::any LoadConfigFile(std::shared_ptr<Atlas::FileMeta> fileMeta);

		static inline std::any LoadJSONFile(std::shared_ptr<Atlas::FileMeta> fileMeta);
	};

	ConfigFileRegistry& GetConfigFileRegistry();
	void InitializeConfigRegistry();
	libconfig::Setting& LookupConfig(const std::string& fileName, const std::string& lookupTarget);

	const char* GetConfigString(std::string fileName, std::string lookupTarget);
}
