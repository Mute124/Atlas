/**
* @file Input.h
* 
* @brief The input system for Atlas.
*
* @details The input system for Atlas works by a Laissez-faire system where the user can register InputActions (<see cref="InputAction"/>) by using functions as callbacks. The reason
* for this is because it is much easier to do it this way (and it is also more efficient & flexible). Plus, it also allows you to register multiple callbacks for the same InputAction, 
* which may be useful.
* 
* @todo Create examples of how to use @ref Atlas::InputRegistry.
* @todo Document @ref Atlas::EInputTrigger & @ref Atlas::EInputType
*/
#pragma once
#include "EInputThread.h"
#include "EInputTrigger.h"
#include "EInputType.h"
#include "InputAction.h"
#include "SelfDeterminingInputAction.h"
#include "InputRegistry.h"

#define ATLAS_ENABLE_CONTROLLER_SUPPORT

namespace Atlas {	
	
}