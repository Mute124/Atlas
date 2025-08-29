#pragma once

namespace Atlas {
	class MakeNonCopyable {
	public:
		MakeNonCopyable() = default;

		MakeNonCopyable(const MakeNonCopyable&) = delete;

		void operator=(const MakeNonCopyable&) = delete;
	};
}
