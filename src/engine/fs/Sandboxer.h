
#pragma once
#include "ISandboxer.h"
#include <unordered_map>
#include <string>

namespace Atlas {
	/// <summary>
	/// This is what is used to determine if a path is safe or not.
	/// </summary>
	/// <seealso cref="ISandboxer" />
	class Sandboxer : public ISandboxer {
	private:
		std::vector<std::string> mSafePaths;
	public:

		/// <summary>
		/// Determines whether [is safe path] [the specified path].
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns>
		///   <c>true</c> if [is safe path] [the specified path]; otherwise, <c>false</c>.
		/// </returns>
		bool isSafePath(FilePath* path) override;
		
		/// <summary>
		/// Adds the safe path.
		/// </summary>
		void addSafePath() override;
	};
}