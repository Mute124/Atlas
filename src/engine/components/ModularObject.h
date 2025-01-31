#pragma once
#include <string>
#include <map>
#include <memory>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "Component.h"

namespace Atlas {
	class ModularObject {
	protected:
		std::map<std::string, std::shared_ptr<Component>, std::less<>> mComponents;

	public:
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

			mComponents[componentName] = std::make_shared<T>();
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
			return std::static_pointer_cast<std::shared_ptr<T>>(mComponents[componentName]);
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
			return mComponents.contains(componentName);

		}

		/// <summary>
		/// Pres the update.
		/// </summary>
		virtual void preUpdate() {
			for (auto& [key, component] : mComponents) {
				if (component.get() != nullptr) {
					component->preUpdate(new PreUpdateEventArgs());
				}
			}
		}

		/// <summary>
		/// Updates this instance.
		/// </summary>
		virtual void update() {
			for (auto& [key, component] : mComponents) {
				if (component.get() != nullptr) {
					component->update(new UpdateEventArgs());
				}
				else {
					throw std::invalid_argument("Component " + key + "cannot be null");
				}
			}
		}

		/// <summary>
		/// Posts the update.
		/// </summary>
		virtual void postUpdate() {
			for (auto& [key, component] : mComponents) {
				if (component.get() != nullptr) {
					component->postUpdate(new PostUpdateEventArgs());
				}
				else {
					throw std::invalid_argument("Component " + key + "cannot be null");
				}
			}
		}

		/// <summary>
		/// This is a pure virtual function that is the drawing function for when objects are being textured to the render texture. If you do not have anything different to do here, then you may
		/// just call <see cref="render()" /> instead, but this must be implemented in order for the object to be rendered.
		/// </summary>
		virtual void texture() {
			for (auto& [key, component] : mComponents) {
				if (component.get() != nullptr) {
					component->texture(new TextureEventArgs());
				}
				else {
					throw std::invalid_argument("Component " + key + "cannot be null");
				}
			}
		}

		/// <summary>
		/// Renders this instance. This is not a pure virtual and is empty because it is up to the user to use this as they see fit without forcing them to implement it.
		/// </summary>
		virtual void render() {
			// See the function's comments for the reason why this is empty
			for (auto& [key, component] : mComponents) {
				if (component.get() != nullptr) {
					component->render(new RenderEventArgs());
				}
				else {
					throw std::invalid_argument("Component " + key + "cannot be null");
				}
			}
		}

		/// <summary>
		/// Destroys this instance, and is called right before the object is removed from memory. 
		/// \note This must be implemented by the user.
		/// </summary>
		/// <inheritdoc />
		virtual void destroy() {
			for (auto& [key, component] : mComponents) {
				if (component.get() != nullptr) {
					component->destroy(new DestroyEventArgs());
				}
				else {
					throw std::invalid_argument("Component " + key + "cannot be null");
				}
			}
		}

		/// <summary>
		/// Cleanups this instance.
		/// \note This must be implemented by the user.
		/// </summary>
		/// <inheritdoc />
		virtual void cleanup() {
			for (auto& [key, component] : mComponents) {
				if (component.get() != nullptr) {
					component->cleanup(new CleanupEventArgs());
				}
				else {
					throw std::invalid_argument("Component " + key + "cannot be null");
				}
			}
		}
	};
}