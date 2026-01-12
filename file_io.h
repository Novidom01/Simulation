#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>
#include "physics_models.h"

bool saveToFile(const std::string& filename, const std::vector<SimulationResult>& results);
bool loadFromFile(const std::string& filename, std::vector<SimulationResult>& results);

#endif