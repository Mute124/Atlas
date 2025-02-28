#include "SelfDeterminingInputAction.h"

Atlas::SelfDeterminingInputAction::SelfDeterminingInputAction(std::function<bool(const SelfDeterminingInputAction*)> const& checker, std::function<void(const SelfDeterminingInputAction*)> const& callback) : mChecker(checker), mCallback(callback) {}

bool Atlas::SelfDeterminingInputAction::check() const {
	return mChecker(this);
}

void Atlas::SelfDeterminingInputAction::callback() const {
	mCallback(this);
}
