#include "file.h"

namespace flounder {
	const std::string file::FILE_SEPARATOR = "/";
	file *const file::RES_FOLDER = new file((std::string) "res");

	file::file(std::string path)
	{
		this->path = FILE_SEPARATOR + path;
		std::vector<std::string> dirs = helperstring::split(path, FILE_SEPARATOR);
		name = dirs[dirs.size() - 1];
	}

	file::file(std::vector<std::string> paths)
	{
		std::string path = "";

		for (int i = 0; i < paths.size(); i++) {
			path += FILE_SEPARATOR + paths.at(i);
		}

		this->path = path;
		std::vector<std::string> dirs = helperstring::split(path, FILE_SEPARATOR);
		name = dirs[dirs.size() - 1];
	}

	file::file(file *file, std::string subFile)
	{
		this->path = file->path + FILE_SEPARATOR + subFile;
		this->name = subFile;
	}

	file::file(file *file, std::vector<std::string> subFiles)
	{
		std::string path = "" + file->path;

		for (int i = 0; i < subFiles.size(); i++) {
			path += FILE_SEPARATOR;
			path += subFiles.at(i);
		}

		this->path = path;
		std::vector<std::string> dirs = helperstring::split(path, FILE_SEPARATOR);
		name = dirs[dirs.size() - 1];
	}

	file::~file()
	{
		delete &path;
		delete &name;
	}

	std::string file::getName()
	{
		return name;
	}

	std::string file::getPath()
	{
		return path;
	}
}
