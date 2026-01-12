#include "physics_models.h"
#include <cmath>
#include <iostream>

// ==================== FallingBodyWithDrag ====================
FallingBodyWithDrag::FallingBodyWithDrag()
    : mass(1.0), gravity(9.81), dragCoeff(0.1), height(100.0) {}

void FallingBodyWithDrag::setParameters(const std::vector<double>& params) {
    if (params.size() >= 4) {
        mass = params[0];
        gravity = params[1];
        dragCoeff = params[2];
        height = params[3];
    }
}

void FallingBodyWithDrag::simulate(double timeStep, double totalTime) {
    clearResults();

    double time = 0.0;
    double position = height;
    double velocity = 0.0;

    while (time <= totalTime && position >= 0.0) {
        // Сила сопротивления: F_drag = 0.5 * dragCoeff * v^2
        double dragForce = 0.5 * dragCoeff * velocity * velocity;
        double acceleration = gravity - dragForce / mass;

        velocity += acceleration * timeStep;
        position -= velocity * timeStep; // Положение уменьшается при падении
        time += timeStep;

        SimulationResult res;
        res.time = time;
        res.position = position;
        res.velocity = velocity;
        res.other_param = acceleration;

        results.push_back(res);

        if (position <= 0.0) break; // Тело достигло земли
    }
}

const std::vector<SimulationResult>& FallingBodyWithDrag::getResults() const {
    return results;
}

void FallingBodyWithDrag::clearResults() {
    results.clear();
}

std::string FallingBodyWithDrag::getName() const {
    return "Падение тела с сопротивлением воздуха";
}

std::vector<std::string> FallingBodyWithDrag::getParamNames() const {
    return {"Масса (кг)", "Ускорение своб. падения (м/с²)",
            "Коэф. сопротивления", "Начальная высота (м)"};
}

// ==================== SpringOscillator ====================
SpringOscillator::SpringOscillator()
    : mass(1.0), stiffness(10.0), damping(0.1), amplitude(1.0) {}

void SpringOscillator::setParameters(const std::vector<double>& params) {
    if (params.size() >= 4) {
        mass = params[0];
        stiffness = params[1];
        damping = params[2];
        amplitude = params[3];
    }
}

void SpringOscillator::simulate(double timeStep, double totalTime) {
    clearResults();

    double time = 0.0;
    double position = amplitude;
    double velocity = 0.0;

    while (time <= totalTime) {
        // Уравнение: m*x'' + c*x' + k*x = 0
        double springForce = -stiffness * position;
        double dampingForce = -damping * velocity;
        double acceleration = (springForce + dampingForce) / mass;

        velocity += acceleration * timeStep;
        position += velocity * timeStep;
        time += timeStep;

        SimulationResult res;
        res.time = time;
        res.position = position;
        res.velocity = velocity;
        res.other_param = acceleration;

        results.push_back(res);
    }
}

const std::vector<SimulationResult>& SpringOscillator::getResults() const {
    return results;
}

void SpringOscillator::clearResults() {
    results.clear();
}

std::string SpringOscillator::getName() const {
    return "Модель пружины (колебания)";
}

std::vector<std::string> SpringOscillator::getParamNames() const {
    return {"Масса (кг)", "Жесткость пружины (Н/м)",
            "Коэф. затухания", "Начальная амплитуда (м)"};
}

// ==================== ConstantAcceleration ====================
ConstantAcceleration::ConstantAcceleration()
    : initialPos(0.0), initialVel(0.0), acceleration(1.0) {}

void ConstantAcceleration::setParameters(const std::vector<double>& params) {
    if (params.size() >= 3) {
        initialPos = params[0];
        initialVel = params[1];
        acceleration = params[2];
    }
}

void ConstantAcceleration::simulate(double timeStep, double totalTime) {
    clearResults();

    double time = 0.0;

    while (time <= totalTime) {
        // Формулы равноускоренного движения
        double position = initialPos + initialVel * time + 0.5 * acceleration * time * time;
        double velocity = initialVel + acceleration * time;

        SimulationResult res;
        res.time = time;
        res.position = position;
        res.velocity = velocity;
        res.other_param = acceleration;

        results.push_back(res);

        time += timeStep;
    }
}

const std::vector<SimulationResult>& ConstantAcceleration::getResults() const {
    return results;
}

void ConstantAcceleration::clearResults() {
    results.clear();
}

std::string ConstantAcceleration::getName() const {
    return "Движение с постоянным ускорением";
}

std::vector<std::string> ConstantAcceleration::getParamNames() const {
    return {"Начальное положение (м)", "Начальная скорость (м/с)",
            "Ускорение (м/с²)"};
}

// ==================== ChargeInField ====================
ChargeInField::ChargeInField()
    : charge(1.0), mass(1.0), eField(1.0), initialVel(0.0) {}

void ChargeInField::setParameters(const std::vector<double>& params) {
    if (params.size() >= 4) {
        charge = params[0];
        mass = params[1];
        eField = params[2];
        initialVel = params[3];
    }
}

void ChargeInField::simulate(double timeStep, double totalTime) {
    clearResults();

    double time = 0.0;
    double position = 0.0;
    double velocity = initialVel;

    while (time <= totalTime) {
        // Сила Лоренца в однородном поле: F = qE
        double force = charge * eField;
        double acceleration = force / mass;

        velocity += acceleration * timeStep;
        position += velocity * timeStep;
        time += timeStep;

        SimulationResult res;
        res.time = time;
        res.position = position;
        res.velocity = velocity;
        res.other_param = acceleration;

        results.push_back(res);
    }
}

const std::vector<SimulationResult>& ChargeInField::getResults() const {
    return results;
}

void ChargeInField::clearResults() {
    results.clear();
}

std::string ChargeInField::getName() const {
    return "Движение заряда в поле";
}

std::vector<std::string> ChargeInField::getParamNames() const {
    return {"Заряд (Кл)", "Масса (кг)", "Напряженность поля (Н/Кл)",
            "Начальная скорость (м/с)"};
}

// ==================== Factory Function ====================
PhysicsModel* createModel(int modelType) {
    switch (modelType) {
        case 1: return new FallingBodyWithDrag();
        case 2: return new SpringOscillator();
        case 3: return new ConstantAcceleration();
        case 4: return new ChargeInField();
        default: return nullptr;
    }
}
