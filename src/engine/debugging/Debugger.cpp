/**************************************************************************************************
 * @file Debugger.cpp
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v
 ***************************************************************************************************/

#include <string>
#include <cstdint>

#include "Debugger.h"

//Atlas::ADebugEvent::ADebugEvent(uint32_t eventType, std::string const& eventMessageString) 
//	: mEventType(eventType), mEventMessageString(eventMessageString) {
//}
//
//uint32_t Atlas::ADebugEvent::getEventType()
//{
//	return mEventType;
//}
//
//std::string Atlas::ADebugEvent::getEventMessageString()
//{
//	return mEventMessageString;
//}
//
//std::string Atlas::ADebugEvent::getPrettyEventMessageString()
//{
//	return std::string();
//}
//
//Atlas::ADebugger::ADebugger(IEventRecorder* eventRecorder) : mEventRecorder(eventRecorder) {}
//
//Atlas::IEventRecorder* Atlas::ADebugger::getEventRecorder()
//{
//	return nullptr;
//}
//
//void Atlas::ADebugger::setEventRecorder(IEventRecorder* eventRecorder)
//{
//}

Atlas::DebugEvent::DebugEvent(
	EDebugEventType eventType,
	std::string const& eventMessageString,
	HighResolutionTimepoint eventTimePoint,
	std::source_location eventLocation
) : mEventType(eventType), mEventMessageString(eventMessageString), mEventTimePoint(eventTimePoint), mEventLocation(eventLocation) {
}

bool Atlas::DebugEvent::isValid()
{
	return false;
}

Atlas::EDebugEventType Atlas::DebugEvent::getEventType() const {
	return mEventType;
}

std::string const& Atlas::DebugEvent::getEventMessageString() const {
	return mEventMessageString;
}

Atlas::HighResolutionTimepoint Atlas::DebugEvent::getEventTimePoint() const { 
	return mEventTimePoint; 
}

std::source_location Atlas::DebugEvent::getEventLocation() const {
	return mEventLocation; 
}
