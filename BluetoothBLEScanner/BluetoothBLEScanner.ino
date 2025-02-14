#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 15; // Scan for 15 seconds

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        String address = advertisedDevice.getAddress().toString().c_str();

        Serial.print("Device Address: ");
        Serial.println(address);

        // Check if the device has a name and print it if available
        if (advertisedDevice.haveName()) {
            Serial.print("Device Name: ");
            Serial.println(advertisedDevice.getName().c_str());
        } else {
            Serial.println("Device Name: Unknown");
        }

        Serial.println("-----------------------");
    }
};

void setup() {
    Serial.begin(115200);

    BLEDevice::init("ESP32_BLE_Scanner");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);    // Active scan to retrieve additional info
    pBLEScan->setInterval(100);       // Set scan interval
    pBLEScan->setWindow(99);          // Set scan window, must be <= interval
}

void loop() {
    Serial.println("Starting BLE scan...");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->start(scanTime, false); // Run scan for 15 seconds
    pBLEScan->clearResults();         // Clear results before starting a new scan
}
