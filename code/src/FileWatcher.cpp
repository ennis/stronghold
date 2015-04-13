#include "FileWatcher.hpp"

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <sys/stat.h>
#include <time.h>

static time_t get_last_modif_time(const char *filename) {
	struct stat attrib;
	time_t clock;

	stat(filename, &attrib);
	clock = mktime(localtime(&(attrib.st_mtime)));

	return clock;
}

bool FileWatcher::fileChanged()
{
	time_t cur = get_last_modif_time(path.c_str());
	bool changed = false;

	if (difftime(cur, old) > 0) {
		changed = true;
	}

	old = cur;

	return changed;
}