#pragma once

namespace Atlas {
	template<typename T>
	class OptionalReference {
	public:
		T& referenceValue;

		explicit OptionalReference(T& value) : referenceValue(value) {}
		
	};
}