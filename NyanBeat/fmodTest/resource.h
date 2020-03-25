#pragma once

#include <filesystem>
#include <vector>

#define PATH_LEN 256

using namespace std;
namespace fs = filesystem;

vector<fs::path> getFiles(const char* Dir);