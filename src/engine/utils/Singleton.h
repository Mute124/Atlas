/**
* \file Singleton.h
* 
* \brief Contains the Singleton class template that is used to create "static classes". If you need to create a class that has a single static instance, use this class.
*/
#pragma once
#define ATLAS_SINGLETON(T) public Singleton<T>

namespace Atlas {
	/// <summary>
	/// A singleton class, it does what singletons do. 
	/// \warning The template (T) must be the type that is inheriting from this class, otherwise there will be issues.
	/// </summary> 
	/// <typeparam name="T">The typename of the class that is inheriting from this class</typeparam>
	template<typename T>
	class Singleton
	{
	public:

		/// <summary>
		/// Retrieves the current singleton instance as a reference. If you need to create a class that has a single static instance, use this class.
		/// \note Please note that this is the only way a singleton instance should be retrieved.
		/// </summary>
		/// <returns>A reference to the current singleton instance</returns>
		static T& Instance()
		{
			static T instance;
			return instance;
		}
	};
}
