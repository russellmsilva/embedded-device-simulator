#include "Simulator.h"
#include <random>

// Temperature sensor constants

static constexpr float TEMP_BASE_VALUE = 22.0f;   // degrees Celsius
static constexpr float TEMP_NOISE_MIN = -0.5f;
static constexpr float TEMP_NOISE_MAX = 0.5f;
static constexpr float TEMP_DRIFT_STEP = 0.01f;   // Temperature drift: slow, stable
static constexpr float TEMP_DRIFT_DECAY = 0.995f;

// Pressure sensor constants

static constexpr float PRESSURE_BASE_VALUE = 101.3f;  // kPa
static constexpr float PRESSURE_NOISE_MIN = -0.2f;
static constexpr float PRESSURE_NOISE_MAX = 0.2f;
static constexpr float PRESSURE_DRIFT_STEP = 0.005f;  // Pressure drift: even slower
static constexpr float PRESSURE_DRIFT_DECAY = 0.998f;

// Motion sensor constants

static constexpr float MOTION_BASE_VALUE = 0.0f;    // m/s^2
static constexpr float MOTION_NOISE_MIN = -0.1f;
static constexpr float MOTION_NOISE_MAX = 0.1f;
static constexpr float MOTION_DRIFT_STEP = 0.02f;   // Motion drift: fastest (IMU bias instability)
static constexpr float MOTION_DRIFT_DECAY = 0.990f;

// Constructor

Simulator::Simulator()
    : tempDrift_(0.0f),
      pressureDrift_(0.0f),
      motionDrift_ (0.0f),
      startTime_(std::chrono::steady_clock::now())
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
        steady_clock::now() - startTime_
    ).count();
}

// Single Reading Generators

SensorReading Simulator::generateTemperature() {
    float noise = randomFloat(TEMP_NOISE_MIN, TEMP_NOISE_MAX);     // small jitter
    tempDrift_ += randomFloat(-TEMP_DRIFT_STEP, TEMP_DRIFT_STEP);  // random walk component
    tempDrift_ *= TEMP_DRIFT_DECAY;                                // decay factor to avoid infinite growth of drift
    
    return {
        SensorType::Temperature,
        TEMP_BASE_VALUE + noise + tempDrift_,
        now()
    };
}

SensorReading Simulator::generatePressure() {
    float noise = randomFloat(PRESSURE_NOISE_MIN, PRESSURE_NOISE_MAX);         // small jitter
    pressureDrift_ += randomFloat(-PRESSURE_DRIFT_STEP, PRESSURE_DRIFT_STEP);  // random walk component
    pressureDrift_ *= PRESSURE_DRIFT_DECAY;                                    // decay factor to avoid infinite growth of drift
    
    return {
        SensorType::Pressure,
        PRESSURE_BASE_VALUE + noise + pressureDrift_,
        now()
    };
}

SensorReading Simulator::generateMotion() {
    float noise = randomFloat(MOTION_NOISE_MIN, MOTION_NOISE_MAX);       // small jitter
    motionDrift_ += randomFloat(-MOTION_DRIFT_STEP, MOTION_DRIFT_STEP);  // random walk component
    motionDrift_ *= MOTION_DRIFT_DECAY;                                  // decay factor to avoid infinite growth of drift

    return {
        SensorType::Motion,
        MOTION_BASE_VALUE + noise + motionDrift_,
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
