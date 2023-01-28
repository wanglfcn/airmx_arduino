#define FAN_OUT 8
#define WATER_LOW_SENSOR A2
#define WATER_FULL_SENSOR A3
#define WATER_LOW_LED 10
#define FAN_SPEED A0
#define PUMP 9
#define UV_LAMP 12
#define GATE_BUTTON A1
#define GATE_MOTOR 11
#define WATER_FULL_LED 13


int count = 0;
int delay1 = 1200;
int enable_fan = 1;
int gate_is_open = 0;
unsigned long gate_btn_last_ts = 0;
unsigned long gate_last_open_ts = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(FAN_OUT, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(WATER_LOW_LED, OUTPUT);
  pinMode(WATER_FULL_LED, OUTPUT);
  pinMode(UV_LAMP, OUTPUT);
  pinMode(GATE_MOTOR, OUTPUT);

  digitalWrite(PUMP, HIGH);
  digitalWrite(UV_LAMP, HIGH);
  analogWrite(WATER_LOW_LED, 0);
}



void loop() {
  // put your main code here, to run repeatedly:
  count ++;
  if (count > 0) {
    count = 0;
    int sensorValue = analogRead(FAN_SPEED) * 4;

    delay1 = 1200 + sensorValue;
//    Serial.print("delay: ");
//    Serial.println(delay1);
    int waterLowSensor = analogRead(WATER_LOW_SENSOR);
//    Serial.print("waterLowSensor: ");
//    Serial.println(waterLowSensor);
    int gateButton = analogRead(GATE_BUTTON);
 //   Serial.print("gateButton: ");
 //   Serial.println(gateButton);
    unsigned long ts = millis();
    if (gateButton < 300) {
        if (gate_is_open == 0) {
          // open the door
          gate_last_open_ts = ts;
          digitalWrite(GATE_MOTOR, HIGH);
          gate_is_open = 1;
          Serial.println("open the gate");
        } else {
          digitalWrite(GATE_MOTOR, LOW);
          gate_is_open = 0;
          Serial.println("close the gate");
        }

        delay(200);

    }

    if (gate_is_open > 0 && (ts - gate_last_open_ts > 300000)) {
      // open too long, close the gate
      digitalWrite(GATE_MOTOR, LOW);
      gate_is_open = 0;
      Serial.println("wait too long, close the gate");
    }


    if (waterLowSensor > 300) {
      //  water is low, turn on water led
      analogWrite(WATER_LOW_LED, 64);
      digitalWrite(PUMP, LOW);
      digitalWrite(UV_LAMP, LOW);
      enable_fan = 0;
    } else {
      analogWrite(WATER_LOW_LED, 0);
      digitalWrite(PUMP, HIGH);
      digitalWrite(UV_LAMP, HIGH);
      enable_fan = 1;
    }
  }

  if (enable_fan > 0) {
    digitalWrite(FAN_OUT, LOW);
  }
  delayMicroseconds(delay1);
  if (enable_fan > 0) {
    digitalWrite(FAN_OUT, HIGH);
  }
  delayMicroseconds(delay1);

}
