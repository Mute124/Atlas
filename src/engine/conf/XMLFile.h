#pragma once
#include <string>
#include <tinyxml2.h>

namespace Atlas {
	class XMLFile {
	private:
		tinyxml2::XMLDocument* mXMLDoc;
	public:

		XMLFile(std::string const& path) {
			this->mXMLDoc = new tinyxml2::XMLDocument();
			this->mXMLDoc->LoadFile(path.c_str());

		}
		
		tinyxml2::XMLElement* GetRoot() { return this->mXMLDoc->RootElement(); }
		tinyxml2::XMLDocument* GetDocument() { return this->mXMLDoc; }
	};
}