#pragma once
#include <cstdint>

#include "../core/Core.h"

namespace Atlas {
	//using HardwareConcurrency = uint32_t;

	// While MacOS is not yet supported, it will be in the future (probably)
	enum class EOperatingSystem {
		Unknown = 0,
		Windows,
		Linux,
		MacOS
	};

	class APlatformInfo {
	private:
		
		EOperatingSystem mOperatingSystem = EOperatingSystem::Unknown;

	protected:
		void setOperatingSystem(EOperatingSystem operatingSystem) { 
			mOperatingSystem = operatingSystem;
		}

	public:

		APlatformInfo() = default;
		virtual ~APlatformInfo() = default;

		EOperatingSystem getOperatingSystem() const noexcept {
			return mOperatingSystem;
		}


	};
}
