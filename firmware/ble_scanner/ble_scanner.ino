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

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (!advertisedDevice.haveServiceUUID()) {
      return;
    }

    String uuid = advertisedDevice.getServiceUUID().toString().c_str();

    if (uuid == targetUUID) {
      int rssi = advertisedDevice.getRSSI();
      String mac = advertisedDevice.getAddress().toString().c_str();

      // 只保留本轮扫描中信号最强的目标
      if (!targetSeen || rssi > bestRSSI) {
        bestRSSI = rssi;
        bestMAC = mac;
        targetSeen = true;
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  delay(3000);

  Serial.println("===== SmartCart Strongest RSSI Scanner Start =====");

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
    Serial.print("Best target | MAC: ");
    Serial.print(bestMAC);
    Serial.print(" | RSSI: ");
    Serial.println(bestRSSI);
  } else {
    Serial.println("Target not found");
  }

  delay(500);
}