#pragma once
#ifdef ATLAS_ENABLE_MODDING
	#ifdef ATLAS_ENABLE_LUA
		/// \brief Protects against safety issues when using lua because it makes the API more stable.
		#define SOL_ALL_SAFETIES_ON 1

		#include <sol/state.hpp>
		#include <sol/state_view.hpp>
		#include <sol/types.hpp>
		#include <sol/forward.hpp>
	#endif

#endif
namespace Atlas {

}
