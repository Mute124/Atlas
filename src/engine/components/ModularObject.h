#pragma once
#include <string>
#include <map>
#include <memory>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>



namespace Atlas {
	class Component;

	class ModularObject {
	protected:

		class ComponentPointer {
		public:
			Component* component;

			template<typename T_COMPONENT_TYPE>
			void init() {
				component = new T_COMPONENT_TYPE();
			}

			Component operator->() {
				return *component;
			}
		};

		std::map<std::string, std::shared_ptr<ComponentPointer>, std::less<>> mComponents;
		/// <summary>
		/// Gets the name of the component.
		/// </summary>
		/// <returns></returns>
		template<typename T_COMPONENT_TYPE>
		std::string getComponentName() {
			return typeid(T_COMPONENT_TYPE).name();
		}
	public:
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

			mComponents[componentName] = std::make_shared<ComponentPointer>(/*T(std::forward<Args>(args)...)*/);
			mComponents[componentName]->init<T_COMPONENT_TYPE>();
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
	};
}