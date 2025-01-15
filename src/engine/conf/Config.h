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
	/// <summary>
	/// 
	/// </summary>
	/// <seealso cref="Singleton&lt;ConfigFileRegistry&gt;" />
	class ConfigFileRegistry : public Singleton<ConfigFileRegistry> {
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="ConfigFileRegistry"/> class.
		/// </summary>
		ConfigFileRegistry();
				
		/// <summary>
		/// Finalizes an instance of the <see cref="ConfigFileRegistry"/> class.
		/// </summary>
		~ConfigFileRegistry();
		
		/// <summary>
		/// Initializes this instance.
		/// </summary>
		void init();
				
		/// <summary>
		/// Reads the configuration files.
		/// </summary>
		void readConfigFiles();
				
		/// <summary>
		/// Reads the configuration file.
		/// </summary>
		/// <param name="name">The name.</param>
		void readConfigFile(const std::string& name);
				
		/// <summary>
		/// Writes the configuration files.
		/// \note Unimplemented
		/// </summary>
		void writeConfigFiles(); 
		
		/// <summary>
		/// Registers the configuration files.
		/// </summary>
		/// <param name="searchPath">The search path.</param>
		void registerConfigFiles(const std::string& searchPath);
		
		/// <summary>
		/// Registers the configuration file.
		/// </summary>
		/// <param name="name">The name.</param>
		/// <param name="path">The path.</param>
		void registerConfigFile(const std::string& name, const std::string& path);
				
		/// <summary>
		/// Unregisters the configuration files.
		/// </summary>
		void unregisterConfigFiles();
		
		/// <summary>
		/// Unregisters the configuration file.
		/// </summary>
		/// <param name="name">The name.</param>
		void unregisterConfigFile(const std::string& name);
				
		/// <summary>
		/// Configurations the lookup.
		/// </summary>
		/// <param name="fileName">Name of the file.</param>
		/// <param name="lookupTarget">The lookup target.</param>
		/// <returns></returns>
		libconfig::Setting& configLookup(const std::string& fileName, const std::string& lookupTarget);


		JSONFile* getJsonFile(std::string const& fileName)
		{
			return GetFile(fileName).get()->get<JSONFile*>();
		}

	private:		
		/// <summary>
		/// Loads the configuration file.
		/// </summary>
		/// <param name="fileMeta">The file meta.</param>
		/// <returns></returns>
		static inline std::any LoadConfigFile(std::shared_ptr<Atlas::FileMeta> fileMeta);
				
		/// <summary>
		/// Loads a json file.
		/// </summary>
		/// <param name="fileMeta">The file meta.</param>
		/// <returns></returns>
		static inline std::any LoadJSONFile(std::shared_ptr<Atlas::FileMeta> fileMeta);
				
		/// <summary>
		/// Loads the XML file.
		/// </summary>
		/// <param name="fileMeta">The file meta.</param>
		/// <returns></returns>
		static inline std::any LoadXMLFile(std::shared_ptr<Atlas::FileMeta> fileMeta);
	};
	
	/// <summary>
	/// Gets the config file registry.
	/// </summary>
	/// <returns></returns>
	ConfigFileRegistry& GetConfigFileRegistry();

	/// <summary>
	/// Initializes the config registry.
	/// </summary>
	void InitializeConfigRegistry();

	/// <summary>
	/// Looks up the config.
	/// </summary>
	/// <param name="fileName">Name of the file.</param>
	/// <param name="lookupTarget">The lookup target.</param>
	/// <returns></returns>
	libconfig::Setting& LookupConfig(const std::string& fileName, const std::string& lookupTarget);

	/// <summary>
	/// Gets the config string.
	/// </summary>
	/// <param name="fileName">Name of the file.</param>
	/// <param name="lookupTarget">The lookup target.</param>
	/// <returns></returns>
	const char* GetConfigString(std::string fileName, std::string lookupTarget);
}
