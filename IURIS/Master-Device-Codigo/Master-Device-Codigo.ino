//code for the Master device. ESSE É O QUE TA FUNCIONANDO COM O BOTÃO INVERTIDO
#include <Adafruit_Sensor.h>

#include <Wire.h>

#include <NewPing.h>


#include <LiquidCrystal.h>
LiquidCrystal lcd(9, 8, 13, 12, 11, 10);/*assigning a reference name and configuring the pins for the Liquid Crystal library*/
boolean mode1 = false, mode2 = false, mode3 = false, mode4 = false; //assigning four boolean variables for 4 differnt modes.
boolean one, two, three, four;//variables for mode toggling. You'll understand as you proceed.
int select, change, M2; // variables to store the push button values.

int in1 = 3, in2 = 4, in3 = 5, in4 = 6;//integer variables for the motordriver input pins.  
NewPing sensor2(7, 7, 500); // NewPing setup of pin and maximum distance.
NewPing sensor1(2, 2, 500); // NewPing setup of pin and maximum distance.
int n;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 Wire.begin();// initailizing the Wire library
 lcd.begin(16, 2);// initializing the LiquidCrystal library.
 lcd.write("Ol~~");
 lcd.setCursor(0, 2);
 lcd.write("Me chamo IURIS");
 delay(3000);
 lcd.clear();
 pinMode(2, INPUT);
 
}

int Get(){
  // the Master device requests the sensor info.(i.e) The distance measured by the third ultrasonic sensor. 
 Wire.requestFrom(1, 2);
 if (Wire.available()){
  n =  Wire.read();
  return n;
 }
}

//seperate functions to drive the robot in different directions.
int goStraight(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

int stopp(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

int turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

int backRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

int turnLeft(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

int backLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

int goBack() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
int SPIN() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  
  mode1 = true;
  mode2 = false;
  mode3 = false;
  mode4 = false;
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.write(";) yeaah");
  delay(100);
  wait: // a particular section where the program will jump as you toggle between the modes.
  delay(200);// this delay is very important because this is what which gives you the time to toggle between another mode before you go on to the next
  if (three == true){//checks if three is true. If yes then makes it false and makes four true.
      lcd.clear();
      lcd.write("Obj avoid mode");
      four = false;
      four = false;
      lcd.setCursor(1, 1);// sets the cursor and writes 'selected'
      lcd.write("Selected");
      mode2 = false;
      mode3 = true;
      mode4 = false;
      goto wait;//jumps back to the particular portion of the program and waits until any pushbutton is pressed.
  }
  else if(four == true){//checks if four was true. If yes then makes it false and make one again true.
    two = false;
    three = false;
    lcd.clear();
    lcd.write("Obj follow mode");
    lcd.setCursor(1, 1);// sets the cursor and writes 'selected'
    lcd.write("Selected");
    mode4 = true;
    goto wait;//jumps back to the particular portion of the program and waits until any pushbutton is pressed.
  }

while(mode1 == true){// if mode1 is true then, the program enters this while loop and is executed until the reset button it pressed on the arduino.
  if(Serial.available() > 0){//if thing is communicating through serial interface.
int setBit = Serial.parseInt();
    if (setBit == 1){
      goStraight();
    }
    else if (setBit == 2){
     turnRight();
    }
    else if(setBit == 3){
      goBack();
    }
    else if (setBit == 4){
     turnLeft();
    }
    else if (setBit == 5){
      stopp();
    }
    else if(setBit == 6){
      SPIN();
    }
    else if (setBit == 10){
      lcd.clear();    
    }
    else if (setBit == 22){
       lcd.clear();
       lcd.write("Line follower");
       three = false;
       four = false;
       lcd.setCursor(1, 1);// sets the cursor and writes 'selected'
       lcd.write("Selected");
       mode2 = true;
       mode3 = false;
       mode4 = false;
       goto wait;//jumps back to the particular portion of the program and waits until any pushbutton is pressed.
    }
    else{}//if Serial.available() function returns 0, then nothing happens.
  }
  
}
 boolean r = false, l = false, start = true;
while(mode2 == true){// this mode enables the robot to follow a line.
 if (analogRead(A3) > 500){
  turnLeft();
 }
 else {
  turnRight();
 }
}
boolean first = false;//this variable enables to execute something only once.
while(mode3 == true){
 if (first == false){
  Wire.beginTransmission(1);
  Wire.write(3);
  Wire.endTransmission();
  first = true;//permanently makes the variable true so that the above is not executed again.
}
 
 if (Get() < 30 && Get() > 7){// if the sensor on the Left shows the distance between 30cms and 7cms, then the robot turns left.
  turnLeft();
 }
 else if(sensor1.ping_cm() < 30 && sensor1.ping_cm() >7){//if the sensor on the Right shows the distance between 30cm and 7cm, then the robot turns Right.
  turnRight();
 }
 else if(sensor2.ping_cm() < 30 && sensor2.ping_cm() > 7){// if the sensor in the middle senses the distance between 30cm and 7cm, then the robot goes forward.
  goStraight();
 }
 else if (sensor2.ping_cm() < 5){// if the sensor in the middle senses the distance lesser than 5 then the robot goes backward.
  goBack();
 }
 
 
 else {// otherwise the robot does not move.
  stopp();
 }
}
boolean checkit = true;// this variable again enables you to execute a particular part of the program only once.
while (mode4 == true){
  if (checkit = true){
    Wire.beginTransmission(1);
    Wire.write(4);//writes '4' to the slave device. When the slave recieves the info, it tilts the servos perpendicular to the middle servo in the opposite direction. 
    Wire.endTransmission();
    delay(500);
    checkit = false;
  }
  goStraight();//goes forward.
  boolean direct = true;// its another variable which will help you to execute a part of the program only once.
  int a,b,c;
  c = sensor2.ping_cm();
  while(c < 17){//checks if any object if in the front.
   if(direct == true){
    a = Get();
    b = sensor1.ping_cm();
    if (a>b){
      turnLeft();
    }
    else {
      turnRight();
    }
    }
    direct = false;
    delay(400);
    c = sensor2.ping_cm();
    
  }
}
}//thus the program confined to the master device ends
