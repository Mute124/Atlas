#pragma once

namespace Atlas {
	struct ValidityCheckResult {
		const bool cIsValid;
		const bool cDoesExist;

		ValidityCheckResult(bool cIsValid, bool cDoesExist)
			: cIsValid(cIsValid), cDoesExist(cDoesExist)
		{
		}
	};
}