/**************************************************************************************************
 * @file Window.cpp
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <iostream>
#include <memory>
#include <type_traits>

#include "../../core/Core.h"
#include "../../core/Common.h"

#include "Window.h"

#ifdef ATLAS_USE_GLFW3
	#include <GLFW/glfw3.h>

#elif defined(ATLAS_USE_SDL2)
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_stdinc.h>
	#include <SDL2/SDL_video.h>
	#include <SDL2/SDL_events.h>
#endif

#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_vulkan.h>

#ifdef ATLAS_USE_VULKAN
	#include "../vulkan/VKDevice.h"
#endif


//enum class KeyboardKey : int {
//	KEY_NULL = 0,        // Key: NULL, used for no key pressed
//	// Alphanumeric keys
//	KEY_APOSTROPHE = 39,       // Key: '
//	KEY_COMMA = 44,       // Key: ,
//	KEY_MINUS = 45,       // Key: -
//	KEY_PERIOD = 46,       // Key: .
//	KEY_SLASH = 47,       // Key: /
//	KEY_ZERO = 48,       // Key: 0
//	KEY_ONE = 49,       // Key: 1
//	KEY_TWO = 50,       // Key: 2
//	KEY_THREE = 51,       // Key: 3
//	KEY_FOUR = 52,       // Key: 4
//	KEY_FIVE = 53,       // Key: 5
//	KEY_SIX = 54,       // Key: 6
//	KEY_SEVEN = 55,       // Key: 7
//	KEY_EIGHT = 56,       // Key: 8
//	KEY_NINE = 57,       // Key: 9
//	KEY_SEMICOLON = 59,       // Key: ;
//	KEY_EQUAL = 61,       // Key: =
//	KEY_A = 65,       // Key: A | a
//	KEY_B = 66,       // Key: B | b
//	KEY_C = 67,       // Key: C | c
//	KEY_D = 68,       // Key: D | d
//	KEY_E = 69,       // Key: E | e
//	KEY_F = 70,       // Key: F | f
//	KEY_G = 71,       // Key: G | g
//	KEY_H = 72,       // Key: H | h
//	KEY_I = 73,       // Key: I | i
//	KEY_J = 74,       // Key: J | j
//	KEY_K = 75,       // Key: K | k
//	KEY_L = 76,       // Key: L | l
//	KEY_M = 77,       // Key: M | m
//	KEY_N = 78,       // Key: N | n
//	KEY_O = 79,       // Key: O | o
//	KEY_P = 80,       // Key: P | p
//	KEY_Q = 81,       // Key: Q | q
//	KEY_R = 82,       // Key: R | r
//	KEY_S = 83,       // Key: S | s
//	KEY_T = 84,       // Key: T | t
//	KEY_U = 85,       // Key: U | u
//	KEY_V = 86,       // Key: V | v
//	KEY_W = 87,       // Key: W | w
//	KEY_X = 88,       // Key: X | x
//	KEY_Y = 89,       // Key: Y | y
//	KEY_Z = 90,       // Key: Z | z
//	KEY_LEFT_BRACKET = 91,       // Key: [
//	KEY_BACKSLASH = 92,       // Key: '\'
//	KEY_RIGHT_BRACKET = 93,       // Key: ]
//	KEY_GRAVE = 96,       // Key: `
//	// Function keys
//	KEY_SPACE = 32,       // Key: Space
//	KEY_ESCAPE = 256,      // Key: Esc
//	KEY_ENTER = 257,      // Key: Enter
//	KEY_TAB = 258,      // Key: Tab
//	KEY_BACKSPACE = 259,      // Key: Backspace
//	KEY_INSERT = 260,      // Key: Ins
//	KEY_DELETE = 261,      // Key: Del
//	KEY_RIGHT = 262,      // Key: Cursor right
//	KEY_LEFT = 263,      // Key: Cursor left
//	KEY_DOWN = 264,      // Key: Cursor down
//	KEY_UP = 265,      // Key: Cursor up
//	KEY_PAGE_UP = 266,      // Key: Page up
//	KEY_PAGE_DOWN = 267,      // Key: Page down
//	KEY_HOME = 268,      // Key: Home
//	KEY_END = 269,      // Key: End
//	KEY_CAPS_LOCK = 280,      // Key: Caps lock
//	KEY_SCROLL_LOCK = 281,      // Key: Scroll down
//	KEY_NUM_LOCK = 282,      // Key: Num lock
//	KEY_PRINT_SCREEN = 283,      // Key: Print screen
//	KEY_PAUSE = 284,      // Key: Pause
//	KEY_F1 = 290,      // Key: F1
//	KEY_F2 = 291,      // Key: F2
//	KEY_F3 = 292,      // Key: F3
//	KEY_F4 = 293,      // Key: F4
//	KEY_F5 = 294,      // Key: F5
//	KEY_F6 = 295,      // Key: F6
//	KEY_F7 = 296,      // Key: F7
//	KEY_F8 = 297,      // Key: F8
//	KEY_F9 = 298,      // Key: F9
//	KEY_F10 = 299,      // Key: F10
//	KEY_F11 = 300,      // Key: F11
//	KEY_F12 = 301,      // Key: F12
//	KEY_LEFT_SHIFT = 340,      // Key: Shift left
//	KEY_LEFT_CONTROL = 341,      // Key: Control left
//	KEY_LEFT_ALT = 342,      // Key: Alt left
//	KEY_LEFT_SUPER = 343,      // Key: Super left
//	KEY_RIGHT_SHIFT = 344,      // Key: Shift right
//	KEY_RIGHT_CONTROL = 345,      // Key: Control right
//	KEY_RIGHT_ALT = 346,      // Key: Alt right
//	KEY_RIGHT_SUPER = 347,      // Key: Super right
//	KEY_KB_MENU = 348,      // Key: KB menu
//	// Keypad keys
//	KEY_KP_0 = 320,      // Key: Keypad 0
//	KEY_KP_1 = 321,      // Key: Keypad 1
//	KEY_KP_2 = 322,      // Key: Keypad 2
//	KEY_KP_3 = 323,      // Key: Keypad 3
//	KEY_KP_4 = 324,      // Key: Keypad 4
//	KEY_KP_5 = 325,      // Key: Keypad 5
//	KEY_KP_6 = 326,      // Key: Keypad 6
//	KEY_KP_7 = 327,      // Key: Keypad 7
//	KEY_KP_8 = 328,      // Key: Keypad 8
//	KEY_KP_9 = 329,      // Key: Keypad 9
//	KEY_KP_DECIMAL = 330,      // Key: Keypad .
//	KEY_KP_DIVIDE = 331,      // Key: Keypad /
//	KEY_KP_MULTIPLY = 332,      // Key: Keypad *
//	KEY_KP_SUBTRACT = 333,      // Key: Keypad -
//	KEY_KP_ADD = 334,      // Key: Keypad +
//	KEY_KP_ENTER = 335,      // Key: Keypad Enter
//	KEY_KP_EQUAL = 336,      // Key: Keypad =
//	// Android key buttons
//	KEY_BACK = 4,        // Key: Android back button
//	KEY_MENU = 5,        // Key: Android menu button
//	KEY_VOLUME_UP = 24,       // Key: Android volume up button
//	KEY_VOLUME_DOWN = 25        // Key: Android volume down button
//};
//
//
//#define SCANCODE_MAPPED_NUM 232
//using enum KeyboardKey;
//
//static const std::array<KeyboardKey, SCANCODE_MAPPED_NUM> mapScancodeToKey = {
//	KEY_NULL,           // SDL_SCANCODE_UNKNOWN
//	KEY_NULL,
//	KEY_NULL,
//	KEY_NULL,
//	KEY_A,              // SDL_SCANCODE_A
//	KEY_B,              // SDL_SCANCODE_B
//	KEY_C,              // SDL_SCANCODE_C
//	KEY_D,              // SDL_SCANCODE_D
//	KEY_E,              // SDL_SCANCODE_E
//	KEY_F,              // SDL_SCANCODE_F
//	KEY_G,              // SDL_SCANCODE_G
//	KEY_H,              // SDL_SCANCODE_H
//	KEY_I,              // SDL_SCANCODE_I
//	KEY_J,              // SDL_SCANCODE_J
//	KEY_K,              // SDL_SCANCODE_K
//	KEY_L,              // SDL_SCANCODE_L
//	KEY_M,              // SDL_SCANCODE_M
//	KEY_N,              // SDL_SCANCODE_N
//	KEY_O,              // SDL_SCANCODE_O
//	KEY_P,              // SDL_SCANCODE_P
//	KEY_Q,              // SDL_SCANCODE_Q
//	KEY_R,              // SDL_SCANCODE_R
//	KEY_S,              // SDL_SCANCODE_S
//	KEY_T,              // SDL_SCANCODE_T
//	KEY_U,              // SDL_SCANCODE_U
//	KEY_V,              // SDL_SCANCODE_V
//	KEY_W,              // SDL_SCANCODE_W
//	KEY_X,              // SDL_SCANCODE_X
//	KEY_Y,              // SDL_SCANCODE_Y
//	KEY_Z,              // SDL_SCANCODE_Z
//	KEY_ONE,            // SDL_SCANCODE_1
//	KEY_TWO,            // SDL_SCANCODE_2
//	KEY_THREE,          // SDL_SCANCODE_3
//	KEY_FOUR,           // SDL_SCANCODE_4
//	KEY_FIVE,           // SDL_SCANCODE_5
//	KEY_SIX,            // SDL_SCANCODE_6
//	KEY_SEVEN,          // SDL_SCANCODE_7
//	KEY_EIGHT,          // SDL_SCANCODE_8
//	KEY_NINE,           // SDL_SCANCODE_9
//	KEY_ZERO,           // SDL_SCANCODE_0
//	KEY_ENTER,          // SDL_SCANCODE_RETURN
//	KEY_ESCAPE,         // SDL_SCANCODE_ESCAPE
//	KEY_BACKSPACE,      // SDL_SCANCODE_BACKSPACE
//	KEY_TAB,            // SDL_SCANCODE_TAB
//	KEY_SPACE,          // SDL_SCANCODE_SPACE
//	KEY_MINUS,          // SDL_SCANCODE_MINUS
//	KEY_EQUAL,          // SDL_SCANCODE_EQUALS
//	KEY_LEFT_BRACKET,   // SDL_SCANCODE_LEFTBRACKET
//	KEY_RIGHT_BRACKET,  // SDL_SCANCODE_RIGHTBRACKET
//	KEY_BACKSLASH,      // SDL_SCANCODE_BACKSLASH
//	KEY_NULL,                  // SDL_SCANCODE_NONUSHASH
//	KEY_SEMICOLON,      // SDL_SCANCODE_SEMICOLON
//	KEY_APOSTROPHE,     // SDL_SCANCODE_APOSTROPHE
//	KEY_GRAVE,          // SDL_SCANCODE_GRAVE
//	KEY_COMMA,          // SDL_SCANCODE_COMMA
//	KEY_PERIOD,         // SDL_SCANCODE_PERIOD
//	KEY_SLASH,          // SDL_SCANCODE_SLASH
//	KEY_CAPS_LOCK,      // SDL_SCANCODE_CAPSLOCK
//	KEY_F1,             // SDL_SCANCODE_F1
//	KEY_F2,             // SDL_SCANCODE_F2
//	KEY_F3,             // SDL_SCANCODE_F3
//	KEY_F4,             // SDL_SCANCODE_F4
//	KEY_F5,             // SDL_SCANCODE_F5
//	KEY_F6,             // SDL_SCANCODE_F6
//	KEY_F7,             // SDL_SCANCODE_F7
//	KEY_F8,             // SDL_SCANCODE_F8
//	KEY_F9,             // SDL_SCANCODE_F9
//	KEY_F10,            // SDL_SCANCODE_F10
//	KEY_F11,            // SDL_SCANCODE_F11
//	KEY_F12,            // SDL_SCANCODE_F12
//	KEY_PRINT_SCREEN,   // SDL_SCANCODE_PRINTSCREEN
//	KEY_SCROLL_LOCK,    // SDL_SCANCODE_SCROLLLOCK
//	KEY_PAUSE,          // SDL_SCANCODE_PAUSE
//	KEY_INSERT,         // SDL_SCANCODE_INSERT
//	KEY_HOME,           // SDL_SCANCODE_HOME
//	KEY_PAGE_UP,        // SDL_SCANCODE_PAGEUP
//	KEY_DELETE,         // SDL_SCANCODE_DELETE
//	KEY_END,            // SDL_SCANCODE_END
//	KEY_PAGE_DOWN,      // SDL_SCANCODE_PAGEDOWN
//	KEY_RIGHT,          // SDL_SCANCODE_RIGHT
//	KEY_LEFT,           // SDL_SCANCODE_LEFT
//	KEY_DOWN,           // SDL_SCANCODE_DOWN
//	KEY_UP,             // SDL_SCANCODE_UP
//	KEY_NUM_LOCK,       // SDL_SCANCODE_NUMLOCKCLEAR
//	KEY_KP_DIVIDE,      // SDL_SCANCODE_KP_DIVIDE
//	KEY_KP_MULTIPLY,    // SDL_SCANCODE_KP_MULTIPLY
//	KEY_KP_SUBTRACT,    // SDL_SCANCODE_KP_MINUS
//	KEY_KP_ADD,         // SDL_SCANCODE_KP_PLUS
//	KEY_KP_ENTER,       // SDL_SCANCODE_KP_ENTER
//	KEY_KP_1,           // SDL_SCANCODE_KP_1
//	KEY_KP_2,           // SDL_SCANCODE_KP_2
//	KEY_KP_3,           // SDL_SCANCODE_KP_3
//	KEY_KP_4,           // SDL_SCANCODE_KP_4
//	KEY_KP_5,           // SDL_SCANCODE_KP_5
//	KEY_KP_6,           // SDL_SCANCODE_KP_6
//	KEY_KP_7,           // SDL_SCANCODE_KP_7
//	KEY_KP_8,           // SDL_SCANCODE_KP_8
//	KEY_KP_9,           // SDL_SCANCODE_KP_9
//	KEY_KP_0,           // SDL_SCANCODE_KP_0
//	KEY_KP_DECIMAL,     // SDL_SCANCODE_KP_PERIOD
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
//	KEY_LEFT_CONTROL,   //SDL_SCANCODE_LCTRL
//	KEY_LEFT_SHIFT,     //SDL_SCANCODE_LSHIFT
//	KEY_LEFT_ALT,       //SDL_SCANCODE_LALT
//	KEY_LEFT_SUPER,     //SDL_SCANCODE_LGUI
//	KEY_RIGHT_CONTROL,  //SDL_SCANCODE_RCTRL
//	KEY_RIGHT_SHIFT,    //SDL_SCANCODE_RSHIFT
//	KEY_RIGHT_ALT,      //SDL_SCANCODE_RALT
//	KEY_RIGHT_SUPER     //SDL_SCANCODE_RGUI
//};
//
//static KeyboardKey ConvertScancodeToKey(SDL_Scancode sdlScancode)
//{
//	if((sdlScancode >= 0) && (sdlScancode < SCANCODE_MAPPED_NUM))
//	{
//		return mapScancodeToKey[sdlScancode];
//	}
//
//	return KEY_NULL; // No equivalent key in Raylib
//}

inline uint32_t Atlas::GameWindow::ComputeWindowFlags(std::vector<EWindowFlag> const& windowFlags) {
	uint32_t flags = 0;

	for (auto const& flag : windowFlags) {
		flags |= static_cast<uint32_t>(flag);
	}

	return flags;
}

inline SDL_Window* Atlas::GameWindow::CreateSDLWindow(WindowDescription const& windowDescription, uint32_t windowFlags) {
	return SDL_CreateWindow(
		windowDescription.windowTitle.c_str(),
		windowDescription.windowRect.position.x,
		windowDescription.windowRect.position.y,
		windowDescription.windowRect.size.x,
		windowDescription.windowRect.size.y,
		windowFlags
	);
}

inline void Atlas::GameWindow::DestroySDLWindow(SDL_Window* window) {
	// No need to destroy a null window
	if (window == nullptr) {
		return;
	}

	SDL_DestroyWindow(window);
}

Atlas::GameWindow::GameWindow(WindowDescription const& windowDescription) : Initializable(false), mWindowDescription(windowDescription) {
	const int cInitResult = SDL_Init(SDL_INIT_EVERYTHING);

	if (cInitResult < 0) {
		const std::string cSDLError = std::string(SDL_GetError());
		const std::string cErrorString = std::format("Failed to initialize SDL video subsystem\n\tError: {}\n\tError Code: {}", cSDLError, cInitResult);

		throw WindowInitFailureException(cErrorString);
	}

	this->setInit();
}

Atlas::GameWindow::~GameWindow() {
	this->setNotInit();

	if (mSDLWindowPointer != nullptr) {
		DestroySDLWindow(mSDLWindowPointer);

		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
}

void Atlas::GameWindow::open(uint32_t windowFlags) {
	mSDLWindowPointer = CreateSDLWindow(getWindowDescription(), windowFlags);

	if (mSDLWindowPointer == nullptr) {
		throw std::runtime_error("Failed to create SDL window");
	}
}

void Atlas::GameWindow::open(std::vector<EWindowFlag> const& windowFlags) {
	if (!windowFlags.empty()) {
		uint32_t flags = ComputeWindowFlags(windowFlags);

		open(flags);
	}
}

void Atlas::GameWindow::close() {
	if (mSDLWindowPointer != nullptr) {
		DestroySDLWindow(mSDLWindowPointer);
	}

	mbQuitRequested = true;
}

void Atlas::GameWindow::hide() {
	if (mSDLWindowPointer == nullptr) {
		throw std::runtime_error("SDL window pointer is null");
	}

	SDL_HideWindow(mSDLWindowPointer);
	mbIsVisible = false;
}

void Atlas::GameWindow::show() {
	if (mSDLWindowPointer == nullptr) {
		throw std::runtime_error("SDL window pointer is null");
	}

	SDL_ShowWindow(mSDLWindowPointer);

	mbIsVisible = true;
}

void Atlas::GameWindow::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			close();
		}

		ImGui_ImplSDL2_ProcessEvent(&event);
	}
}

void Atlas::GameWindow::flash(const EWindowFlashCondition flashCondition) {
	if (mSDLWindowPointer != nullptr) {
		SDL_FlashOperation translatedFlashCondition;

		switch (flashCondition)
		{
			using enum EWindowFlashCondition;

		case Cancel:
			translatedFlashCondition = SDL_FLASH_CANCEL;
			break;

		case Briefly:
			translatedFlashCondition = SDL_FLASH_BRIEFLY;
			break;

		case Until_Focused:
			translatedFlashCondition = SDL_FLASH_UNTIL_FOCUSED;
			break;

		default:
			translatedFlashCondition = SDL_FLASH_CANCEL;
			break;
		}

		SDL_FlashWindow(mSDLWindowPointer, translatedFlashCondition);
	}
}

void Atlas::GameWindow::setOpacity(float newOpacity)
{
	if (mSDLWindowPointer != nullptr) {
		SDL_SetWindowOpacity(mSDLWindowPointer, newOpacity);
	}
}

float Atlas::GameWindow::getOpacity() const
{
	float opacity = 0.0f;

	if (mSDLWindowPointer != nullptr) {
		SDL_GetWindowOpacity(mSDLWindowPointer, &opacity);
	}

	return opacity;
}

bool Atlas::GameWindow::shouldClose() const {
	//return SDL_PollEvent(nullptr) == 0;
	return mSDLWindowPointer == nullptr && !mbQuitRequested;
}

bool Atlas::GameWindow::isOpen() const { 
	return mSDLWindowPointer != nullptr;
}

bool Atlas::GameWindow::isVisible() const { 
	return mbIsVisible;
}

Atlas::WindowDescription Atlas::GameWindow::getWindowDescription() const { 
	return mWindowDescription;
}

SDL_Window* Atlas::GameWindow::getWindowHandle() { 
	return mSDLWindowPointer;
}
