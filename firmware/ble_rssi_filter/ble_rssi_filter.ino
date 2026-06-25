#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;
int scanTime = 3;

String targetUUID = "0000fdaa-0000-1000-8000-00805f9b34fb";

int bestRSSI = -999;
String bestMAC = "";
bool targetSeen = false;

// RSSI filter variables
float filteredRSSI = 0;
bool firstReading = true;
float alpha = 0.2;  // 0.1更平滑，0.3反应更快

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (!advertisedDevice.haveServiceUUID()) {
      return;
    }

    String uuid = advertisedDevice.getServiceUUID().toString().c_str();

    if (uuid == targetUUID) {
      int rssi = advertisedDevice.getRSSI();
      String mac = advertisedDevice.getAddress().toString().c_str();

      // Keep the strongest target in this scan round
      if (!targetSeen || rssi > bestRSSI) {
        bestRSSI = rssi;
        bestMAC = mac;
        targetSeen = true;
      }
    }
  }
};

String getDistanceZone(float rssi) {
  if (rssi > -45) {  // -45差不多40-50cm
    return "TOO CLOSE";
  } else if (rssi < -66) { // -66差不多1m
    return "TOO FAR";
  } else {
    return "GOOD DISTANCE";  //50cm-100cm
  }
}
void setup() {
  Serial.begin(115200);
  delay(3000);

  Serial.println("===== SmartCart RSSI Filter Scanner Start =====");

  BLEDevice::init("");

  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  bestRSSI = -999;
  bestMAC = "";
  targetSeen = false;

  pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();

  if (targetSeen) {
    if (firstReading) {
      filteredRSSI = bestRSSI;
      firstReading = false;
    } else {
      filteredRSSI = alpha * bestRSSI + (1 - alpha) * filteredRSSI;
    }

    Serial.print("Best target | MAC: ");
    Serial.print(bestMAC);

    Serial.print(" | Raw RSSI: ");
    Serial.print(bestRSSI);

    Serial.print(" | Filtered RSSI: ");
    Serial.print(filteredRSSI);

    Serial.print(" | Zone: ");
    Serial.println(getDistanceZone(filteredRSSI));
  } else {
    Serial.println("Target not found");
  }

  delay(500);
}