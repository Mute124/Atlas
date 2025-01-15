#include "RegisteredFile.h"

Atlas::RegisteredFile::RegisteredFile() {}

Atlas::RegisteredFile::RegisteredFile(std::shared_ptr<FileMeta> const& meta) : mFileMeta(meta) {}
