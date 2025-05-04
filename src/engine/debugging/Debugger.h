/**************************************************************************************************
 * @file Debugger.h
 * 
 * @brief Debugging tools.
 * 
 * @date May 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include <string>
#include <memory>
#include <cstdint>
#include <any>
#include <vector>
#include <source_location>

#include "../core/Core.h"

#include "Logging.h"


namespace Atlas {
	class IDebugEvent {
	public:

		virtual uint32_t getEventType() = 0;
		virtual std::string getEventMessageString() = 0;
		virtual std::string getPrettyEventMessageString() = 0;


		virtual bool isFatal() = 0;
		virtual bool isError() = 0;
		virtual bool isException() = 0;
	};

	class ADebugEvent : public IDebugEvent {
	private:

		const uint32_t mEventType;
		const std::string mEventMessageString;


	public:

		ADebugEvent(uint32_t eventType, std::string const& eventMessageString);

		uint32_t getEventType() override;
		std::string getEventMessageString() override;
		std::string getPrettyEventMessageString() override;

		virtual bool isFatal() = 0;

	};

	class IEventRecorder {
	public:
		virtual void pushEvent(IDebugEvent* eventOccurred) = 0;

		virtual std::vector<IDebugEvent*> getRecordedEvents() = 0;


	};

	class IDebugger {
	public:


		virtual void init() = 0;

		virtual bool enableDebugging() = 0;
		virtual bool disableDebugging() = 0;

		virtual bool startRecording() = 0;
		virtual bool stopRecording() = 0;

		virtual void recordEvent(IDebugEvent* eventOccurred) = 0;
		
		virtual void onFatalEvent(IDebugEvent* fatalDebugEvent) = 0;

		virtual IEventRecorder* getEventRecorder() = 0;
		virtual void setEventRecorder(IEventRecorder* eventRecorder) = 0;
	};

	class ADebugger : public IDebugger {
	private:
		IEventRecorder* mEventRecorder;
	public:

		ADebugger(IEventRecorder* eventRecorder);



		IEventRecorder* getEventRecorder() final override;
		void setEventRecorder(IEventRecorder* eventRecorder) final override;
	};
}
