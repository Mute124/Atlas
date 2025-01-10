#pragma once

namespace Atlas {
	/// <summary>
	/// A class that acts as, well, a gate! It is essentially as a piece of machinery on an assembly line. Please do note, that this is a template class and is not meant to be
	/// used directly. Please see <see cref="GameObjectRegistry" /> for more information and an example.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <seealso cref="GameObjectRegistry" />
	template<typename T>
	class IGate abstract {
	public:

		/// <summary>
		/// Processes the specified entree.
		/// </summary>
		/// <param name="entree">The entree to process.</param>
		/// <typeparam name="T">The type of the entree. It is already set by the gate's template.</typeparam>
		virtual void process(T entree) = 0;
	};
}
