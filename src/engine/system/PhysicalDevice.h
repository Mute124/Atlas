#pragma once
#include <string>
#include <__msvc_string_view.hpp>

namespace Atlas {
	class PhysicalDevice {
	private:
		std::string mDeviceName;
	public:

		void setName(std::string const& cNameRef) {
			mDeviceName = cNameRef;
		}

		std::string_view getName() const {
			return mDeviceName;
		}

	};


}
