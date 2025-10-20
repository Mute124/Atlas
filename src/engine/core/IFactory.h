/**************************************************************************************************
 * @file IFactory.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <memory>

namespace Atlas {
	
	/**
	 * @brief A pure virtual factory interface for creating unique pointer objects. 
	 * 
	 * @tparam T_BUILDS A class type that will be created by this factory.
	 * 
	 * @tparam T_BUILD_ARGS The varidic arguments that will be the parameters for the full create function.
	 * 
	 * @since v0.0.1
	 */
	template<typename T_BUILDS, typename... T_BUILD_ARGS>
	class IFactory {
	public:

		/**
		 * @brief Default destructor. This is being declared virtual for the sake of polymorphism
		 * 
		 * @since v0.0.1
		 */
		virtual ~IFactory() = default;
		
		/**
		 * @brief When implemented, this should create a new object using the provided arguments. 
		 * 
		 * @param ... The varidic arguments that will be used to construct the object
		 * 
		 * @return The created object of type @c T_BUILDS if successful, otherwise nullptr should be returned or throw an exception.
		 * 
		 * @since v0.0.1
		 */
		virtual std::unique_ptr<T_BUILDS> create(T_BUILD_ARGS...) = 0;

		/**
		 * @brief When implemented, this should create a new object using default configurations.
		 * 
		 * @note This is a convenience function for creating objects with default configurations.
		 * 
		 * @returns The created object of type @c T_BUILDS if successful, otherwise nullptr should be returned or throw an exception.
		 * 
		 * @since v0.0.1
		 */
		virtual std::unique_ptr<T_BUILDS> create() = 0;

		/**
		 * @brief When implemented, this should clone an existing object.
		 * 
		 * @param cSourceRef The source object instance to clone.
		 * 
		 * @return A unique pointer to the cloned object if successful, otherwise nullptr should be returned or throw an exception.
		 * 
		 * @since v0.0.1
		 */
		virtual std::unique_ptr<T_BUILDS> clone(const T_BUILDS& cSourceRef) = 0;
	};

}
