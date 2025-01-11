#include "RegisteredFile.h"

inline Atlas::RegisteredFile::RegisteredFile() {}

inline Atlas::RegisteredFile::RegisteredFile(std::shared_ptr<FileMeta> const& meta) : meta(meta) {}
