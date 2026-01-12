#ifndef PHYSICS_MODELS_H
#define PHYSICS_MODELS_H

#include <vector>
#include <string>

// Общая структура для результатов симуляции
struct SimulationResult {
    double time;
    double position;
    double velocity;
    double other_param; // Дополнительный параметр для разных моделей
};

// Базовый класс для физических моделей
class PhysicsModel {
public:
    virtual ~PhysicsModel() = default;
    virtual void setParameters(const std::vector<double>& params) = 0;
    virtual void simulate(double timeStep, double totalTime) = 0;
    virtual const std::vector<SimulationResult>& getResults() const = 0;
    virtual void clearResults() = 0;
    virtual std::string getName() const = 0;
    virtual std::vector<std::string> getParamNames() const = 0;
};

// 1. Падение тела с сопротивлением воздуха
class FallingBodyWithDrag : public PhysicsModel {
public:
    FallingBodyWithDrag();
    void setParameters(const std::vector<double>& params) override;
    void simulate(double timeStep, double totalTime) override;
    const std::vector<SimulationResult>& getResults() const override;
    void clearResults() override;
    std::string getName() const override;
    std::vector<std::string> getParamNames() const override;

private:
    double mass;        // масса
    double gravity;     // ускорение свободного падения
    double dragCoeff;   // коэффициент сопротивления
    double height;      // начальная высота
    std::vector<SimulationResult> results;
};

// 2. Модель пружины (колебания)
class SpringOscillator : public PhysicsModel {
public:
    SpringOscillator();
    void setParameters(const std::vector<double>& params) override;
    void simulate(double timeStep, double totalTime) override;
    const std::vector<SimulationResult>& getResults() const override;
    void clearResults() override;
    std::string getName() const override;
    std::vector<std::string> getParamNames() const override;

private:
    double mass;        // масса
    double stiffness;   // жесткость пружины
    double damping;     // коэффициент затухания
    double amplitude;   // начальная амплитуда
    std::vector<SimulationResult> results;
};

// 3. Движение с постоянным ускорением
class ConstantAcceleration : public PhysicsModel {
public:
    ConstantAcceleration();
    void setParameters(const std::vector<double>& params) override;
    void simulate(double timeStep, double totalTime) override;
    const std::vector<SimulationResult>& getResults() const override;
    void clearResults() override;
    std::string getName() const override;
    std::vector<std::string> getParamNames() const override;

private:
    double initialPos;  // начальное положение
    double initialVel;  // начальная скорость
    double acceleration;// ускорение
    std::vector<SimulationResult> results;
};

// 4. Движение заряда в поле
class ChargeInField : public PhysicsModel {
public:
    ChargeInField();
    void setParameters(const std::vector<double>& params) override;
    void simulate(double timeStep, double totalTime) override;
    const std::vector<SimulationResult>& getResults() const override;
    void clearResults() override;
    std::string getName() const override;
    std::vector<std::string> getParamNames() const override;

private:
    double charge;      // заряд
    double mass;        // масса
    double eField;      // напряженность поля
    double initialVel;  // начальная скорость
    std::vector<SimulationResult> results;
};

// Фабрика для создания моделей
PhysicsModel* createModel(int modelType);

#endif // PHYSICS_MODELS_H