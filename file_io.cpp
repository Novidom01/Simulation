#include "file_io.h"
#include <fstream>
#include <iostream>
#include <iomanip>

bool saveToFile(const std::string& filename, const std::vector<SimulationResult>& results) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл для записи.\n";
        return false;
    }

    // Записываем заголовок
    file << "Time(s),Position(m),Velocity(m/s),Acceleration(m/s^2)\n";

    // Записываем данные с фиксированной точностью
    file << std::fixed << std::setprecision(6);
    for (const auto& res : results) {
        file << res.time << ","
             << res.position << ","
             << res.velocity << ","
             << res.other_param << "\n";
    }

    file.close();
    std::cout << "Данные сохранены в файл: " << filename << "\n";
    return true;
}

bool loadFromFile(const std::string& filename, std::vector<SimulationResult>& results) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл для чтения.\n";
        return false;
    }

    results.clear();
    std::string line;

    // Пропускаем заголовок
    std::getline(file, line);

    SimulationResult res;
    char comma;

    while (file >> res.time >> comma >> res.position >> comma
                >> res.velocity >> comma >> res.other_param) {
        results.push_back(res);
                }

    file.close();
    std::cout << "Данные загружены из файла: " << filename
              << " (" << results.size() << " записей)\n";
    return !results.empty();
}

