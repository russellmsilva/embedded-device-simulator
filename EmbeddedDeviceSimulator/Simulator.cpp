#include "Simulator.h"
#include <random>
#include <chrono>

Simulator::Simulator()
    : tempDrift_(0.0f),
      pressureDrift_(0.0f),
      motionBias_ (0.0f)
{}

// Internal Helpers

float Simulator::randomFloat(float min, float max) {
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

uint64_t Simulator::now() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()
    ).count();
}

// Single Reading Generators

SensorReading Simulator::generateTemperature() {
    float base = 22.0f;                      // baseline degrees Celsius
    float noise = randomFloat(-0.5f, 0.5f);  // small jitter
    tempDrift_ += randomFloat(-0.01f, 0.01f); 
    
    return {
        SensorType::Temperature,
        base + noise + tempDrift_,
        now()
    };
}

SensorReading Simulator::generatePressure() {
    float base = 101.3f;                     // baseline kPa
    float noise = randomFloat(-0.2f, 0.2f);
    pressureDrift_ += randomFloat(-0.005f, 0.005f);
    
    return {
        SensorType::Pressure,
        base + noise + pressureDrift_,
        now()
    };
}

SensorReading Simulator::generateMotion() {
    float base = 0.0f;                        // baseline m/s^2
    float noise = randomFloat(-0.1f, 0.1f);
    motionBias_ += randomFloat(-0.02f, 0.02f);

    return {
        SensorType::Motion,
        base + noise + motionBias_,
        now()
    };
}

// Stream generators

std::vector<SensorReading> Simulator::generateTemperatureStream(std::size_t count) {
    std::vector<SensorReading> out;
    out.reserve(count);
    for (std::size_t i = 0; i < count; ++i)
        out.push_back(generateTemperature());
    return out;
}

std::vector<SensorReading> Simulator::generatePressureStream(std::size_t count) {
    std::vector<SensorReading> out;
    out.reserve(count);
    for (std::size_t i = 0; i < count; ++i)
        out.push_back(generatePressure());
    return out;
}

std::vector<SensorReading> Simulator::generateMotionStream(std::size_t count) {
    std::vector<SensorReading> out;
    out.reserve(count);
    for (std::size_t i = 0; i < count; ++i)
        out.push_back(generateMotion());
    return out;
}
