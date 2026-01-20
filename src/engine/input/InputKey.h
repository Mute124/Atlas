#pragma once
#include <string>

namespace Atlas {
	enum class EKeycode {

	};

	enum class EInputSource {
		Keyboard,
		Mouse,
		Controller
	};
	
	enum class EInputKeyState {
		None,
		Pressed,
		Released,
		Down,
		Up
	};

	template<typename T_KeyCode>
	struct InputKeyInfo {
		T_KeyCode keycode;
		EInputKeyState state;

		bool isPressed() const { return state == EInputKeyState::Pressed; }
		bool isReleased() const { return state == EInputKeyState::Released; }
		bool isDown() const { return state == EInputKeyState::Down; }
		bool isUp() const { return state == EInputKeyState::Up; }
	};

	struct InputKey {
		EKeycode keycode;
		EInputSource inputSource;
		std::string keyString;
	};
}
