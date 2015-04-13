#ifndef FILEWATCHER_HPP
#define FILEWATCHER_HPP

#include <iostream>

class FileWatcher
{
public:
    FileWatcher ()
    {}

	FileWatcher(const char *path_) : path(path_)
	{}

	bool fileChanged();

	std::string path;

private:
	time_t old;
};

#endif
