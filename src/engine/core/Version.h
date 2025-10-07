#pragma once
#include <cstdint>

namespace Atlas {
	struct Version {
		uint32_t major = -1;
		uint32_t minor = -1;
		uint32_t patch = -1;

		Version(uint32_t major, uint32_t minor, uint32_t patch) : major(major), minor(minor), patch(patch) {}

		Version(uint32_t major, uint32_t minor) : major(major), minor(minor), patch(0) {}

		Version() = default;

		~Version() = default;
	};
}
