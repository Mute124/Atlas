#pragma once
#include <array>

#include "InputKey.h"

namespace Atlas {


	class IInputDevice {
	public:
		virtual ~IInputDevice() = default;

		virtual void pollInput() = 0;
	};

	template<typename T_SCAN_CODE, size_t MAX_KEYS>
	class AButtonInputDevice : public IInputDevice {
	private:
		std::array<InputKeyInfo<T_SCAN_CODE>, MAX_KEYS> mKeys;
	
	public:
				
		virtual bool isKeyDown(T_SCAN_CODE key) const {
			const InputKeyInfo<T_SCAN_CODE> cKeyInfo = mKeys[key];
			
			return cKeyInfo.state == EInputKeyState::Down;	
		}
	};

	
	class AInputDevice {
	private:

	public:

	};
}
