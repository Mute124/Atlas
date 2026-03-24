/**************************************************************************************************
 * @file FPS.cpp
 * 
 * @brief This file contains the implementation of the FPS class.
 * 
 * @date March 2026
 * 
 * @since v0.0.1
 * 
 *  Copyright 2024 Mute124
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License. 
 * ***************************************************************************************************/
#include <chrono>

#include "FPS.h"

double Atlas::FPS::Now() {
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count(); 
}

void Atlas::FPS::updateAveragedFPS() {
	incrementFrameCount();

	// If a second has passed
	if (hasSecondPassed()) {
		// Since a second has passed, the FPS is the frame count.
		// Additionally, the static_cast is used to convert the 
		// frame count to a float for the FPS.
		setFPS(static_cast<float>(getFrameCount()));
		// Reset the counters
		mFrameCount = 0;
		mPreviousTime = mCurrentTime;
	}
}

void Atlas::FPS::updateRealTimeFPS() {
	setFPS(1.0f / (mCurrentTime - mPreviousTime));
	setPreviousTime(mCurrentTime);
	incrementFrameCount();
}

bool Atlas::FPS::hasSecondPassed() const { 
	return mCurrentTime - mPreviousTime >= 1.0;
}

void Atlas::FPS::update() {
	mCurrentTime = Now();

	if (mFPSCountingMethod == EFPSCountingMethod::Averaged) {
		updateAveragedFPS();
	}
	else {
		updateRealTimeFPS();
	}
}

void Atlas::FPS::reset()
{
	resetFrameCount();
	mFPS = 0.0;
	mCurrentTime = Now();
	mPreviousTime = mCurrentTime;
}

void Atlas::FPS::resetFrameCount() {
	mFrameCount = 0;
}

void Atlas::FPS::incrementFrameCount() { 
	++mFrameCount; 
}

void Atlas::FPS::setFPSCountingMethod(EFPSCountingMethod newFPSCountingMethod) {
	mFPSCountingMethod = newFPSCountingMethod;
}

void Atlas::FPS::setFrameCount(int newFrameCount) {
	mFrameCount = newFrameCount;
}

void Atlas::FPS::setFPS(double newFPS) {
	mFPS = newFPS;
}

void Atlas::FPS::setPreviousTime(double newPreviousTime) {
	mPreviousTime = newPreviousTime;
}

void Atlas::FPS::setCurrentTime(double newCurrentTime) {
	mCurrentTime = newCurrentTime;
}

int Atlas::FPS::getFrameCount() const {
	return mFrameCount;
}

Atlas::EFPSCountingMethod Atlas::FPS::getFPSCountingMethod() const {
	return mFPSCountingMethod;
}

double Atlas::FPS::getFPS() const { 
	return mFPS;
}

double Atlas::FPS::getPreviousTime() const {
	return mPreviousTime;
}

double Atlas::FPS::getCurrentTime() const {
	return mCurrentTime; 
}
