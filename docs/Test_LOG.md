# Test Log

## Test 1 - BLE Strongest RSSI Scanner

Date: 2026-06-25

### Goal

Test whether the ESP32-C3 can scan nearby BLE devices, detect the target Service UUID, and select the strongest RSSI target in each scan cycle.

### Test Setup

* Board: ESP32-C3
* BLE identification method: Service UUID
* Target Service UUID: `0000fdaa-0000-1000-8000-00805f9b34fb`
* Scan time: 3 seconds
* Output: Strongest detected target MAC address and RSSI value

### Serial Monitor Sample

```text
===== SmartCart Strongest RSSI Scanner Start =====
Best target | MAC: 71:13:56:1c:56:db | RSSI: -49
Best target | MAC: 71:13:56:1c:56:db | RSSI: -39
Best target | MAC: 71:13:56:1c:56:db | RSSI: -47
Best target | MAC: 71:13:56:1c:56:db | RSSI: -56
Best target | MAC: 71:13:56:1c:56:db | RSSI: -70
Best target | MAC: 71:13:56:1c:56:db | RSSI: -76
```

### Result

The ESP32-C3 successfully detected BLE devices with the target Service UUID.
When multiple devices or advertisements were detected, the program selected the strongest RSSI target during each scan cycle.

### Observation

The raw RSSI value changed quickly between scan cycles. For example, the RSSI changed from around `-39` to `-76` during testing. This showed that raw RSSI alone is not stable enough for direct motor control.

### Next Step

Add RSSI filtering to smooth the signal before using it for distance zone classification.

## Test 2 - RSSI Filtering

Date: 2026-06-25

### Goal

Test whether RSSI values can be smoothed using a simple exponential filter.

### Method

The program first selects the strongest target with the target Service UUID.
Then it applies the following filter:

```cpp
filteredRSSI = alpha * rawRSSI + (1 - alpha) * filteredRSSI;
```

Current filter setting:

```cpp
alpha = 0.2;
```

### Serial Monitor Sample

```text
===== SmartCart RSSI Filter Scanner Start =====
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -49 | Filtered RSSI: -49.00
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -39 | Filtered RSSI: -47.00
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -39 | Filtered RSSI: -45.40
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -47 | Filtered RSSI: -45.72
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -56 | Filtered RSSI: -47.82
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -70 | Filtered RSSI: -53.09
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -76 | Filtered RSSI: -57.67
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -65 | Filtered RSSI: -59.13
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -63 | Filtered RSSI: -59.91
Best target | MAC: 71:13:56:1c:56:db | Raw RSSI: -38 | Filtered RSSI: -55.53
```

### Result

The filtered RSSI changed more smoothly than the raw RSSI.
For example, when the raw RSSI changed suddenly from `-63` to `-38`, the filtered RSSI only changed from `-59.91` to `-55.53`.

### Observation

The filter successfully reduced sudden RSSI jumps. However, the filtered value still responds to real distance changes, so it can be used for rough distance classification.

### Next Step

Use the filtered RSSI value to classify the target into distance zones.

## Test 3 - RSSI Zone Classification

Date: 2026-06-25

### Goal

Classify the target distance into three zones based on filtered RSSI:

* `TOO CLOSE`
* `GOOD DISTANCE`
* `TOO FAR`

### Current Thresholds

```text
TOO CLOSE: filteredRSSI > -48
GOOD DISTANCE: -62 <= filteredRSSI <= -48
TOO FAR: filteredRSSI < -62
```

### Serial Monitor Sample

```text
===== SmartCart RSSI Filter Scanner Start =====
Best target | MAC: 42:b6:98:94:5b:de | Raw RSSI: -47 | Filtered RSSI: -47.00 | Zone: TOO CLOSE
Best target | MAC: 42:b6:98:94:5b:de | Raw RSSI: -48 | Filtered RSSI: -47.20 | Zone: TOO CLOSE
Best target | MAC: 42:b6:98:94:5b:de | Raw RSSI: -49 | Filtered RSSI: -47.56 | Zone: TOO CLOSE
Best target | MAC: 42:b6:98:94:5b:de | Raw RSSI: -61 | Filtered RSSI: -50.25 | Zone: GOOD DISTANCE
Best target | MAC: 42:b6:98:94:5b:de | Raw RSSI: -67 | Filtered RSSI: -53.60 | Zone: GOOD DISTANCE
Best target | MAC: 42:b6:98:94:5b:de | Raw RSSI: -60 | Filtered RSSI: -54.88 | Zone: GOOD DISTANCE
```

### Result

The ESP32-C3 successfully classified the target into distance zones using the filtered RSSI value.
The `TOO CLOSE`, `GOOD DISTANCE`, and `TOO FAR` states were all observed during testing.

### Estimated Distance Meaning

These RSSI zones are used as rough distance categories, not exact measurements.

```text
TOO CLOSE: approximately 0–40 cm
GOOD DISTANCE: approximately 40–100 cm
TOO FAR: approximately more than 100 cm
```

### Observation

RSSI is affected by body blocking, device direction, room reflection, and BLE signal variation. Because of this, the smart cart should use distance zones instead of exact distance values.

### Next Step

Create a separate `motor_test.ino` file to test motor movement before combining BLE detection with motor control.
