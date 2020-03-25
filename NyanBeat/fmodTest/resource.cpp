#include "resource.h"

vector<fs::path> getFiles(const char* dir) {
	vector<fs::path> fileList{};

	for (const fs::directory_entry& entry : fs::directory_iterator(dir)) {
		fileList.emplace_back(entry.path());
	}

	return fileList;
}