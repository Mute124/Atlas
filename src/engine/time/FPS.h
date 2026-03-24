/**************************************************************************************************
 * @file FrameTime.h
 * 
 * @brief Declarations for the FPS class, which provides an easy way to track your application's FPS.
 * 
 * @author Mute124
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
 ***************************************************************************************************/
#pragma once

#include <cstdint>

namespace Atlas {
	/**
	 * @brief Enumerates which FPS tracking method to use. See each enum for more information about each method.
	 * 
	 * @since v0.0.1
	 */
	enum class EFPSCountingMethod : uint8_t {
		/**
		 * @brief Uses the average of the number of frames counted in the last second to calculate the FPS, thereby
		 * giving a more stable result. This doesn't give the most accurate results, but is the most stable and is
		 * the recommended method. In other words, the FPS count is only updated once per second, compared to the 
		 * RealTime method, which updates the FPS count every frame.
		 * 
		 * @since v0.0.1
		 */
		Averaged = 0,

		/**
		 * @brief As the name suggests, real time FPS calculation involves calculating the FPS for each frame
		 * individually, resulting in a "live" FPS count. This is the most accurate, but may give jumpy results.
		 * 
		 * @since v0.0.1
		 */
		RealTime
	};

	/**
	 * @brief Provides an easy way to track your application's FPS using either a real-time or averaged method.
	 * 
	 * @note No attempt has been made to make the class thread-safe for the reason of keeping this class simple.
	 * 
	 * @since v0.0.1
	 * 
	 * @sa see @ref EFPSCountingMethod for tracking methods currently supported and the differences between them.
	 */
	class FPS {
	private:

		/**
		 * @brief The FPS counting method that this class should use to calculate the FPS. By default, this is
		 * set to averaged.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref EFPSCountingMethod
		 * @sa @ref setFPSCountingMethod
		 * @sa @ref getFPSCountingMethod
		 */
		EFPSCountingMethod mFPSCountingMethod{ EFPSCountingMethod::Averaged };

		/**
		 * @brief The current number of frames counted within the current second.
		 * 
		 * @since v0.0.1
		 */
		int mFrameCount{ 0 };
		
		/**
		 * @brief The most recently calculated FPS.
		 * 
		 * @since v0.0.1
		 */
		double mFPS{ 0.0 };

		/**
		 * @brief The time of the @b current and most recent update call. Not to be confused with the previous time.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref mPreviousTime
		 */
		double mCurrentTime{ Now() };

		/**
		 * @brief The time of the @b previous FPS update. Not to be confused with the current time.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref mCurrentTime
		 */
		double mPreviousTime{ mCurrentTime };
		

		/**
		 * @brief Returns the current time as a double. This utilizes a high resolution clock to get the current time
		 * from `std::Chrono`.
		 * 
		 * @return The current time in seconds.
		 * 
		 * @since v0.0.1
		 */
		static inline double Now();
	
		/**
		 * @brief Updates the FPS count using the averaged method. If a second has passed, the FPS is calculated by dividing
		 * the frame count by the time elapsed since the previous update. The frame count and previous time are then
		 * reset. This function will only be called if `mFPSCountingMethod` is set to `EFPSCountingMethod::Averaged`.
		 * 
		 * @since v0.0.1
		 */
		void updateAveragedFPS();

		/**
		 * @brief Updates the FPS count using the real-time method. This involves calculating the FPS for each frame individually,
		 * resulting in a "live" FPS count. The function calculates the FPS by dividing 1 by the time elapsed between the
		 * current and previous frames. The FPS count is then updated and the previous time is set to the current time.
		 * Additionally, the frame count is incremented. This function will only be called if `mFPSCountingMethod` is set to
		 * `EFPSCountingMethod::RealTime`.
		 * 
		 * @since v0.0.1
		 */
		void updateRealTimeFPS();

		/**
		 * Checks if a second has passed since the previous update.
		 *
		 * @return @a True if a second has passed, @a false otherwise.
		 */
		bool hasSecondPassed() const;

	public:
		
		/**
		 * @brief Updates the FPS count based on the current time and the FPS counting method.  This function updates the currently stored
		 * time to the current time using the `Now` function. Then it checks the FPS counting method and calls either the `updateAveragedFPS`
		 * or the `updateRealTimeFPS` function accordingly.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref EFPSCountingMethod and @ref mFPSCountingMethod for tracking methods currently supported and the differences between them.
		 * @sa @ref updateAveragedFPS for more information on the averaged method's implementation.
		 * @sa @ref updateRealTimeFPS for more information on the real-time method's implementation.
		 * @sa @ref Now for information on how the current time is gotten.
		 * @sa @ref mCurrentTime for more information on how the current time is stored.
		 */
		void update();

		/**
		 * @brief Resets the FPS count, frame count, and previous time to their default values. Does not reset the FPS counting method.
		 * 
		 * @since v0.0.1
		 */
		void reset();

		/**
		 * @brief Resets the frame count to 0.
		 * 
		 * @since v0.0.1
		 */
		void resetFrameCount();
		
		/**
		 * @brief Increments the frame count by 1.
		 * 
		 * @since v0.0.1
		 */
		void incrementFrameCount();

		/**
		 * @brief Sets the FPS counting method.
		 * 
		 * @param newFPSCountingMethod The new FPS counting method.
		 * 
		 * @since v0.0.1
		 */
		void setFPSCountingMethod(EFPSCountingMethod newFPSCountingMethod);

		/**
		 * @brief Sets the frame count to a new value.
		 * 
		 * @param newFrameCount The new frame count to set.
		 * 
		 * @since v0.0.1
		 */
		void setFrameCount(int newFrameCount);
		
		/**
		 * @brief Sets the FPS to a new value.
		 * 
		 * @param newFPS The new FPS to set.
		 * 
		 * @since v0.0.1
		 */
		void setFPS(double newFPS);

		/**
		 * @brief Sets the previous time to a new value.
		 * 
		 * @param newPreviousTime The new previous time to set.
		 * 
		 * @since v0.0.1
		 */
		void setPreviousTime(double newPreviousTime);

		/**
		 * @brief Sets the current time to a new value.
		 * 
		 * @param newCurrentTime The new current time to set.
		 * 
		 * @since v0.0.1
		 */
		void setCurrentTime(double newCurrentTime);

/**
		 * @brief Gets the frame count.
		 * 
		 * @return The current frame count.
		 * 
		 * @since v0.0.1
		 */
		int getFrameCount() const;

		/**
		 * @brief Gets the FPS counting method.
		 * 
		 * @return The current FPS counting method.
		 * 
		 * @since v0.0.1
		 */
		EFPSCountingMethod getFPSCountingMethod() const;

		/**
		 * @brief Gets the currently stored FPS.
		 * 
		 * @return The current FPS.
		 * 
		 * @since v0.0.1
		 */
		double getFPS() const;

		/**
		 * @brief Gets the previous time.
		 * 
		 * @return The previous time.
		 * 
		 * @since v0.0.1
		 */
		double getPreviousTime() const;

		/**
		 * @brief Gets the current time.
		 * 
		 * @return The current time.
		 * 
		 * @since v0.0.1
		 */
		double getCurrentTime() const;
	};
}
