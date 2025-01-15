#pragma once
#include "../../engine/Project.h"
#include "SampleGroup.h"

#include "basic/BasicSampleGroup.h"

#include <string>
#include <unordered_map>





class SamplesProject final : public IProject
{
public:
	SamplesProject() : IProject() {}

	void postInit() override {
		addSampleGroup(new BasicSampleGroup());
	}

	void addSampleGroup(SampleGroup* group) {
		mSamples[group->getName()] = group;
	}

	SampleGroup* getSampleGroup(std::string name) {
		return mSamples[name];
	}

	int update() override {
		return 0;
	}

private:
	static inline std::unordered_map<std::string, SampleGroup*> mSamples;
};