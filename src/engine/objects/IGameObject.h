/**
* @brief test
*/
#pragma once
#include "../components/Component.h"
#include "../components/TransformComponent.h"

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

#include "../dbg/Errors.h"

namespace Atlas {
	/**
	 * @brief Represents an object that can be rendered in the screen and contains all the common properties of a game object.
	 * @remarks 
	 * @paragraph gameobjects-interface-uses Class Use Cases
	 * You may implement this yourself to create your own game objects and drawing code, or you can inherit from @ref GameObject to use the default
	 * implementation. However, if you inherit from this class, be sure you have a basic understanding of Raylib's and OpenGL's rendering system. If not,
	 * see @link https://learnopengl.com @endlink or @link https://www.raylib.com @endlink for a good place to start. In addition, it is not advised for you to inherit this
	 * yourself because it will require you to implement rendering code. However, it @a is recommended if your game requires a rendering system that is not provided by default. 
	 * If you do implement your own rendering system, be sure to ensure that the @ref Renderer used in the @ref IAtlas derived class or @ref AtlasEngine class can be used with
	 * your code.
	 * @endparblock
	 * @note The IGameObject class is abstract and cannot be instantiated directly. Instead, you should inherit from this class and implement the pure virtual functions. As of version 0.0.3, the pure 
	 * virtual functions are: @ref render(), @ref texture(), @ref update(), @ref prePhysicsUpdate(), @ref postUpdate(), @ref destroy(), and @ref cleanup().
	 * @note This is an abstract class 
	 * @todo add support for multiple layers.
	 * @since v0.0.3
	 * @sa @ref Component for the component class
	 */
	class IGameObject {
	protected:

		/**
		 * @brief Represents an exception that is thrown when a component name ends up being invalid.
		 * @since v0.0.9
		 * @sa @ref InvalidValue for the base class
		 * @sa @ref addComponent() for where this exception is thrown
		 * @sa @ref getComponentName() for how the component name is generated
		 */
		class InvalidComponentNameException : public InvalidValue {
		public:

			/**
			 * @brief Constructs a new instance of the @ref InvalidComponentNameException class.
			 * @param message The message to be associated with the exception.
			 * @param location The source location of the exception.
			 * @since v0.0.9
			 */
			InvalidComponentNameException(std::string const& message, std::source_location location = std::source_location::current()) : InvalidValue(message, location) {}
		};

		std::map<std::string, std::shared_ptr<Component>> mComponents;
		
		/**
		 * @brief Gets the name of a component.
		 * @remarks This is done by using the typeid of @ref T_COMPONENT_TYPE. This is done to ensure that the component name is unique
		 * and that it is not the same as another component name. Furthermore, it is done to make it easier to identify the type of the
		 * component without having to manually define a string for each component.
		 * @tparam T_COMPONENT_TYPE The type of the component.
		 * @return A string that corrosponds to the type of the component
		 * @since v0.0.7
		 * @sa @ref addComponent() for where this function is used
		 */
		template<class T_COMPONENT_TYPE>
		std::string getComponentName() {
			return typeid(T_COMPONENT_TYPE).name();
		}

	public:
		/**
		 * @brief Represents an exception that is thrown when a game object is invalid.
		 * @since v0.0.9
		 * @sa @ref InvalidValue for the base class
		 */
		class InvalidGameObjectException : public InvalidValue {
		public:
			/**
			 * @brief The exception's constructor
			 * @param message What message to be associated with the exception
			 * @param location The source location of the exception
			 * @since v0.0.9
			 */
			InvalidGameObjectException(std::string const& message, std::source_location location = std::source_location::current()) : InvalidValue(message, location) {}
		};

		/**
		 * @brief Represents an exception that is thrown when a game object is a nullptr. This is a final subclass of @ref InvalidGameObjectException
		 * @since v0.0.9
		 * @sa @ref InvalidGameObjectException for the parent class
		 * @sa @ref InvalidValue for the base class
		 */
		class NullptrGameObjectException final : public InvalidGameObjectException {
		public:

			/**
			 * @copydoc InvalidGameObjectException::InvalidGameObjectException
			 */
			NullptrGameObjectException(std::string const& message, std::source_location location = std::source_location::current()) : InvalidGameObjectException(message, location) {}
		};

		// Defines what layer the object is on to prevent z-fighting
		/// \todo add support for multiple layers.
		int depth = 0;

		Model model;
		Vector3 position;
		// positional variables that are used to draw the object
		Transform transform;
		Color tint = WHITE;

		/**
		 * @brief Default destructor that finalizes an instance of the @ref IGameObject class.
		 * @since v0.0.3
		 */
		virtual ~IGameObject() = default;

		/**
		 * @brief Adds a component to this game object
		 * @throws InvalidComponentNameException if the component name is empty
		 * @note The type parameter @ref T_COMPONENT_TYPE must be a subclass of @ref Component
		 * @tparam T_COMPONENT_TYPE The type of the component being added.
		 * @since v0.0.7
		 * @sa @ref removeComponent()
		 * @sa @ref getComponent() 
		 * @sa @ref Component
		 * @sa @ref mComponents for the map of components. Aka the variable where components are stored
		 */
		template<class T_COMPONENT_TYPE>
		void addComponent() {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();
			
			if (componentName.empty()) {
				throw InvalidComponentNameException("Component name cannot be empty");
			}

			mComponents[componentName] = std::make_shared<T_COMPONENT_TYPE>(/*T(std::forward<Args>(args)...)*/);
			mComponents[componentName]->setOwner(this);
		}

		/**
		 * @brief Adds a component to this game object
		 * @remarks The component is added by making a shared pointer of the @ref component parameter and assigning it to the component name in @ref mComponents.
		 * @throws InvalidComponentNameException if the component name is empty
		 * @tparam T_COMPONENT_TYPE The type of the component being added.
		 * @param component The component to be added.
		 * @since v0.0.7
		 * @sa @ref removeComponent()
		 * @sa @ref getComponent()
		 * @sa @ref Component
		 * @sa @ref mComponents for the map of components. Aka the variable where components are stored
		 */
		template<class T_COMPONENT_TYPE>
		void addComponent(T_COMPONENT_TYPE component) {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();

			if (componentName.empty()) {
				throw InvalidComponentNameException("Component name cannot be empty");
			}

			mComponents[componentName] = std::make_shared<T_COMPONENT_TYPE>(component);
			mComponents[componentName]->setOwner(this);
		}

		/**
		 * @brief Adds a component to this game object.
		 * @note This will construct the component using the arguments provided
		 * @throws InvalidComponentNameException if the component name is empty
		 * @tparam T_COMPONENT_TYPE The type of the component being added.
		 * @tparam ...Args A variadic parameter pack of arguments to be forwarded to the constructor of the component.
		 * @param ...args The arguments to be forwarded to the constructor of the component.
		 * @since v0.0.7
		 * @sa @ref removeComponent() for removing a component
		 * @sa @ref getComponent() for getting a component
		 * @sa @ref Component for the component class
		 * @sa @ref mComponents for the map of components. Aka the variable where components are stored
		 */
		template<class T_COMPONENT_TYPE, typename ...Args>
		void addComponent(Args... args) {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();

			if (componentName.empty()) {
				throw InvalidComponentNameException("Component name cannot be empty");
			}

			mComponents[componentName] = std::make_shared<T_COMPONENT_TYPE>(T_COMPONENT_TYPE(args));
			mComponents[componentName]->setOwner(this);
		}


		/**
		 * @brief Gets a currently stored component of template type: @ref T_COMPONENT_TYPE
		 * @tparam T_COMPONENT_TYPE The type of the component AS IT IS STORED in @ref mComponents
		 * @return The component of template type: @ref T_COMPONENT_TYPE
		 * @since v0.0.7
		 * @sa @ref addComponent() for adding a component
		 * @sa @ref removeComponent() for removing a component
		 * @sa @ref Component for the component class
		 * @sa @ref mComponents for the map of components. Aka the variable where components are stored
		 */
		template<class T_COMPONENT_TYPE>
		std::shared_ptr<T_COMPONENT_TYPE> getComponent() {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();
			auto component = mComponents[componentName];
			return std::dynamic_pointer_cast<T_COMPONENT_TYPE>(component);
		}

		/**
		 * @brief Removes a component from this game object
		 * @remarks This does a simple erase of the component name from @ref mComponents
		 * @tparam T_COMPONENT_TYPE The type of the component being removed.
		 * @since v0.0.7
		 * @sa @ref addComponent() for adding a component
		 * @sa @ref getComponent() for getting a component
		 * @sa @ref Component for the component class
		 * @sa @ref mComponents for the map of components. Aka the variable where components are stored
		 */
		template<class T_COMPONENT_TYPE>
		void removeComponent() {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();
			mComponents.erase(componentName);
		}
		
		/**
		 * @brief Determines whether this instance has a specified component.
		 * @tparam T_COMPONENT_TYPE The type of the component, <b>AS IT IS STORED</b> in @ref mComponents.
		 * @return @c true if this instance has a specified component; otherwise, it will return @c false.
		 * @since v0.0.7
		 * @sa @ref addComponent() for adding a component
		 * @sa @ref getComponent() for getting a component
		 * @sa @ref Component for the component class
		 * @sa @ref mComponents for the map of components. Aka the variable where components are stored
		 * @sa @ref getComponentName() for getting the component name
		 */
		template<class T_COMPONENT_TYPE>
		bool hasComponent() {
			std::string componentName = getComponentName<T_COMPONENT_TYPE>();
			return mComponents.contains(componentName);
		}
		
		/**
		 * @brief Prepares this instance for the main update. This is a @b pure virtual function and @b must be implemented in order for the object to be updated
		 * @note This is called before @ref update()
		 * @since v0.0.7
		 */
		virtual void preUpdate() = 0;
				
		/**
		 * @brief The main update function. This is a @b pure virtual function and @b must be implemented in order for the object to be updated
		 * @since v0.0.1
		 */
		virtual void update() = 0;
		
		/**
		 * @brief Anything that needs to be done after the main update. This is a pure virtual function and must be implemented in order for the object to be updated.
		 * @since v0.0.1
		 */
		virtual void postUpdate() = 0;

		/**
		 * @brief A pure virtual function that is the drawing function for when objects are being textured to the render texture.
		 * @remarks If you do not have anything different to do here, then you may just call @ref render() instead, but this must be implemented in order for the object to be rendered
		 * because the rendering system draws game objects to a render texture.
		 * @since v0.0.1
		 */
		virtual void texture() = 0;

		/**
		 * @brief A pure virtual function that is the rendering function that will be called @b AFTER the render texture has been drawn to the screen.
		 * @remarks You can draw objects within this function, but behavior will be undefined if you do so.
		 * @since v0.0.1
		 */
		virtual void render() = 0;

		/**
		 * @brief A pure virtual function that is called when the object is being destroyed. This happens right before the object is removed from memory.
		 * @since v0.0.1
		 */
		virtual void destroy() = 0;

		/**
		 * @brief Cleans up this instance.
		 * @todo Is this necessary? @ref destroy() can take on this responsibility
		 * @since v0.0.1
		 */
		virtual void cleanup() = 0;
	};
}
