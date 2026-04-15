/**************************************************************************************************
 * @file Version.h
 * 
 * @brief Contains the @ref Version class and a custom `std::formatter` class that will automatically
 * format @ref Version objects into a string.
 * 
 * @todo The date is not accurate and should be updated
 * 
 * @date April 2026
 * 
 * @since v0.0.1
 * 
 *  Copyright 2024 Mute124
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License. 
 * ***************************************************************************************************/
#pragma once

#include <cstdint>
#include <string>
#include <format>
#include <compare> // Used for operator<=>

namespace Atlas {
	/**
	 * @brief A data-holding struct representing a software version number (or really any version number)
	 * using @b three @c uint32_t values for the @a major, @a minor and @a patch version numbers. The
	 * @c patch value is @b optional, and will automatically default to @b 1 if not specified in the class'
	 * constructor. It is recommended that the values you use as the version numbers are @b positive integers.
	 * Another note is that this struct can be used directly within a `std::format()` call as this file contains
	 * a `std::formatter` class that will automatically format the version number into a string (@ref toString()).
	 * 
	 * @note If the @a patch version number is not a concern, or you are only using the @a major and @a minor,
	 * then you can use the @ref Version(uint32_t major, uint32_t minor) constructor instead of the full
	 * @ref Version(uint32_t major, uint32_t minor, uint32_t patch) constructor. 
	 * 
	 * @since v0.0.1
	 */
	struct Version {
		/**
		 * @brief A @c uint32_t value representing the @a major version number. By default, this is set to @b 0.
		 * Consider this as the primary version number determining the compatibility of the software with other
		 * software.
		 * 
		 * @since v0.0.1
		 */
		uint32_t majorVersion{ 0 };
		
		/**
		 * @brief A @c uint32_t value representing the @a minor version number. By default, this is set to @b 0.
		 * Consider this as the secondary version number denoting a minor and/or smaller feature release.
		 * 
		 * @since v0.0.1
		 */
		uint32_t minorVersion{ 0 };

		/**
		 * @brief A @c uint32_t value representing the @a patch version number. By default, this is set to @b 1.
		 * Consider this as the tertiary version number denoting a bugfix, or as the name suggests, a patch.
		 * 
		 * @since v0.0.1
		 */
		uint32_t patchVersion{ 1 };

		/**
		 * @brief A three-argument constructor that will fill in all three (3) values with the given parameters.
		 * As an important note, this function will @b not throw any exceptions, validate the values passed in, or
		 * do any other checks.
		 * 
		 * @param major The current @b major release number to be used.
		 * @param minor The current @b minor release number to be used.
		 * @param patch The current bug, hotfix, and/or patch release number to be used.
		 * 
		 * @todo Consider adding some validation for the values passed into the constructor.
		 * 
		 * @since v0.0.1
		 */
		Version(uint32_t major, uint32_t minor, uint32_t patch) noexcept : majorVersion(major), minorVersion(minor), patchVersion(patch) {}

		/**
		 * @brief A two-argument constructor that will fill in the @a major and @a minor version numbers with the given
		 * parameters. The @a patch version number will default to @b 1.
		 * 
		 * @param major The current @b major release number to be used.
		 * @param minor The current @b minor release number to be used.
		 * 
		 * @since v0.0.1
		 */
		Version(uint32_t major, uint32_t minor) noexcept : Version(major, minor, 1) {}

		Version() = default;

		~Version() = default;

		/**
		 * @brief Formats the version number into a string with the following format: @a majorVersion.@a minorVersion.@a patchVersion.
		 * This function is used within the std::formatter<Atlas::Version> class to automatically format the version number into a
		 * string. The formatter can be found in this file outside the namespace scope.
		 * 
		 * @return A formatted @c std::string containing a "pretty" version number.
		 * 
		 * @since v0.0.1
		 */
		std::string toString() const {
			return std::format("{}.{}.{}", majorVersion, minorVersion, patchVersion);
		}

		auto operator<=>(const Version& other) const = default;
	};
}


// This has to be outside the namespace
template<>
struct std::formatter <Atlas::Version> : std::formatter<std::string> {
	auto format(Atlas::Version const& version, format_context& ctx) const {
		return formatter<std::string>::format(version.toString(), ctx);
	}
};