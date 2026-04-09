# Embedded Device Simulator

## Overview

The Embedded Device Simulator is a C++-based tool designed to simulate environmental sensor data for embedded systems testing and development. It provides synthetic data streams for common sensor types such as temperature, pressure, and motion.

This project is intended to support:

- Firmware development without physical hardware
- Algorithm testing under controlled conditions
- Prototyping and validation of embedded systems

---

## Features

- Simulates multiple sensor types:
  - Temperature (°C)
  - Pressure (kPa)
  - Motion (m/s²)

- Generates realistic data using:
  - Random noise
  - Gradual drift over time

- Provides both:
  - Single sensor readings
  - Batched data streams

- Lightweight and dependency-free (standard C++ only)

---

## Project Structure

```
.
├── Simulator.h    # Class definition and data structures
└── Simulator.cpp  # Implementation of simulation logic
```

---

## Data Model

### `SensorType`

Enumeration representing supported sensor types:

```cpp
enum class SensorType : uint8_t {
    Temperature,
    Pressure,
    Motion
};
```

### `SensorReading`

Represents a single sensor data point:

```cpp
struct SensorReading {
    SensorType type;
    float value;
    uint64_t timestamp;
};
```

- `type` → Sensor category
- `value` → Simulated measurement
- `timestamp` → Time since program start (milliseconds)

---

## Usage

### 1. Include the Simulator

```cpp
#include "Simulator.h"
```

---

### 2. Create an Instance

```cpp
Simulator sim;
```

---

### 3. Generate Single Readings

```cpp
auto temp = sim.generateTemperature();
auto pressure = sim.generatePressure();
auto motion = sim.generateMotion();
```

---

### 4. Generate Data Streams

```cpp
auto tempStream = sim.generateTemperatureStream(100);
auto pressureStream = sim.generatePressureStream(100);
auto motionStream = sim.generateMotionStream(100);
```

Each function returns a `std::vector<SensorReading>`.

---

## Build Instructions

#### Visual Studio 2022 (Recommended)

This project was developed using **Microsoft Visual Studio 2022**.  
You can open and build the project directly using the provided `.sln` (solution) file:

1. Open Visual Studio 2022
2. Select **File → Open → Project/Solution**
3. Choose the `.sln` file in the project directory
4. Build and run using the standard Visual Studio workflow

While Visual Studio is the primary development environment, you can try building the project using other tools such as **CMake** or **g++**.

---

## Thread Safety

The current implementation is **not thread-safe**.

- Internal state (e.g., drift variables) is shared and mutable
- Concurrent access from multiple threads may lead to undefined behavior

If multithreading is required, external synchronization must be applied.

---

## Example Usage

```cpp
#include <iostream>
#include "Simulator.h"

int main() {
    Simulator sim;

    auto readings = sim.generateTemperatureStream(5);

    for (const auto& r : readings) {
        std::cout << "Value: " << r.value
                  << " Timestamp: " << r.timestamp << "\n";
    }

    return 0;
}
```
