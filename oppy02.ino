#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(2, 3); // RX, TX

#define enA 10
#define in1 9 
#define in2 8 
#define in3 7 
#define in4 6
#define enB 5 

#define servo A4

#define echo A2    //Echo pin
#define trigger A3 //Trigger pin

int distance_L, distance_F = 30, distance_R;
long distance;
int set = 20;

int bt_data; // Variable que recibe los datos del serial port
int Speed = 130;  
int mode=0;
int IR_data;

void setup(){ // declaraciones del setup

pinMode(echo, INPUT );// Ultrasonido Echo pin o input
pinMode(trigger, OUTPUT); // Ultrasonido Trigger pin o Output  

pinMode(enA, OUTPUT); 
pinMode(in1, OUTPUT);  
pinMode(in2, OUTPUT);  
pinMode(in3, OUTPUT);   
pinMode(in4, OUTPUT);
pinMode(enB, OUTPUT); 

Serial.begin(9600); // start serial communication at 9600bps
BT_Serial.begin(9600); 

pinMode(servo, OUTPUT);

 for (int angle = 60; angle <= 120; angle += 10)  {
   servoPulse(servo, angle);  }
 for (int angle = 120; angle >= 0; angle -= 10)  {
   servoPulse(servo, angle);  }

 for (int angle = 0; angle <= 60; angle += 10)  {
   servoPulse(servo, angle);  }
delay(500);
}


void loop(){  

if(BT_Serial.available() > 0){  // si es que se recibe datos lo guarda     
bt_data = BT_Serial.read(); 
Serial.println(bt_data);     
if(bt_data > 20){Speed = bt_data;}      
}

     if(bt_data == 8){mode=0; Stop();}    // Modo Manual   
else if(bt_data == 9){mode=1; Speed=130;} // Reservado 
else if(bt_data ==10){mode=2; Speed=130;} // Esquivador de obstaculos

analogWrite(enA, Speed); // velocidad para el motor1
analogWrite(enB, Speed); // velocidad para el motor2 

if(mode==0){     

// Controles del mando 
     if(bt_data == 1){forward(); }  
else if(bt_data == 2){backward();} 
else if(bt_data == 3){turnLeft();}  
else if(bt_data == 4){turnRight();} 
else if(bt_data == 5){Stop(); } 

// Control de voz  
else if(bt_data == 6){turnLeft();  delay(400);  bt_data = 5;}
else if(bt_data == 7){turnRight(); delay(400);  bt_data = 5;}
else if(bt_data == 11){forward();  delay(1000);  bt_data = 5;}
else if(bt_data == 12){backward(); delay(1000);  bt_data = 5;} 
else if(bt_data == 13){turnLeft();  delay(1000);  bt_data = 5;}
else if(bt_data == 14){turnRight(); delay(1000);  bt_data = 5;} 
else if(bt_data == 15){forward();  delay(200); Stop(); delay(200); backward(); delay(200); Stop(); delay(200); forward(); delay(200); Stop(); delay(200); backward(); delay(200);  bt_data = 5;} // if the bt_data '15' Adelante y atrás 
else if(bt_data == 16){turnLeft();  delay(200); Stop(); delay(100); turnRight(); delay(200); Stop(); delay(100); turnLeft();  delay(200); Stop(); delay(100); turnRight(); delay(200);  bt_data = 5;} // if the bt_data '16' Izquierda y derecha
}

// mode =1 reservado uu

if(mode==2){    
// Modo esquiva u Obstacle avoiding      
 distance_F = Ultrasonic_read();
 Serial.print("S=");Serial.println(distance_F);
  if (distance_F > set){forward();}
    else{Check_side();}
}

delay(10);
}

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convertir angulos en microsegundos
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(50);                   // Descansito
}

// Lectura del ultradsonido
long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  distance = pulseIn (echo, HIGH);
  return distance / 29 / 2;
}

void compareDistance(){
       if (distance_L > distance_R){
  turnLeft();
  delay(350);
  }
  else if (distance_R > distance_L){
  turnRight();
  delay(350);
  }
  else{
  backward();
  delay(300);
  turnRight();
  delay(600);
  }
}

void Check_side(){ // verificador
    Stop();
    delay(100);
 for (int angle = 60; angle <= 120; angle += 10)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_L = Ultrasonic_read();
    delay(100);
  for (int angle = 120; angle >= 0; angle -= 10)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_R = Ultrasonic_read();
    delay(100);
 for (int angle = 0; angle <= 60; angle += 10)  {
   servoPulse(servo, angle);  }
    delay(300);
    compareDistance();
}

void forward(){ 
digitalWrite(in1, HIGH); 
digitalWrite(in2, LOW);
digitalWrite(in3, HIGH); 
digitalWrite(in4, LOW);
}

void backward(){ 
digitalWrite(in1, LOW);   
digitalWrite(in2, HIGH);
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
}

void turnRight(){
digitalWrite(in1, LOW); 
digitalWrite(in2, HIGH); 
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
}

void turnLeft(){
digitalWrite(in1, HIGH); 
digitalWrite(in2, LOW); 
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH); 
}

void Stop(){
digitalWrite(in1, LOW); 
digitalWrite(in2, LOW); 
digitalWrite(in3, LOW); 
digitalWrite(in4, LOW);
}
