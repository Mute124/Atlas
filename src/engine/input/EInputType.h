#pragma once

namespace Atlas {
	/// <summary>
	/// The enumeration for the different input device categories. This is here because the input system is going to be expanded in the future and it will be useful to 
	/// maintaining developer sanity.
	/// </summary>
	enum class EInputType {
		INPUT_KEYBOARD,
		INPUT_MOUSE,
		INPUT_CONTROLLER
	};
}