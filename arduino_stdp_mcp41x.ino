#include <SPI.h>
const int CS_PIN = 10;
const int PRE_PIN = 2;
const int POST_PIN = 3;
volatile unsigned long t_pre = 0;
volatile unsigned long t_post = 0;
volatile bool pre_flag = false;
volatile bool post_flag = false;
int w = 128;
void writeDigipot(uint8_t value){
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x11);
  SPI.transfer(value);
  digitalWrite(CS_PIN, HIGH);
}
void isr_pre(){
  t_pre = micros();
  pre_flag = true;
}
void isr_post(){
  t_post = micros();
  post_flag = true;
}
void setup(){
  Serial.begin(115200);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  pinMode(PRE_PIN, INPUT_PULLUP);
  pinMode(POST_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PRE_PIN), isr_pre, FALLING);
  attachInterrupt(digitalPinToInterrupt(POST_PIN), isr_post, FALLING);
  writeDigipot(w);
}
void loop(){
  if(pre_flag && post_flag){
    noInterrupts();
    long dt = (long)(t_post - t_pre);
    pre_flag = post_flag = false;
    interrupts();
    if(dt > 0 && dt < 20000){
      int delta = map(dt, 0, 20000, 10, 1);
      w = constrain(w - delta, 0, 255);
      Serial.print("potentiation dt=");
      Serial.print(dt);
      Serial.print(" w=");
      Serial.println(w);
    } else if(dt < 0 && dt > -20000){
      int delta = map(-dt, 0, 20000, 10, 1);
      w = constrain(w + delta, 0, 255);
      Serial.print("depression dt=");
      Serial.print(dt);
      Serial.print(" w=");
      Serial.println(w);
    }
    writeDigipot((uint8_t)w);
  }
}
