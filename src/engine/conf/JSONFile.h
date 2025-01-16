#pragma once
#include <string>
#include <iostream>
#include <any>
#include <simdjson.h>

namespace Atlas {
	using PaddedString = simdjson::padded_string;
	using JSONDocument = simdjson::ondemand::document;
	using JSONParser = simdjson::ondemand::parser;
	using JSONElement = simdjson::dom::element;

	/// <summary>
	/// Represents a JSON file for Atlas' config system. Since there were issues with simply loading <see cref="simdjson::ondemand::document"/>s, this class was created to solve that. 
	/// \important This class will be STORED AS A POINTER in the file system.
	/// </summary>
	class JSONFile {
	private:
		friend class ConfigFileRegistry;

		PaddedString mPaddedJSONString;
		JSONDocument mJSONDocument;
		JSONElement mElement;
	public:
		

		/// <summary>
		/// Initializes a new instance of the <see cref="JSONFile"/> class by loading the file at the specified path. This does not load the file into the file system.	
		/// </summary>
		/// <param name="path">The path.</param>
		explicit JSONFile(const std::string& path);
		
		/// <summary>
		/// Gets the json document.
		/// </summary>
		/// <returns></returns>
		JSONDocument& GetJSONDocument() { return mJSONDocument; }
		
		JSONElement& GetElement() { return mElement; }

		/// <summary>
		/// Gets the padded json string.
		/// </summary>
		/// <returns></returns>
		PaddedString& GetPaddedJSONString() { return mPaddedJSONString; }
	};
}