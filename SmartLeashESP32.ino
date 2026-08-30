#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define MOTOR_PIN 32

#define SERVICE_UUID        "00001234-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "0000ABCD-0000-1000-8000-00805f9b34fb"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Cliente conectado");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Cliente desconectado");
    BLEDevice::startAdvertising();
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {

    String value = pCharacteristic->getValue();

    Serial.print("Comando recibido: ");
    Serial.println(value);

    // ==========================
    // COMANDO 1 - VÁMONOS
    // ==========================
    if (value == "1") {

      Serial.println("VÁMONOS");

      digitalWrite(MOTOR_PIN, HIGH);
      delay(300);
      digitalWrite(MOTOR_PIN, LOW);
    }

    // ==========================
    // COMANDO 2 - FEA
    // ==========================
    else if (value == "2") {

      Serial.println("FEA");

      digitalWrite(MOTOR_PIN, HIGH);
      delay(200);
      digitalWrite(MOTOR_PIN, LOW);

      delay(200);

      digitalWrite(MOTOR_PIN, HIGH);
      delay(200);
      digitalWrite(MOTOR_PIN, LOW);
    }

    // ==========================
    // COMANDO 3 - QUIETA
    // ==========================
    else if (value == "3") {

      Serial.println("QUIETA");

      digitalWrite(MOTOR_PIN, HIGH);
      delay(1000);
      digitalWrite(MOTOR_PIN, LOW);
    }

    else {

      Serial.println("Comando desconocido");

    }
  }
};

void setup() {

  Serial.begin(115200);

  pinMode(MOTOR_PIN, OUTPUT);

  BLEDevice::init("SmartLeash");

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  Serial.println("SmartLeash listo y esperando conexión...");
}

void loop() {
  delay(1000);
}
