#pragma once
#include "Sample.h"

#include <string>
#include <unordered_map>

using namespace Atlas;





class SampleGroup {
public:
	SampleGroup(std::string name) : name(name) {}

	void addSample(Sample* sample) {
		mSamples[sample->name] = sample;
	}

	std::string& getName() { return name; }

	virtual void startSample(std::string const& name) {
		mSamples[name]->init();
	}

private:
	std::string name;
	std::unordered_map<std::string, Sample*> mSamples;
};