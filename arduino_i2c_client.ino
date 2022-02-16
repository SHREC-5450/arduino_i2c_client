#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 gyro = Adafruit_BNO055(55, 0x28);

char ori[3][8];
char accel[3][8];
char requestID = '\0';

void receiveEvent(int);
void requestEvent();
float* getOrientation();
float* getAcceleration();

void setup() {
    Wire.begin(0x27);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    Serial.begin(115200);

    if (!gyro.begin()) {
        Serial.println("No Gyroscope detected!");
        while(1);
    }
}

void loop() {
    float* temp;

    temp = getOrientation();
    for (int i = 0; i < 3; i++) {
        String str = String(temp[i]);

        for (int j = 0; j < 7; j++) {
            if (str.length() <= j) ori[i][j] = '0';
            else ori[i][j] = str.charAt(j);
        }
        ori[i][7] = '\0';
    }
    delete [] temp;

    temp = getAcceleration();
    for (int i = 0; i < 3; i++) {
        String str = String(temp[i]);

        for (int j = 0; j < 7; j++) {
            if (str.length() <= j) accel[i][j] = '0';
            else accel[i][j] = str.charAt(j);
        }
        accel[i][7] = '\0';
    }
    delete [] temp;

    delay(100);
}

void receiveEvent(int h) {
    requestID = Wire.read();
}

void requestEvent() {
    switch (requestID)
    {
    case 'o':
        Wire.write(ori[0]);
        Wire.write(ori[1]);
        Wire.write(ori[2]);
        break;
    
    case 'a':
        Wire.write(accel[0]);
        Wire.write(accel[1]);
        Wire.write(accel[2]);
        break;

    default:
        Wire.write(-1);
        break;
    }
}

float* getOrientation() {

    sensors_event_t event;
    gyro.getEvent(&event);
    float* orientation = new float[3];
    
    orientation[0] = event.orientation.x;
    orientation[1] = event.orientation.y;
    orientation[2] = event.orientation.z;

    return orientation;
}

float* getAcceleration() {
    sensors_event_t event;
    gyro.getEvent(&event);
    float* acceleration = new float[3];

    acceleration[0] = event.acceleration.x;
    acceleration[1] = event.acceleration.y;
    acceleration[2] = event.acceleration.z;

    return acceleration;
}