#include <Stepper.h>

//硬體按鈕宣告
const int start_stop = 5; //常量宣告
const int sensorR = 2;
const int sensorL = 6;
const int uartread = 4;
//馬達腳位宣告
const int pwmA = 3;
const int pwmB =  11;
const int brakeA =  9;
const int brakeB =  8;
const int dirA =  12;
const int dirB =  13;

//馬達常數宣告
const int stepsPerRevolution = 2048;
Stepper myStepper =Stepper(stepsPerRevolution, dirA, dirB);

//變數宣告
volatile int stateFlag = 1; //變量宣告
volatile int t = 0; //循環變量 0 1 2
volatile int cont_r = 0; //右邊按鈕計數
volatile int cont_l = 0; // 左邊按鈕計數
volatile int degree = 512; //旋轉步組
volatile int tmp = 1; // print一次使用
volatile int r_step = 0; //右轉步組
volatile int l_step = 0; //左轉步組
volatile int totle_step = 0; //總步數
volatile int totle_cont = 0; //總次數

volatile int max_r = 0;
volatile int min_r = degree;
volatile int avg_r = 0;
volatile int sum_r =0;
volatile int tmp_r = 0;

volatile int max_l = 0;
volatile int min_l = degree;
volatile int avg_l = 0;
volatile int sum_l =0;
volatile int tmp_l = 0;

void setup() {
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(brakeB, OUTPUT);
  digitalWrite(pwmA, HIGH);
  digitalWrite(pwmB, HIGH);
  digitalWrite(brakeA, LOW);
  digitalWrite(brakeB, LOW);
  pinMode(start_stop,INPUT_PULLUP);
  pinMode(sensorR,INPUT_PULLUP);
  pinMode(sensorL,INPUT_PULLUP);
  // Set the motor speed (RPMs):
  myStepper.setSpeed(12);
  Serial.begin(9600);
}

void loop() {
  
 if(digitalRead(start_stop)==0){
    delay(30);
    while(digitalRead(start_stop)==0);
    if (stateFlag==1){
      stateFlag = 0;
    }
    else{
      stateFlag = 1;
    }
    tmp = 0;
 }    
 if(stateFlag==0){
  totle_cont = totle_cont +1;
  motor(degree);
  stateFlag = 2;
  }
 else if(stateFlag==1){
  if(tmp == 0){
  int cont_rl = cont_l + cont_r;
  avg_r = sum_r/cont_r;
  avg_l = sum_l/cont_l;
  Serial.println("===============================");
  Serial.print("總共轉了 ");
  Serial.print(totle_cont);
  Serial.println(" 次");
  Serial.println("----------------------------------------------------------");
  Serial.print("左邊");
  Serial.print(cont_l);
  Serial.print("次  ");
  Serial.print("最大步數");
  Serial.print(max_l);
  Serial.print("步 ");
  Serial.print("最小步數");
  Serial.print(min_l);
  Serial.print("步 ");
  Serial.print("平均步數");
  Serial.print(abs(avg_l));
  Serial.println("步 ");
  Serial.println("----------------------------------------------------------");
  
  Serial.print("右邊");
  Serial.print(cont_r);
  Serial.print("次 ");
  Serial.print("最大步數");
  Serial.print(max_r);
  Serial.print("步 ");
  Serial.print("最小步數");
  Serial.print(min_r);
  Serial.print("步 ");
  Serial.print("平均步數");
  Serial.print(avg_r);
  Serial.println("步 ");
  
  Serial.println("-------------------------------------");
  
  Serial.print("共計 ");
  Serial.print(cont_rl);
  Serial.println("次 ");
  
  cont_l = 0;
  cont_r = 0;
  totle_cont = 0;
  }
 tmp = 1;
 }
 else{
  stateFlag = 0;
 }
}


//==============馬達程式=============================

void motor (int x)
//step = (360/2048)*degree
{ 
 
  if(x >0){
      int i = 0;
      while(i < x){
        i = i+1; 
          if(digitalRead(sensorR)==0){
            degree = -512; //反轉
            cont_r = cont_r+1;
            l_step = i;
            stateFlag = 0;
            Serial.print("撞到右邊開關   ");
            if(i > max_r){
              max_r = i;
            }
            else if(i < min_r){
              min_r = i;
            }
            else{
              ;
            }
            sum_r = sum_r + i;
            break;
          }
          else{
            myStepper.step(1);
          }
        delay(1);
        
      }
      degree = -512;  
      Serial.print(F("往右轉了"));
      Serial.print(i);
      Serial.println("步");
   }
  else{
     int i = -1;
     while(i > x){
        i = i-1;
          if(digitalRead(sensorL)==0){
            degree = 512;
            r_step = i;
            cont_l = cont_l+1;
            stateFlag = 0;
            Serial.print("撞到左邊開關   ");
            if(abs(i) > max_l){
              max_l = abs(i);
            }
            else if(abs(i) < min_l){
              min_l = abs(i);
            }
            else{
              ;
            }
            sum_l = sum_l + abs(i);
            break;
          }
          else{
            myStepper.step(-1);
          }
      delay(1);  
     }
      degree = 512;
      Serial.print(F("往左轉了"));
      Serial.print(abs(i));
      Serial.println("步");
   }
}
