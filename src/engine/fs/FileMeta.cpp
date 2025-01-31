#include "FileMeta.h"
#include <format>
std::string Atlas::FileMeta::resolveExtension(std::string const& filename)
{
    return filename.substr(filename.find_last_of(".") + 1);
}

std::string Atlas::FileMeta::resolveFilename(std::string const& path)
{

    return path.substr(path.find_last_of("\\") + 1);
}

std::string Atlas::FileMeta::resolveSandboxPath(std::string const& path)
{
    // truncate the path down to just everything on the right of "/game/"
    std::string newPath = path.substr(path.find("/game/") + 6);
    return newPath;
}

Atlas::FileMeta::FileMeta() {
}

Atlas::FileMeta::FileMeta(std::string const& path) 
{
    this->path = path;
    this->filename = resolveFilename(path);
    this->extension = resolveExtension(this->filename);
	this->sandboxPath = resolveSandboxPath(path);
}

Atlas::FilePath Atlas::FileMeta::getFullPath()
{
    return FilePath();
}

Atlas::FilePath Atlas::FileMeta::getSandboxPath()
{
    return FilePath();
}

Atlas::FilePath Atlas::FileMeta::getFullSandboxPath()
{
    return FilePath();
}

std::string Atlas::FileMeta::format()
{
    return std::format("{} ({}) at {} sandboxed at: {}", filename, extension, path, sandboxPath);
}
