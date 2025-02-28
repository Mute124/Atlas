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

namespace Atlas {

	/**
	 * @brief This concept is used to ensure that the type is either a std::unordered_set or std::set.
	 * @tparam T_SET_TYPE The type to check. This must be either a std::unordered_set or std::set.
	 */
	//template<typename T_SET_TYPE, typename T_KEY>
	//concept RegistrySetType = requires(T_SET_TYPE t) {
	
	//};

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

	void t() {
		
	}
}