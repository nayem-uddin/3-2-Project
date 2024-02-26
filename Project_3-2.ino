#define gt2 2 //gate terminal 2
#define gt1 3 //gate terminal 1
#define win1 4 //window terminal 1
#define win2 5 //window terminal 2
#define ist 6 //inner sensor trigger
#define ise 7 //inner sensor echo
#define alarm 8 //fire alarm
#define fan 9 //fan
#define light 10 //indoor light
#define ose 11 //outer sensor echo
#define ost 12 //outer sensor trigger
#define i_ldr A0 //inner LDR
#define o_ldr A1 //outer LDR
#define fire A2 //smoke sensor
#define tmp A4 //temperature sensor
int circuit=LOW;
void setup() {
  pinMode(tmp,INPUT);
  pinMode(o_ldr,INPUT);
  pinMode(i_ldr,INPUT);
  pinMode(fire,INPUT);
  pinMode(ise,INPUT);
  pinMode(ose,INPUT);
  pinMode(13,OUTPUT); //circuit switch
  pinMode(gt1,OUTPUT);
  pinMode(gt2,OUTPUT);
  pinMode(fan,OUTPUT);
  pinMode(ist,OUTPUT);
  pinMode(ost,OUTPUT);
  pinMode(alarm,OUTPUT);
  pinMode(light,OUTPUT);
}

void loop() {
   //distance measure for outer sensor
  digitalWrite(ost, LOW);
  delay(2);
  digitalWrite(ost, HIGH);
  delay(10);
  digitalWrite(ost, LOW);
  long duration_o = pulseIn(ose, HIGH);
  int distance_o = duration_o * 0.034 / 2;
  //distance measure for inner sensor
  digitalWrite(ist, LOW);
  delay(2);
  digitalWrite(ist, HIGH);
  delay(10);
  digitalWrite(ist, LOW);
  long duration_i = pulseIn(ise, HIGH);
  int distance_i = duration_i * 0.034 / 2;
  if(distance_o<=5)
  {
    circuit=HIGH;
    }
    if(distance_i<=5)
    {
      circuit=LOW;
      }
      else{
        circuit=circuit;
      }
      digitalWrite(13,circuit);
      if((distance_o<=5)||(distance_i<=5)){
        //gate open
        analogWrite(gt1,0);
        analogWrite(gt2,160);
        delay(200);
        //wait
        analogWrite(gt1,0);
        analogWrite(gt2,0);
        delay(1000);
        //gate close
        analogWrite(gt1,170);
        analogWrite(gt2,0);
        delay(200);
        //stay
        analogWrite(gt1,0);
        analogWrite(gt2,0);
      }
  int v=analogRead(tmp); 
  float tem = v*0.2048; //voltage to temperature  
  digitalWrite(fan,tem>25); //fan on for over temperature
Serial.println(analogRead(fire));
  digitalWrite(alarm,analogRead(fire)<10&&analogRead(fire)>2); //alarm on if smoke detected
  digitalWrite(light,analogRead(o_ldr)<150&&analogRead(o_ldr)>2); //light on when it is evening (2 is the offset voltage while circuit is off)
  //window moves if indoor light intensity is out of optimum range during daytime
  if(analogRead(o_ldr)>150&&analogRead(i_ldr)<150&&analogRead(i_ldr)>2){ 
    analogWrite(win1,150); 
    analogWrite(win2,0);
  }  
  else if(analogRead(o_ldr)>150&&analogRead(i_ldr)>300){
    analogWrite(win2,150); 
    analogWrite(win1,0);    
  }
  else{
    analogWrite(win1,0);
    analogWrite(win2,0);
  }
}