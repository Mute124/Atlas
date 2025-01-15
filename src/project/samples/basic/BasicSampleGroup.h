#pragma once
#include "../SampleGroup.h"
#include "HelloWorldSample.h"


class BasicSampleGroup final : public SampleGroup {
public:
	BasicSampleGroup() : SampleGroup("Basic") {
		addSample(new HelloWorldSample());
	}
};