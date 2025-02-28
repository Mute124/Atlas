/*****************************************************************//**
 * @file   PerformanceTest.h
 * 
 * @brief  
 * 
 * @date   February 2025
 * 
 * @since 
 *********************************************************************/
#pragma once
#include <chrono>


namespace Atlas {


	class IPerformanceTest {
	private:

	public:

		virtual void setup() = 0;

		virtual void beginTimer() = 0;

	};

	class SavablePerformanceTest : public IPerformanceTest {
	private:

	public:
	};
}
