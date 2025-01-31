#pragma once
#include "../math/Transform.h"
#include "../components/Component.h"

#include <memory>
#include <unordered_map>
#include <map>
#include <raylib.h>

#include "../components/CleanupEventArgs.h"
#include "../components/DestroyEventArgs.h"
#include "../components/PostUpdateEventArgs.h"
#include "../components/PreUpdateEventArgs.h"
#include "../components/RenderEventArgs.h"
#include "../components/TextureEventArgs.h"
#include "../components/UpdateEventArgs.h"

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>


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
		std::map<std::string, std::shared_ptr<Component>, std::less<>> mComponents;

		
		/// <summary>
		/// Gets the name of the component.
		/// </summary>
		/// <returns></returns>
		template<typename T_COMPONENT_TYPE>
		std::string getComponentName() {
			return typeid(T_COMPONENT_TYPE).name();
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
		/// Default constructor that initializes a new instance of the <see cref="IGameObject"/> class.
		/// </summary>
		IGameObject() = default;

		/// <summary>
		/// Default destructor that finalizes an instance of the <see cref="IGameObject"/> class.
		/// </summary>
		virtual ~IGameObject() = default;
		
		/// <summary>
		/// Adds a component.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		template<typename T_COMPONENT_TYPE>
		void addComponent() {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();
			
			if (componentName.empty()) {
				throw std::invalid_argument("Component name cannot be empty");
			}

			mComponents[componentName] = std::make_shared<T_COMPONENT_TYPE>(/*T(std::forward<Args>(args)...)*/);
			mComponents[componentName]->setOwner(std::make_shared<IGameObject>(this));
		}

		/// <summary>
		/// Gets a component.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		/// <returns></returns>
		template<typename T_COMPONENT_TYPE>
		std::shared_ptr<T_COMPONENT_TYPE> getComponent() {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();
			return std::static_pointer_cast<std::shared_ptr<T_COMPONENT_TYPE>>(mComponents[componentName]);
		}

		/// <summary>
		/// Removes a component.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		template<typename T_COMPONENT_TYPE>
		void removeComponent() {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();
			mComponents.erase(componentName);
		}

		/// <summary>
		/// Determines whether this instance has a specified component.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		/// <returns>
		///   <c>true</c> if this instance has component; otherwise, <c>false</c>.
		/// </returns>
		template<typename T_COMPONENT_TYPE>
		bool hasComponent() {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();
			return mComponents.contains(componentName);
			
		}
		
		/// <summary>
		/// Pres the update.
		/// </summary>
		virtual void preUpdate() = 0;
				
		/// <summary>
		/// Updates this instance.
		/// </summary>
		virtual void update() = 0;
		
		/// <summary>
		/// Posts the update.
		/// </summary>
		virtual void postUpdate() = 0;

		/// <summary>
		/// This is a pure virtual function that is the drawing function for when objects are being textured to the render texture. If you do not have anything different to do here, then you may
		/// just call <see cref="render()" /> instead, but this must be implemented in order for the object to be rendered.
		/// </summary>
		virtual void texture() = 0;

		/// <summary>
		/// Renders this instance. This is not a pure virtual and is empty because it is up to the user to use this as they see fit without forcing them to implement it.
		/// </summary>
		virtual void render() = 0;

		/// <summary>
		/// Destroys this instance, and is called right before the object is removed from memory. 
		/// \note This must be implemented by the user.
		/// </summary>
		/// <inheritdoc />
		virtual void destroy() = 0;

		/// <summary>
		/// Cleanups this instance.
		/// \note This must be implemented by the user.
		/// </summary>
		/// <inheritdoc />
		virtual void cleanup() = 0;
	};
}
