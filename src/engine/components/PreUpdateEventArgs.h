#pragma once

namespace Atlas {	
	/// <summary>
	/// Used by the PreUpdate event for passing arguments to components to allow developers the ability to polymorph the arguments.
	/// </summary>
	struct PreUpdateEventArgs {
		float dt;
	};
}