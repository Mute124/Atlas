#pragma once
#include <string>
#include <any>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include "../fs/FileMeta.h"
namespace Atlas {
	/**
	 * @brief This class is used to make an object serializable and deserializable. This class is intended
	 * to be inherited by other classes. If you want to make a class serializable, inherit this class and you must override
	 * the @ref serializeVariables function. Do @b NOT touch the @ref serialize function because that is what
	 * boost uses to serialize the object. In the @ref serializeVariables function, you should set the values. 
	 * @remarks If you are curious about how Boost serializes objects, check out their documentation.
	 * @since v0.0.9
	 */
	class Serializable {
	protected:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int cVersion) {
			serializeVariables(ar, cVersion);
		}


		/**
		 * @brief This is where you should actually set the values. Keep in mind that the archive is an any object.
		 * This is because the function needs to be virtual, but templates do not support virtual functions. As such, 
		 * this is a workaround to it. Is it annoying? Sure, but it works. 
		 * @param archive An any object reference to the archive. 
		 * @param cVersion The current version of the object.
		 * @since v0.0.9
		 * 
		 * @sa @ref serialize
		 */
		virtual void serializeVariables(std::any& archive, const unsigned int cVersion) = 0;

		
	public:
		boost::uuids::uuid UUID;

		virtual ~Serializable() {}
	};
}