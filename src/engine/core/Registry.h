/*****************************************************************//**
 * @file   Registry.h
 * 
 * @brief  
 * 
 * @date   March 2025
 * 
 * @since 
 *********************************************************************/
#pragma once

#include <string>
#include <cstdint>
#include <unordered_set>
#include <set>
#include <map>
#include <vector>
#include <unordered_map>
#include <list>
#include <functional>
#include <type_traits>
#include <iterator>

#include "CompileTimeCommons.h"



namespace Atlas {

	/**
	 * @brief This concept is used to ensure that the type is either a std::unordered_set or std::set.
	 * @tparam T_SET_TYPE The type to check. This must be either a std::unordered_set or std::set.
	 */
	//template<typename T_SET_TYPE, typename T_KEY>
	//concept RegistrySetType = requires(T_SET_TYPE t) {
	
	//};

	/**
	 * @brief A [heavily] templated interface for a registry that
	 * provides basic functionality for storing and retrieving
	 * entities, along with some additional utility features.
	 * 
	 * @tparam T_KEY The type of the ID of the entity (what it
	 * should be stored as in the set and the container).
	 * 
	 * @tparam T_CONTAINER The type of the container used to store
	 * the entities. 
	 * 
	 * @tparam T_ENTITY_TYPE The type of the entity stored in the
	 * container. This is also used when adding an entity to the
	 * container, so make sure to plan accordingly.
	 * 
	 * @tparam T_ENTITY_REF_TYPE The type of the reference to the
	 * entity stored in the container. This is used when getting
	 * an entity from the container. By default, it is a reference
	 * wrapper around the entity type.
	 * 
	 * @tparam T_CONTAINER_REF_TYPE The type of the reference to the
	 * container. This is used when getting the container from the
	 * registry. By default, it is a reference wrapper around the
	 * container type.
	 * 
	 * @tparam T_SIZE_TYPE The type to use when getting the size
	 * of the container. By default, it is size_t as this is the
	 * most common type. However, this template parameter exists
	 * here to ensure that this interface is flexible.
	 * 
	 * @since v0.0.9
	 */
	template<
		typename T_KEY,
		typename T_CONTAINER,
		typename T_ENTITY_TYPE,
		typename T_ENTITY_REF_TYPE = std::reference_wrapper<T_ENTITY_TYPE>,
		typename T_CONTAINER_REF_TYPE = std::reference_wrapper<T_CONTAINER>,
		typename T_SIZE_TYPE = size_t
	>
	class IRegistry {
	protected:
		/**
		 * @brief This pure virtual function should return a
		 * reference to the container that is being stored.
		 * 
		 * @note This function is protected to allow derived
		 * classes the ability to choose if they want it to
		 * be accessible or not.
		 * 
		 * @return v0.0.9
		 * 
		 * @since v0.0.9
		 */
		virtual T_CONTAINER_REF_TYPE getContainer() = 0;
	public:

		/**
		 * @brief .
		 * 
		 * @since 
		 */
		virtual ~IRegistry() {
			if (!isEmpty()) {
				clear();
			}
		}

		/**
		 * @brief This pure virtual function should add an entity
		 * to the container.
		 * 
		 * @param id The location within the container where the
		 * entity should be stored. If the container does not 
		 * work off of an ID (ie. a vector), then the overload
		 * that does not take an ID should be used instead.
		 * However, this function still needs to be implemented
		 * in the derived class as it is a pure virtual function.
		 * 
		 * @param entity 
		 * 
		 * @since v0.0.9
		 */
		virtual void emplace(T_KEY id, T_ENTITY_TYPE entity) = 0;

		/**
		 * @brief .
		 * 
		 * @param entity
		 * 
		 * @since 
		 */
		virtual void emplace(T_ENTITY_TYPE entity) = 0;

		/**
		 * @brief .
		 * 
		 * @param id
		 * 
		 * @since 
		 */
		virtual void remove(T_KEY id) = 0;

		/**
		 * @brief .
		 * 
		 * @param id
		 * 
		 * 
		 * @since 
		 */
		virtual void invalidate(T_KEY id) = 0;

		/**
		 * @brief .
		 * 
		 * @since 
		 */
		virtual void invalidateAll() = 0;

		/**
		 * @brief .
		 * 
		 * @param id
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		virtual bool exists(T_KEY id) = 0;

		/**
		 * @brief .
		 * 
		 * @param id
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		virtual bool isValid(T_KEY id) = 0;

		/**
		 * @brief .
		 * 
		 * @param id
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		virtual T_ENTITY_REF_TYPE get(T_KEY id) = 0;

		/**
		 * @brief .
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		virtual T_SIZE_TYPE getSize() = 0;

		/**
		 * @brief .
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		virtual bool isEmpty() = 0;

		/**
		 * @brief .
		 * 
		 * @since 
		 */
		virtual void clear() = 0;
	};

	/**
	 * @brief This class serves as a base class for all registry classes. It is able to lookup entities by their ID, remove them,
	 * add them, iterate over them, and check if they exist.
	 * @tparam T_KEY The type of the ID of the entity (what it should be stored as in the set and the container).
	 * @tparam T_SET_TYPE The type of the set used to store the entities. This should be either std::unordered_set or std::set.
	 * @tparam T_EQUAL The type of the equality function used to compare the IDs of the entities.
	 * @tparam T_CONTAINER The type of the container used to store the entities. This should be either a std::map, std::unordered_map,
	 * std::multimap, std::unordered_multimap, std::vector.
	 * @tparam T_HASH The type of the hash function used to hash the IDs of the entities.
	 * @since v0.0.9
	 */
	template<typename T_KEY, typename T_SET_TYPE, typename T_CONTAINER, 
		typename T_ENTITY_TYPE, typename T_HASH = std::hash<T_KEY>, typename T_EQUAL = std::equal_to<T_KEY>>
	class Registry {
	private:
		/**
		 * @brief A set (defined by T_SET_TYPE) of all the IDs of the entities in the registry. 
		 * This is used within this class to check if an entity exists. When an entity is pushed, 
		 * it is also added to this set. Similarly, when an entity is removed, it is also removed 
		 * from this set.
		 * @since v0.0.9
		 */
		T_SET_TYPE mEntityExistsLookupSet;

		/**
		 * @brief A container (defined by T_CONTAINER) of all the entities in the registry. This is
		 * used within this class to store entities. 
		 * @since v0.0.9
		 */
		T_CONTAINER mContainer;
	public:
		Registry() = default;

		T_ENTITY_TYPE get(T_KEY id) const {
			return mContainer.at(id);
		}

		void add(T_KEY id, T_ENTITY_TYPE entity) {
			mEntityExistsLookupSet.insert(id);
			mContainer.insert({ id, entity });
		}

		void remove(T_KEY id) {
			mEntityExistsLookupSet.erase(id);
			mContainer.erase(id);
		}

		void clear() {
			mEntityExistsLookupSet.clear();
			mContainer.clear();
		}

		bool isCleared() const {
			return mEntityExistsLookupSet.empty();
		}

		bool exists(T_KEY id) const {
			return mEntityExistsLookupSet.find(id) != mEntityExistsLookupSet.end();
		}

		std::reference_wrapper<T_CONTAINER> getContainer() {
			return mContainer;
		}

		std::reference_wrapper<T_SET_TYPE> getEntityExistsLookupSet() {
			return mEntityExistsLookupSet;
		}

		T_CONTAINER copyContainer() const {
			return mContainer;
		}

		int getSize() const {
			return mContainer.size();
		}

		template<typename T_FUNCTION>
		void forEach(T_FUNCTION function) {
		if (!function) {
				throw std::invalid_argument("Function pointer is null");
			}

			try {
				for (auto& [id, entity] : mContainer) {
					if (!entity) {
						throw std::runtime_error("Entity is null");
					}
					function(id, entity);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error(std::string("Error iterating over container: ") + e.what());
			}
		}

		operator[] (T_KEY id) const {
			return get(id);
		}

	};

}