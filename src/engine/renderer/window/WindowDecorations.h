/*****************************************************************//**
 * @file   WindowDecorations.h
 * 
 * @brief  Everything that is related to the WindowDecorations 
 * class and it's relevant code.
 * 
 * @date   September 2024
 * 
 * @since v0.0.1
 *********************************************************************/
#pragma once

#include <cstdint>
#include <string>

/**
 * @brief A macro that represents the default value that should
 * be used to initialize the window title variable in the 
 * WindowDecorations class.
 * 
 * @note This was added in v0.0.9 because it was originally
 * hardcoded within the @ref WindowDecorations class.
 * 
 * @since v0.0.9
 */
#define ATLAS_DEFAULT_WINDOW_TITLE "Untitled Atlas Project"

/**
 * @brief A macro that represents the default value that should
 * be used to initialize the window width variable in the 
 * WindowDecorations class.
 * 
 * @note This was added in v0.0.9 because it was originally
 * hardcoded within the @ref WindowDecorations class.
 * 
 * @since 
 */
#define ATLAS_DEFAULT_WINDOW_WIDTH static_cast<uint32_t>(1280)

/**
 * @brief A macro that represents the default value that should
 * be used to initialize the window height variable in the 
 * WindowDecorations class.
 *
 * @note This was added in v0.0.9 because it was originally
 * hardcoded within the @ref WindowDecorations class.
 *
 * @since 
 */
#define ATLAS_DEFAULT_WINDOW_HEIGHT static_cast<uint32_t>(720)

/**
 * @brief A macro that represents the default value that should
 * be used to initialize the window flags variable in the 
 * WindowDecorations class.
 *
 * @note This was added in v0.0.9 because it was originally
 * hardcoded within the @ref WindowDecorations class.
 *
 * @since 
 */
#define ATLAS_DEFAULT_WINDOW_CONFIG_FLAGS static_cast<unsigned int>(-1)

/**
 * @brief A macro that represents the default value that should
 * be used to initialize the window target FPS variable in the 
 * WindowDecorations class.
 *
 * @note This was added in v0.0.9 because it was originally
 * hardcoded within the @ref WindowDecorations class.
 *
 * @since 
 */
#define ATLAS_DEFAULT_WINDOW_TARGET_FPS static_cast<unsigned int>(60)

/**
 * @brief A macro that represents the default value that should
 * be used to initialize the window icon path variable in the 
 * WindowDecorations class.
 *
 * @note This was added in v0.0.9 because it was originally
 * hardcoded within the @ref WindowDecorations class.
 *
 * @since 
 */
#define ATLAS_DEFAULT_WINDOW_ICON_PATH "\\engine\\assets\\engine\\techstorm.png"

namespace Atlas {
	/**
	 * @brief A class that is responsible for decorating the window.
	 * This responsibility includes:
	 * - Setting the window title
	 * - Setting the window size 
	 * - Setting the window icon
	 * - Setting the window flags (aka window hints)
	 * - Setting the target FPS
	 * This class has some important usages, of which can be seen
	 * within the remarks section below.
	 * 
	 * @remarks This class is important because it holds all the 
	 * window configuration information for the window and is
	 * exclusively used by the children of the @ref IWindow class
	 * to decorate the window.
	 * 
	 * @warning The values of this should be set before the window
	 * is created.
	 * 
	 * @note Within the codebase, the term "window decoration" is
	 * used to refer to the setting of the window title, size,
	 * position, icon, flags, and target FPS. Essentially, it
	 * "decorates" the window with all the information necessary
	 * to create the window.
	 * 
	 * @note Originally, the strings in this class were made as a
	 * const char* type (this refers to @ref title, @ref iconPath)
	 * so that it would seamlessly integrate with raylib (their
	 * functions are in C, therefore it must be a const char* type),
	 * but that is not the case anymore. This is due to the fact
	 * that v0.0.9 is being rewritten to "generalize" the rendering
	 * APIs in a way that allows developers to implement other
	 * window management APIs, such as SDL. Flexibility is a key
	 * feature of Atlas, and therefore this change is needed.
	 * 
	 * @todo Consider adding support for the following:
	 * 1. Window resizing
	 * 2. Manual window position
	 * 3. Window resolution
	 * 4. The ability to do serialization and deserialization of
	 * its configuration. 
	 * 5. The ability to request window attention (flash, etc.).
	 * 
	 * @todo The window decorations class should be able to
	 * manually set the window size, position, and resolution instead
	 * of it being in the @ref Window class.
	 * 
	 * @todo Encapsulate the variables within the class so that they
	 * are private.
	 * 	 
	 * @since v0.0.1
	 */
	class WindowDecorations {
	public:

		/**
		 * @brief A variable that holds the window title that can be
		 * seen. By default, it is set to the value of the
		 * ATLAS_DEFAULT_WINDOW_TITLE macro.
		 * 
		 * 
		 * @todo Consider replacing this with a std::string type
		 * instead of a const char*. See the notes section of this
		 * class for an explanation on why this needs to be done.
		 * 
		 * @todo Convert this variable's type into a std::string
		 * type.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref ATLAS_DEFAULT_WINDOW_TITLE for what this is set
		 * to by default.
		 */
		const char* mWindowTitle = ATLAS_DEFAULT_WINDOW_TITLE;

		/**
		 * @brief How wide the window is. By default, it is set to
		 * the value of the ATLAS_DEFAULT_WINDOW_WIDTH macro. 
		 * 
		 * @remarks When figuring out what to set this value to,
		 * consider the following questions:
		 * - Is the window going to be fullscreen?
		 * - Are you planning on giving players the ability to resize
		 * the window? How about if the window is not fullscreen? 
		 * - How are you going to determine the size of the window?
		 * - How will the width of the window impact the gameplay?
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref ATLAS_DEFAULT_WINDOW_WIDTH for what this is set to
		 * by default.
		 */
		uint32_t mWindowWidth = ATLAS_DEFAULT_WINDOW_WIDTH; 

		/**
		 * @brief How tall the window is. By default, it is set to
		 * the value of the ATLAS_DEFAULT_WINDOW_HEIGHT macro.
		 * 
		 * @remarks See the remarks for the width variable above for
		 * some helpful questions to ask yourself when setting this
		 * variable. Instead of wondering about the width, simply
		 * focus on the height with the same questions.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref ATLAS_DEFAULT_WINDOW_HEIGHT for what this is set to
		 * by default.
		 */
		uint32_t mWindowHeight = ATLAS_DEFAULT_WINDOW_HEIGHT; 

		/**
		 * @brief The configuration flags for the window. This is
		 * also known as window hints in GLFW. It is recommended
		 * that you take a quick look at the documentation for
		 * this variable because it contains a lot of information
		 * about what to keep an eye out for or relevant
		 * information. You may find it by going to the @c docs
		 * folder within the root directory of the engine.
		 * 
		 * @remarks The reason for why this is an unsigned int is
		 * because it is a bitfield and it should also be able to
		 * be used with an enum class that inherits from an
		 * unsigned integer. Furthermore, the specific values of
		 * which ever enum is used to configure the window are
		 * unknown, therefore how many bits are needed to store
		 * them is also unknown.
		 * 
		 * @important If you are using @b Raylib as the graphics 
		 * API, make sure that this matches the ConfigFlags enum
		 * within raylib.h. 
		 * 
		 * @note If you want to use multiple config flags, you can
		 * use the | operator to combine them.
		 * 
		 * @todo Consider replacing the use of an unsigned int with
		 * a more suitable uint type that is from the stdint header.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref ATLAS_DEFAULT_WINDOW_CONFIG_FLAGS for what this
		 * is set to by default.
		 */
		unsigned int mWindowConfigFlags = ATLAS_DEFAULT_WINDOW_CONFIG_FLAGS; 

		/**
		 * @brief As is the name suggests, this is the amount of frames
		 * per second that Atlas will try to run at. This does @b not 
		 * necessarily mean that the window will run at this FPS, but
		 * rather it is a suggestion to the engine. If you are using
		 * the default renderer, this is handled by raylib's SetTargetFPS
		 * function. By default, it is set to the value of the value of
		 * the ATLAS_DEFAULT_WINDOW_TARGET_FPS macro.
		 * 
		 * @remarks It is recommended that this is used because it will
		 * help manage the amount of system resources used by Atlas. 
		 * If this was not set, Atlas would just use as much system
		 * resources as possible, thereby bogging down the user's
		 * computer. Nobody likes system resources being consumed
		 * by a gluttonous application.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref ATLAS_DEFAULT_WINDOW_TARGET_FPS for what this is
		 * set to by default.
		 */
		unsigned int mTargetFPS = ATLAS_DEFAULT_WINDOW_TARGET_FPS; 

		/**
		 * @brief This is the path to the icon that will be used for
		 * the window. It will set the icon on the window itself and
		 * on the taskbar. By default, it is set to the value of the
		 * ATLAS_DEFAULT_WINDOW_ICON_PATH macro. During the
		 * initialization phase of the window, the icon will be
		 * loaded and set based on this path. That means that this
		 * @b must be compliant with Atlas' file system sandboxing
		 * and other restrictions.
		 * 
		 * @pre The @ref FileSystemRegistry must be initialized
		 * and both the discovery and registration phases of the
		 * file system must have already been completed.
		 * 
		 * @warning The path must lead to a valid image file.
		 * 
		 * 
		 * @todo Consider replacing this with a std::string type
		 * instead of a const char*. See the notes section of this
		 * class for an explanation on why this needs to be done.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref ATLAS_DEFAULT_WINDOW_ICON_PATH for what this is
		 * set to by default.
		 * 
		 * @sa @ref FileSystemRegistry for how to use the file system
		 * in Atlas.
		 */
		const char* mIconPath = ATLAS_DEFAULT_WINDOW_ICON_PATH;

		WindowDecorations(const WindowDecorations&) = delete;

		/**
		 * @brief A constructor that initializes the window decorations
		 * to their default values, as specified by the macros.
		 * 
		 * @remarks This constructor is helpful for when you are 
		 * prototyping and you do not need to set any values.
		 * 
		 * @since v0.0.1
		 */
		WindowDecorations() = default;

		

		~WindowDecorations();
	};
}
