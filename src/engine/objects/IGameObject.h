#pragma once
#include "../math/Transform.h"
#include <unordered_map>
#include "../Component.h"

#include <exception>

#include <memory>
#include <map>
#include <ranges>

namespace Atlas {
	
	//class IGameObject;

	
	//template<typename T>
	//concept GameObjectType = std::is_base_of_v<IGameObject, T>;

	/// <summary>
	/// Represents an object that can be rendered in the screen. This is an abstract class that contains all the common properties of a game object.
	/// </summary>
	/// <remarks>
	/// <para>
	/// You may implement this yourself to create your own game objects and drawing code, or you can inherit from <see cref="Atlas::GameObject" /> to use the default
	/// implementation. However, if you inherit from this class, be sure you have a basic understanding of Raylib's and OpenGL's rendering system. If not,
	/// see <see cref="https://learnopengl.com/"/> and <see cref="https://www.raylib.com/"/> for a good place to start. In addition, it is not advised for you to inherit this
	/// yourself because it will require you to implement rendering code. However, it is recommended if your game requires a rendering system that is not provided by default.
	/// </para>
	/// <para>
	/// The IGameObject class is abstract and cannot be instantiated directly. Instead, you should inherit from this class and implement the pure virtual functions. As of version 0.0.3, the pure 
	/// virtual functions are: <see cref="render()"/>, <see cref="texture()"/>, <see cref="update()"/>, <see cref="prePhysicsUpdate()"/>, <see cref="postUpdate()"/>, <see cref="destroy()"/>,
	/// and <see cref="cleanup()"/>. 
	/// </para>
	/// </remarks>
	/// <inheritdoc />
	class IGameObject abstract {
	protected:
		std::map<std::string, std::unique_ptr<Component>> mComponents;

		
		/// <summary>
		/// Gets the name of the component.
		/// </summary>
		/// <returns></returns>
		template<typename T>
		std::string getComponentName() {
			return typeid(T).name();
		}

	public:
		// Defines what layer the object is on to prevent z-fighting
		/// \todo add support for multiple layers.
		int depth = 0;
		Model model;

		// positional variables that are used to draw the object
		Transform transform;
		Color tint = WHITE;
				
		
		/// <summary>
		/// Adds a component.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		template<typename T>
		void addComponent() {
			std::string componentName = getComponentName<T>();
			
			if (componentName.empty()) {
				throw std::invalid_argument("Component name cannot be empty");
			}

			mComponents[componentName] = std::make_unique<T>();
			mComponents[componentName]->setGameObject(std::make_shared<IGameObject>(this));
		}

		/// <summary>
		/// Gets a component.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		/// <returns></returns>
		template<typename T>
		std::shared_ptr<T> getComponent() {
			std::string componentName = getComponentName<T>();
			return std::static_pointer_cast<T>(mComponents[componentName]);
		}

		/// <summary>
		/// Removes a component.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		template<typename T>
		void removeComponent() {
			std::string componentName = getComponentName<T>();
			mComponents.erase(componentName);
		}

		/// <summary>
		/// Determines whether this instance has a specified component.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		/// <returns>
		///   <c>true</c> if this instance has component; otherwise, <c>false</c>.
		/// </returns>
		template<typename T>
		bool hasComponent() {
			std::string componentName = getComponentName<T>();
			return mComponents.find(componentName) != mComponents.end();
			
		}
		
		/// <summary>
		/// Default constructor that initializes a new instance of the <see cref="IGameObject"/> class.
		/// </summary>
		IGameObject() = default;
		
		/// <summary>
		/// Default destructor that finalizes an instance of the <see cref="IGameObject"/> class.
		/// </summary>
		virtual ~IGameObject() = default;
		
		/// <summary>
		/// Pres the update.
		/// </summary>
		virtual void preUpdate() {
			for (auto& component : mComponents) {
				if (component.second != nullptr) {
					component.second->preUpdate();
				}
			}
		}
				
		/// <summary>
		/// Updates this instance.
		/// </summary>
		virtual void update() {
			for (auto& component : mComponents) {
				if (component.second != nullptr) {
					component.second->update();
				}
			}
		}
		
		/// <summary>
		/// Posts the update.
		/// </summary>
		virtual void postUpdate() {
			for (auto& component : mComponents) {
				if (component.second != nullptr) {
					component.second->postUpdate();
				}
			}
		}

		/// <summary>
		/// This is a pure virtual function that is the drawing function for when objects are being textured to the render texture. If you do not have anything different to do here, then you may
		/// just call <see cref="render()" /> instead, but this must be implemented in order for the object to be rendered.
		/// </summary>
		virtual void texture() {
			for (auto& component : mComponents) {
				if (component.second != nullptr) {
					component.second->texture();
				}
			}
		}

		/// <summary>
		/// Renders this instance. This is not a pure virtual and is empty because it is up to the user to use this as they see fit without forcing them to implement it.
		/// </summary>
		virtual void render() {
			// See the function's comments for the reason why this is empty
			for (auto& component : mComponents) {
				if (component.second != nullptr) {
					component.second->render();
				}
			}
		}

		/// <summary>
		/// Destroys this instance, and is called right before the object is removed from memory. 
		/// \note This must be implemented by the user.
		/// </summary>
		/// <inheritdoc />
		virtual void destroy() {
			for (auto& component : mComponents) {
				if (component.second != nullptr) {
					component.second->destroy();
				}
			}
		}

		/// <summary>
		/// Cleanups this instance.
		/// \note This must be implemented by the user.
		/// </summary>
		/// <inheritdoc />
		virtual void cleanup() {
			for (auto& component : mComponents) {
				if (component.second != nullptr) {
					component.second->cleanup();
					component.second.reset();
				}
			}
		}
	};
}
