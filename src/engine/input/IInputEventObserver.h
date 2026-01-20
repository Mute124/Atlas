#pragma once
#include <vector>

#include "InputAction.h"

namespace Atlas {
	struct InputPollResult {
		std::vector<ButtonEvent> buttonEvents;
		std::vector<AxisEvent> axisEvents;
		
		const size_t actionCount = 0;
	};

	class IInputSubject {
	public:
		virtual bool userHasInputSource(EInputSource inputSource) const noexcept = 0;

		virtual void registerInputEvent(InputEvent* inputEvent) = 0;

		virtual EInputSource getInputSource() const = 0;
	};

	class AInputSubject : public IInputSubject {
	private:
		EInputSource mInputSource;
	protected:
		void setInputSource(EInputSource inputSource) noexcept { 
			mInputSource = inputSource;
		}

		std::vector<InputEvent*> mRegisteredInputEvents;
		size_t mRegisteredInputEventCount = 0;
	public:
		
		virtual bool userHasInputSource(EInputSource inputSource) const noexcept = 0;

		virtual void registerInputEvent(InputEvent* inputEvent) override {
			mRegisteredInputEvents.push_back(inputEvent);
			mRegisteredInputEventCount++;
		}

		EInputSource getInputSource() const final { 
			return mInputSource;
		}
	};

	class KeyboardInputSubject : public AInputSubject {
	public:
		virtual bool userHasInputSource(EInputSource inputSource) const noexcept override {
			return inputSource == EInputSource::Keyboard;
		}
	};

	class SDLKeyboardInputSubject : public KeyboardInputSubject {
	public:
	};
}
