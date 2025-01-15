#pragma once
#include <string>

class Sample {
public:

	Sample(std::string name) : name(name) {}

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	std::string name;
};