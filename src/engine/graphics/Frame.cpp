
#include "Frame.h"
#include "backend/VKDevice.h"

void Atlas::Frame::updateFrameCount() {
	// Using this method rather than simply incrementing the frame count through mFrameCount++
	// because the setFrameCount function has checks
	frameCounter.increment();
}

void Atlas::Frame::update()
{
	updateFrameCount();
}

Atlas::FrameCount Atlas::Frame::getFrameCount() const {
	return frameCounter.getCount();
}
