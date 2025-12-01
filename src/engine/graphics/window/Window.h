/**************************************************************************************************
 * @file Window.h
 * 
 * @brief Everything required to create a window. If you wish to create a window, you may use
 * the GLFWGameWindow or the SDLGameWindow classes direction (if you dont wish to make your
 * own window class). If you want to use a custom window class, you should inherit from
 * AGameWindow or NullGameWindow, depending on your needs.
 * 
 * @date May 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <string>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <functional>
#include <type_traits>
#include <any>
#include <variant>
#include <array>

#include "../../core/Core.h" // this include has to be put here because the GLFW_INCLUDE_VULKAN is defined in this file (of course if vulkan and GLFW3 is being used!)
#include "../../core/Common.h"

#ifdef ATLAS_USE_GLFW3
	#include <GLFW/glfw3.h>

#elif defined ATLAS_USE_SDL2
	#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#endif // ATLAS_USE_GLFW3


#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/detail/qualifier.hpp>
#include <SDL2/SDL_events.h>
#include <vulkan/vulkan_core.h>

#include <eventpp/eventqueue.h>

namespace Atlas {

	/**
	 * @brief A using alias for a GLM low-precision 2D vector that can be used to store the size
	 * of a window.
	 * 
	 * @note There is no need for any real amount of precision as this is just used for storing
	 * the size of the window, so having a higher precision is not necessary and could be a
	 * performance hit.
	 *
	 * @since v0.0.1
	 */
	using WindowSize = glm::vec<2, uint32_t, glm::lowp>;
	
	using WindowRectangle = Rectangle<int>;

	struct WindowHint {
		std::string identifier;
		std::string value;
	};

	/**
	 * @brief A struct that contains all the information that a window needs. 
	 * 
	 * @since v
	 */
	struct WindowDescription {
		std::string title;
		std::string iconPath;
		WindowRectangle windowRectangle;

		uint16_t targetFPS;
	};

	template<typename T_SURFACE_HANDLE>
	class SurfaceBase : public InitializableAndValidatable {
	private:
		T_SURFACE_HANDLE mSurfaceHandle;

	protected:
		//void setSurfaceHandle(T_SURFACE_HANDLE surfaceHandle) { this->mSurfaceHandle = surfaceHandle; }

	public:
		
		virtual void destroySurface() {
			setNotInit();
			setInvalid();
		}

		T_SURFACE_HANDLE getHandle() const { return this->mSurfaceHandle; }

		//T_SURFACE_HANDLE getSurfaceHandle() const { return this->mSurfaceHandle; }

		//T_SURFACE_HANDLE& getSurfaceHandleRef() { return this->mSurfaceHandle; }
				
	};

#ifdef ATLAS_USE_VULKAN
	class Surface : public SurfaceBase<VkSurfaceKHR> {
	public:
	};
#endif

	class Viewport : public InitializableAndValidatable {
	private:
		VkViewport mViewport;

		VkCommandBuffer mCommandBuffer;
	
	public:

		
	};

	class ViewportManager : public InitializableAndValidatable {
	public:
			
	};

#ifdef ATLAS_USE_SDL2

	//class IGameWindow;

	//using WindowEventTriggerCallback = std::function<Result<void>(SDL_Event, std::shared_ptr<IGameWindow>)>;

	//struct WindowEventSubscriptionInfo {
	//	SDL_Event eventType;
	//	WindowEventTriggerCallback callback;
	//};

	//class SDLWindowEventHandler {
	//private:
	//	eventpp::EventQueue<SDL_Event, void(SDL_Event, std::shared_ptr<IGameWindow>)> mEventQueue;
	//public:

	//	Result<SDL_Event> pollEvent() {
	//		SDL_Event event;

	//		if (SDL_PollEvent(&event) != 0) {
	//			return event;
	//		}
	//		else {
	//			return tl::make_unexpected<Error>({
	//				Error::EErrorCategory::OutOfRange,
	//				0,
	//				"SDL_PollEvent returned 0. This is probably because there are no more events to be polled."
	//			});
	//		}
	//	}
	//	
	//	Result<void> enqueue(SDL_Event event, std::shared_ptr<IGameWindow> window) {
	//		return mEventQueue.enqueue(event, event, window);
	//	}

	//	Result<void> enqueue(std::vector<SDL_Event> events, std::shared_ptr<IGameWindow> window) {
	//		return mEventQueue.enqueue(events, window);
	//	}

	//	Result<void> processEvents() {
	//		
	//	}
	//};
#endif

	//class IGameWindow : public InitializableAndValidatable {
	//protected:

	//	virtual void setHandle(std::any handle) = 0;
	//	virtual std::any getHandle() const = 0;
	//public:

	//	
	//};

	template<typename T_WINDOW_HANDLE>
	class AGameWindow2 : public InitializableAndValidatable {
	public:


	private:
		
		T_WINDOW_HANDLE mWindowHandle;
		
		bool mbIsOpen{ false };
		bool mbShouldClose;
	protected:

		void setWindowHandle(T_WINDOW_HANDLE windowHandle) { this->mWindowHandle = windowHandle; }

	public:

		T_WINDOW_HANDLE getHandle() const { return this->mWindowHandle; }
	};

	/**
	 * @brief An interface that allows the creation of custom window classes in
	 * an API-agnostic way. It contains some general pure virtual functions that
	 * are required to create a window. **This interface is what Atlas stores
	 * a pointer as when creating a window.**
	 * 
	 * @note If you want to make your own window class and you don't want to use
	 * the @ref NullGameWindow class, you should inherit from this interface instead.
	 * 
	 * @since v0.0.1
	 */
	class AGameWindow {
	public:
		struct Options {
			std::vector<uint32_t> windowConfigFlags;
			WindowDescription windowDescription;
		};
	protected:
		static inline bool sHasSetGraphicsAPIFlag = false;
		static inline uint32_t sGraphicsAPIFlag = -1;

		std::string mWindowTitle;
		std::string mIconPath;

		WindowRectangle mWindowRect;

		int16_t mTargetFPS;

		bool mIsInitialized = false;

		bool mIsOpen = false;

		bool mShouldClose = false;

	public:

		explicit AGameWindow(const uint32_t cGraphicsAPIFlag)
		{
			if (sHasSetGraphicsAPIFlag == false) {
				sGraphicsAPIFlag = cGraphicsAPIFlag;
				sHasSetGraphicsAPIFlag = true;
			}
		}

		AGameWindow() = default;

		virtual ~AGameWindow() = default;

		virtual void init(const uint32_t cInitFlags) = 0;

		virtual void update() = 0;

		virtual void open(const uint32_t cOpenFlags) = 0;

		virtual bool isOpen() const = 0;

		virtual void close() = 0;

		virtual bool shouldClose() = 0;

		virtual void cleanup() = 0;

		virtual bool isInitialized() const = 0;

		virtual void setHint(WindowHint const& hint) = 0;

		virtual void setIcon(std::string const& newIconPath) = 0;

		virtual void setWindowTitle(std::string const& newWindowTitle) = 0;

		virtual void setWindowPosition(int newX, int newY) = 0;

		virtual void setWindowSize(int16_t newWidth, int16_t newHeight) = 0;

		virtual void setTargetFPS(int16_t newTargetFPS) = 0;

		virtual void* getUncastWindowHandle() = 0;

		virtual WindowDescription getWindowDescription() const {

			WindowDescription description{};

			description.title = this->mWindowTitle;
			description.iconPath = this->mIconPath;
			description.windowRectangle = this->mWindowRect;
			description.targetFPS = this->mTargetFPS;

			return description;
		}
	};

	/**
	 * @brief A null, non-copyable implementation of a game window. It does not do anything as it only provides stubs.
	 * Furthermore, this class is also used as the base class for the GLFWGameWindow and SDLGameWindow classes.
	 * 
	 * @since v0.0.1
	 */
	//class NullGameWindow : public AGameWindow {
	//protected:

	//	static inline bool sHasSetGraphicsAPIFlag = false;

	//	std::string mWindowTitle;
	//	std::string mIconPath;

	//	WindowRectangle mWindowRect;

	//	int16_t mTargetFPS;

	//	bool mIsInitialized = false;

	//	bool mIsOpen = false;
	//public:

	//	static inline uint32_t sGraphicsAPIFlag = -1;

	//	using AGameWindow::AGameWindow;

	//	NullGameWindow(NullGameWindow const&) = delete;

	//	NullGameWindow& operator=(NullGameWindow const&) = delete;

	//	explicit NullGameWindow(const uint32_t cGraphicsAPIFlag)
	//		: AGameWindow()
	//	{
	//		if (sHasSetGraphicsAPIFlag == false) {
	//			sGraphicsAPIFlag = cGraphicsAPIFlag;
	//			sHasSetGraphicsAPIFlag = true;
	//		}
	//	}

	//	~NullGameWindow() override = default;

	//	WindowDescription getWindowDescription() const override;


	//	//explicit operator WindowDescription() const override;
	//};


#ifdef ATLAS_USE_GLFW3

	struct GameWindowSettings {
		bool enableEventPolling = true;
		bool fullscreen = false;

		GLFWmonitor* monitor = nullptr; 
	};

	class GLFWGameWindow final : public AGameWindow {
	private:

		friend unsigned int GetWindowConfigFlag(std::string const& flagName);

		/**
		 * @brief A static and read-only translation map. It takes an input string of the flag name and returns the GLFW flag value
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref setFlag() for one of the functions that this map is used in.
		 */
		static inline const std::unordered_map<std::string, uint16_t> sWindowFlagsTranslationMap = {
			{ "Resizable", GLFW_RESIZABLE },
			{ "Visible", GLFW_VISIBLE },
			{ "Decorated", GLFW_DECORATED },
			{ "Focused", GLFW_FOCUSED },
			{ "Auto Iconify", GLFW_AUTO_ICONIFY },
			{ "Floating", GLFW_FLOATING },
			{ "Maximized", GLFW_MAXIMIZED },
			{ "Centered", GLFW_CENTER_CURSOR },
			{ "Transparent", GLFW_TRANSPARENT_FRAMEBUFFER },
			{ "Focus", GLFW_FOCUS_ON_SHOW },
			{ "Scale to Monitor", GLFW_SCALE_TO_MONITOR }
		};

		GLFWwindow* mGLFWWindowPointer = nullptr;

		GameWindowSettings mGameWindowSettings;


	public:

		GLFWGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon, GameWindowSettings const& gameWindowSettings);

		virtual ~GLFWGameWindow();

		// Inherited via IWindow
		void init() override;

		void open() override;

		void update() override;

		bool shouldClose() override;

		void close(bool shouldCleanup) override;

		void cleanup() override;	

		void setFlag(std::string const& flagName, unsigned int value) override;
	};

#elif defined(ATLAS_USE_SDL2)

	using SDLEvent = SDL_Event;

	struct GameWindowSettings {
		bool enableEventPolling = true;
		bool fullscreen = false;

		uint32_t windowInitFlags;
	};

	class SDLGameWindow final : public AGameWindow, private std::enable_shared_from_this<SDLGameWindow> {
	public:
		class EventPoller {
		private:
			eventpp::EventQueue<int, void(SDL_Event, std::weak_ptr<SDLGameWindow>)> mEventQueue;

			size_t mTotalEventCount;
		public:
			
			static inline bool Poll(SDL_Event* eventPtr) {
				return SDL_PollEvent(eventPtr) != 0;
			}

			void appendEvent(int event, std::function<void(SDL_Event, std::weak_ptr<SDLGameWindow>)> const& callback) {
				mEventQueue.appendListener(event, callback);
				
				mTotalEventCount++;
			}

			Result<void> enqueueEvent(int event, std::weak_ptr<SDLGameWindow> weakWindowPtr) {
				Result<void> result = {};
				
				if (weakWindowPtr.expired()) {
					const std::string errorMessage = std::format("Failed to enqueue an SDL event of type {} because the passed window pointer is expired!", event);

					result = tl::make_unexpected<Error>({
						Error::EErrorCategory::InvalidArgument,
						0,
						errorMessage
					});
				}

				mEventQueue.enqueue(event, weakWindowPtr);

				
				return result;
			}

			Result<void> processEvents() {
				Result<void> result = {};

				if (!mEventQueue.process()) {
					result = tl::make_unexpected<Error>({
						Error::EErrorCategory::RuntimeError,
						0,
						"Failed to process SDL events!"
					});
				}

				return result;
			}

			Result<void> pollAndProcessAll(std::weak_ptr<SDLGameWindow> weakWindowPtr, std::optional<std::function<void(SDL_Event, std::weak_ptr<SDLGameWindow>)>> const& afterPollCallback = {}) {
				Result<void> result;
				bool bErrorOccurred = false;

				if (weakWindowPtr.expired()) {
					result = tl::make_unexpected<Error>({
						Error::EErrorCategory::InvalidArgument,
						0,
						"Failed to poll and process SDL events because the passed window pointer is expired!"
					});
				}

				SDL_Event event;

				while (Poll(&event) == true) {
					if (afterPollCallback.has_value()) {
						afterPollCallback.value().operator()(event, weakWindowPtr);
					}

					Result<void> enqueueResult = enqueueEvent(event.type, weakWindowPtr);
					if (!enqueueResult) {
						result = enqueueResult;
						bErrorOccurred = true;
					}

					// This should not be done if the enqueue step failed, hence the bErrorOccurred flag check
					if (!bErrorOccurred) {
						if (Result<void> processResult = processEvents(); !processResult) {
							result = processResult;
							bErrorOccurred = true;
						}
					}
					
					// Since each occurance of an error automatically will set the bErrorOccurred flag to true and set the result to the error,
					// we can break out of the loop here and it will automatically return the error.
					if (bErrorOccurred) {
						break;
					}
				}

				return result;
			}
			
			void clear() {
				mEventQueue.clearEvents();
			}

			size_t GetTotalEventCount() const { return mTotalEventCount; }
		};
	private:
		std::unordered_map<uint32_t, std::function<int(SDL_Event const&, SDLGameWindow&)>> mEventHandlers;

		EventPoller mEventPoller;

		std::shared_ptr<SDLGameWindow> mSharedThisPtr = nullptr;

		SDL_Window* mSDLWindowPointer = nullptr;

		uint64_t mFrameCount = 0;

		uint32_t mSDLWindowFlags = 0;

		bool mShouldClose = false;

		/**
		 * @brief Checks if an event occurred by simply calling SDL_PollEvents with a nullptr.
		 * 
		 * @return @a True if SDL_PollEvents does not return 0, otherwise @a false.
		 * 
		 * @since v
		 */
		static inline bool EventOccurred();

		static inline uint32_t GetGraphicsAPIFlag();

		static inline void OnWindowEvent(SDL_Event event, std::weak_ptr<SDLGameWindow> window);
		static inline void OnWindowCloseEvent(SDL_Event event, std::weak_ptr<SDLGameWindow> window);
		static inline void AfterEachPollCallback(SDL_Event event, std::weak_ptr<SDLGameWindow> window);
	public:

		// SDLGameWindow(std::string const& title, uint32_t width, uint32_t height, int x, int y, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon);

		explicit SDLGameWindow(const uint32_t cGraphicsAPIFlag);

		SDLGameWindow() = default;

		~SDLGameWindow() override = default;

		// Inherited via NullGameWindow
		void init(const uint32_t cInitFlags) override;

		void update() override;

		void open(const uint32_t cOpenFlags) override;

		bool shouldClose() override;

		void close() override;

		void cleanup() override;

		bool isOpen() const override;

		bool isInitialized() const override;

		void setHint(WindowHint const& hint) override;

		void setIcon(std::string const& newIconPath) override;

		void setWindowTitle(std::string const& newWindowTitle) override;

		void setWindowPosition(int newX, int newY) override;

		void setWindowSize(int16_t newWidth, int16_t newHeight) override;

		void setTargetFPS(int16_t newTargetFPS) override;

		void* getUncastWindowHandle() override {
			return this->mSDLWindowPointer;
		}

	};

#endif // ATLAS_USE_GLFW3

	unsigned int GetWindowConfigFlag(std::string const& flagName);
}
