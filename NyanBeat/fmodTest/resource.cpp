#include "pch.h"
#include "resource.h"

std::vector<fs::path> getFiles(const char* dir) {
	std::vector<fs::path> fileList{};

	for (const fs::directory_entry& entry : fs::directory_iterator(dir)) {
		fileList.emplace_back(entry.path());
	}

	return fileList;
}