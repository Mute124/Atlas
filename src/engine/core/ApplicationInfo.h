#pragma once
#include <string>

#include "Version.h"
namespace Atlas {
	struct ApplicationInfo {
		std::string name{ "Example Application" };
		
		Version vulkanVersion{ 1, 3, 0 };

		ApplicationInfo(const std::string& name, const Version& vulkanVersion)
			: name(name), vulkanVersion(vulkanVersion)
		{
		}

		ApplicationInfo() = default;
	};
}
