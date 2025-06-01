#include "iostream"
#include "iomanip"
#include "dataAnalyze.h"
#include "sqlite3.h"
#include "create.h"
#include <map>

using namespace std;

int avgSpeedCallback(void* data, int argc, char** argv, char** azColName) {
    DataAnalysisManager::AvgSpeedData* avgData = static_cast<DataAnalysisManager::AvgSpeedData*>(data);
    if (argc >= 2 && argv[0] && argv[1]) {
        try {
            int vehicleId = std::stoi(argv[0]);
            double speed = std::stod(argv[1]);
            avgData->vehicleTotalSpeed[vehicleId] += speed;
            avgData->vehicleRecordCount[vehicleId]++;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing avg speed data: " << e.what() << std::endl;
        }
    }
    return 0;
}

void DataAnalysisManager::analyzeVehiclePerformance() {
    std::cout << "\n--- Vehicle Performance Analysis ---" << std::endl;
    int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
    if (rc) {
        std::cerr << "Failed to open database for vehicle performance analysis." << std::endl;
        return;
    }

    // --- Average Vehicle Speed ---
    std::cout << "\nAverage Vehicle Speed (per Vehicle ID):" << std::endl;
    std::string sqlAvgSpeed = "SELECT VehicleID, VehicleSpeed FROM RealTimeData;";
    AvgSpeedData avgSpeedData;
    char* errMsg = nullptr;
    rc = sqlite3_exec(database, sqlAvgSpeed.c_str(), avgSpeedCallback, &avgSpeedData, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (Avg Speed): " << (errMsg ? errMsg : "Unknown error") << std::endl;
        sqlite3_free(errMsg); // Free the error message memory
    } else {
        if (avgSpeedData.vehicleTotalSpeed.empty()) {
            std::cout << "  No real-time data available for average speed analysis." << std::endl;
        } else {
            std::cout << std::fixed << std::setprecision(2); // Format output
            for (const auto& pair : avgSpeedData.vehicleTotalSpeed) {
                int vehicleId = pair.first;
                double totalSpeed = pair.second;
                int count = avgSpeedData.vehicleRecordCount[vehicleId];
                if (count > 0) {
                    std::cout << "  Vehicle ID " << vehicleId << ": " << (totalSpeed / count) << " units/s" << std::endl;
                } else {
                    std::cout << "  Vehicle ID " << vehicleId << ": No speed records." << std::endl;
                }
            }
        }
    }

    // --- Average Fuel Level ---
    // Using a direct SQL query with GROUP BY and AVG for simplicity here,
    // and the generalCallback to print results directly.
    std::cout << "\nAverage Fuel Level (per Vehicle ID):" << std::endl;
    std::string sqlAvgFuel = "SELECT VehicleID, AVG(FuelLevel) AS AverageFuelLevel FROM RealTimeData GROUP BY VehicleID;";
    rc = sqlite3_exec(database, sqlAvgFuel.c_str(), callback, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (Avg Fuel Level): " << (errMsg ? errMsg : "Unknown error") << std::endl;
        sqlite3_free(errMsg);
    }

    std::cout << "--- End Vehicle Performance Analysis ---" << std::endl;
}
