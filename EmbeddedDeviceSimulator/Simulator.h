#pragma once

#include <cstdint>
#include <vector>
#include <chrono>

enum class SensorType : uint8_t {
    Temperature,
    Pressure,
    Motion
};

struct SensorReading {
    SensorType type;
    float value;          // e.g., degrees Celsius, kPa, m/s^2 depending on sensor
    uint64_t timestamp;   // milliseconds since the Simulator instance was created
};

class Simulator {
public:
    Simulator();

    // Generate a single reading
    SensorReading generateTemperature();
    SensorReading generatePressure();
    SensorReading generateMotion();

    // Generate a stream of readings
    std::vector<SensorReading> generateTemperatureStream(std::size_t count);
    std::vector<SensorReading> generatePressureStream(std::size_t count);
    std::vector<SensorReading> generateMotionStream(std::size_t count);

private:
    // Internal helpers for noise, drift, etc.
    float randomFloat(float min, float max);
    uint64_t now();

    // Interal state for drift simulation
    float tempDrift_;
    float pressureDrift_;
    float motionDrift_;

    // Internal timestamp representing time of Simulator instance creation
    std::chrono::steady_clock::time_point startTime_;
};