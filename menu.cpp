#include "menu.h"
#include "physics_models.h"
#include "file_io.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>

// Глобальные переменные для текущей модели и состояния
std::unique_ptr<PhysicsModel> currentModel;
bool hasModel = false;
bool hasResults = false;

// Функция для вывода главного меню
void displayMainMenu() {
    std::cout << "\n=== ФИЗИЧЕСКИЙ СИМУЛЯТОР ===\n";
    std::cout << "Текущая модель: ";
    if (hasModel) {
        std::cout << currentModel->getName();
    } else {
        std::cout << "не выбрана";
    }
    std::cout << "\n\n";

    std::cout << "1. Выбрать физическую модель\n";
    std::cout << "2. Ввести параметры модели\n";
    std::cout << "3. Запустить симуляцию\n";
    std::cout << "4. Вывести таблицу результатов\n";
    std::cout << "5. Сохранить результаты в файл\n";
    std::cout << "6. Загрузить результаты из файла\n";
    std::cout << "7. Очистить результаты\n";
    std::cout << "8. Выход\n";
    std::cout << "Выберите пункт меню: ";
}

// Функция для выбора модели
void selectModel() {
    std::cout << "\n=== ВЫБОР ФИЗИЧЕСКОЙ МОДЕЛИ ===\n";
    std::cout << "1. Падение тела с сопротивлением воздуха\n";
    std::cout << "2. Модель пружины (колебания)\n";
    std::cout << "3. Движение с постоянным ускорением\n";
    std::cout << "4. Движение заряда в поле\n";
    std::cout << "0. Назад\n";
    std::cout << "Выберите модель: ";

    int choice;
    std::cin >> choice;

    if (choice >= 1 && choice <= 4) {
        currentModel.reset(createModel(choice));
        hasModel = true;
        hasResults = false;
        std::cout << "Модель выбрана: " << currentModel->getName() << "\n";
    } else if (choice != 0) {
        std::cout << "Неверный выбор!\n";
    }
}

// Функция для ввода параметров
void inputParameters() {
    if (!hasModel) {
        std::cout << "Сначала выберите модель!\n";
        return;
    }

    std::cout << "\n=== ВВОД ПАРАМЕТРОВ ===\n";
    auto paramNames = currentModel->getParamNames();
    std::vector<double> params;

    for (size_t i = 0; i < paramNames.size(); ++i) {
        double value;
        std::cout << paramNames[i] << ": ";
        std::cin >> value;

        // Проверка корректности ввода
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите число: ";
            std::cin >> value;
        }

        params.push_back(value);
    }

    currentModel->setParameters(params);
    std::cout << "Параметры установлены.\n";
}

// Функция для запуска симуляции
void runSimulation() {
    if (!hasModel) {
        std::cout << "Сначала выберите модель!\n";
        return;
    }

    std::cout << "\n=== ЗАПУСК СИМУЛЯЦИИ ===\n";

    double timeStep, totalTime;

    std::cout << "Шаг времени (с): ";
    std::cin >> timeStep;
    while (timeStep <= 0 || std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Шаг должен быть положительным числом: ";
        std::cin >> timeStep;
    }

    std::cout << "Общее время симуляции (с): ";
    std::cin >> totalTime;
    while (totalTime <= 0 || std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Время должно быть положительным числом: ";
        std::cin >> totalTime;
    }

    currentModel->simulate(timeStep, totalTime);
    hasResults = true;

    auto results = currentModel->getResults();
    std::cout << "Симуляция завершена. Получено " << results.size() << " точек.\n";
}

// Функция для вывода таблицы
void displayResults() {
    if (!hasResults) {
        std::cout << "Сначала выполните симуляцию!\n";
        return;
    }

    auto results = currentModel->getResults();

    std::cout << "\n=== РЕЗУЛЬТАТЫ СИМУЛЯЦИИ ===\n";
    std::cout << std::left << std::setw(12) << "Время (с)"
              << std::setw(15) << "Положение (м)"
              << std::setw(15) << "Скорость (м/с)"
              << std::setw(15) << "Ускорение (м/с²)" << "\n";

    std::cout << std::string(57, '-') << "\n";

    for (const auto& res : results) {
        std::cout << std::left << std::setw(12) << std::fixed << std::setprecision(4) << res.time
                  << std::setw(15) << res.position
                  << std::setw(15) << res.velocity
                  << std::setw(15) << res.other_param << "\n";
    }
}

// Главная функция меню
void runMainMenu() {
    int choice;

    do {
        displayMainMenu();
        std::cin >> choice;

        // Очистка буфера ввода
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                selectModel();
                break;

            case 2:
                inputParameters();
                break;

            case 3:
                runSimulation();
                break;

            case 4:
                displayResults();
                break;

            case 5:
                if (hasResults) {
                    std::string filename;
                    std::cout << "Введите имя файла для сохранения: ";
                    std::getline(std::cin, filename);
                    if (filename.empty()) filename = "simulation_results.csv";

                    auto results = currentModel->getResults();
                    saveToFile(filename, results);
                } else {
                    std::cout << "Нет данных для сохранения!\n";
                }
                break;

            case 6:
                {
                    std::string filename;
                    std::cout << "Введите имя файла для загрузки: ";
                    std::getline(std::cin, filename);

                    std::vector<SimulationResult> loadedResults;
                    if (loadFromFile(filename, loadedResults)) {
                        // Создаем временную модель для хранения загруженных данных
                        currentModel.reset(new ConstantAcceleration());
                        hasModel = true;
                        hasResults = true;
                        // Для простоты используем первую модель
                        static_cast<ConstantAcceleration*>(currentModel.get())->simulate(0.1, 1.0);
                    }
                }
                break;

            case 7:
                if (hasModel) {
                    currentModel->clearResults();
                    hasResults = false;
                    std::cout << "Результаты очищены.\n";
                }
                break;

            case 8:
                std::cout << "Выход из программы.\n";
                break;

            default:
                std::cout << "Неверный выбор! Попробуйте снова.\n";
                break;
        }

        // Пауза для удобства чтения
        if (choice != 8) {
            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.get();
        }

    } while (choice != 8);
}