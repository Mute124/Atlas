#pragma once

namespace Atlas {
	/// <summary>
	/// What sort of action should trigger the InputAction? 
	/// @warning Please pay attention when using this because any slight change in InputAction keys will give you entirely different input results.
	/// </summary>
	
	/**
	 * @brief What sort of action should trigger the InputAction? 
	 * @warning Please pay attention when using this because any slight change in InputAction keys will give you entirely different input results.
	 * @since v0.0.6
	 * @see EInputType
	 */
	enum class EInputTrigger {
		INPUT_TRIGGER_PRESSED,
		INPUT_TRIGGER_RELEASED,
		INPUT_TRIGGER_DOWN,
		INPUT_TRIGGER_UP,
		INPUT_TRIGGER_REPEAT
	};
}