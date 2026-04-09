#include <iostream>
#include "Simulator.h"

int main()
{
    Simulator sim; // Create Simulator Instance

    // Generate 10 temperature readings
    std::vector<SensorReading> temps = sim.generateTemperatureStream(10);

    // Print them out
    for (const auto& reading : temps) {
        std::cout << "Temp: " << reading.value
                  << " Degrees Celsius at t=" << reading.timestamp << " ms\n";
    }
    
    return 0;
}
