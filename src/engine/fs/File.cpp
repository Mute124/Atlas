#include "File.h"
#include "FileMeta.h"
#include <memory>

bool Atlas::File::isLoaded() const { return mIsLoaded; }

void Atlas::File::setLoadState(bool state)
{
	mIsLoaded = state;
}

std::shared_ptr<Atlas::FileMeta> Atlas::File::getFileMeta() const
{
	return mFileMeta;
}
