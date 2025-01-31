#pragma once

namespace Atlas {
	class FilePath;

	class ISandboxer abstract {
	public:

		virtual bool isSafePath(FilePath* path) = 0;
		
		/// <summary>
		/// Adds the safe path.
		/// </summary>
		virtual void addSafePath() = 0;
	};
}