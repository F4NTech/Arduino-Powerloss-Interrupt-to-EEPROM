#include <EEPROM.h>

// this program to learn for simulation :D
// farhan@datacakra.com
// ref : @dariee on Youtube : https://www.youtube.com/watch?v=HnLKS8_OzC8. Big thanks for you _/\_

int Interrupt2 = 3, Interrupt1 = 2;               // Interrupt Pin on Arduino Uno R3. (Just pin 2 and 3)
int NG_UP = 4, NG_DOWN = 5, Reset = 12;           // Simulation Button

int StateUp, StateDown, Value, ResState;          //State Var
int countUp = 0, countDown = 0;
int PowerState;


void setup() {
  Serial.begin(9600);

  pinMode(A0, OUTPUT), digitalWrite(A0, HIGH);    //pin Powerloss get 5v when starting simulation
  delay(2000);
  
  //  EEPROM.clear();
  pinMode(Interrupt2, INPUT);
  pinMode(Interrupt1, INPUT);
  
  pinMode(NG_UP, INPUT_PULLUP);
  pinMode(NG_DOWN, INPUT_PULLUP);
  
  pinMode(Reset, INPUT_PULLUP);

  pinMode(A0, INPUT);                               // change pin function to detect PowerLoss
  Value = EEPROM.read(0);
  
  attachInterrupt(digitalPinToInterrupt(Interrupt2), PowerLoss, LOW);
  attachInterrupt(digitalPinToInterrupt(Interrupt1), InterruptA, RISING);
  Serial.println("Start");
}

void loop() {
  //
  //
  //
}
void InterruptA() {
  StateUp = digitalRead(NG_UP), StateDown = digitalRead(NG_DOWN), ResState = digitalRead(Reset);
  if (digitalRead(Interrupt1)) {
    if (StateUp == LOW && countUp == 0) {       //NG UP
      Value += 1;
      countUp = 1;
      Serial.println(Value);
    }
    if ( StateUp == HIGH) {
      countUp = 0;
    }

    if ( StateDown == LOW && countDown == 0) {            // NG Less
      if ( Value > 0) {
        Value -= 1;
        countDown = 1;
        Serial.println(Value);
      }
    }
    if ( StateDown == HIGH) {
      countDown = 0;
    }
    if (ResState == LOW) {                    // RESET All Address Value
      Serial.println("Reseting Address !");
      delay(1000);
      for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
        //      Serial.println("Reset Addres : " + String(i));
      }
      Serial.println("Done Reset Value EEPROM Address");
      Value = 0;
    }

  }


}
void PowerLoss() {
  if (digitalRead(Interrupt2) && analogRead(A0) < 920) { //920 Analog Power Value 4.5 V / Under 5V
    EEPROM.write(0, Value);   //Save count Value to Address 0 EEPROM.
    // Serial.println("save");
    

    delay(12345);
  }
}
