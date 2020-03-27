#pragma once

#include <filesystem>
#include <vector>

#define PATH_LEN 256

namespace fs = std::filesystem;

std::vector<fs::path> getFiles(const char* Dir);