// 3-5-1 서서히 밝아지고 어두워지게 LED각각 다른 속도로
// 100Hz, 100scale PWM
// 루프 주기는 2ms

#include <avr/wdt.h>

bool run_once = false;

uint8_t countISR = 0;
uint8_t count1to100 = 0;
uint8_t count_dimming1, count_dimming2 = 0;
uint8_t duty1, duty2 = 0;
uint8_t seq_dimming1, seq_dimming2 = 0;


void setup() {
  //set pins as outputs
  Serial.begin(9600);
  Serial.println("reset");

  pinMode(13, OUTPUT);  // built in led
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  cli();  //stop interrupts
  set_timer_10KHz();
  sei();  //allow interrupts

  wdt_enable(WDTO_15MS);

}  //end setup

void loop() {
  //do other things here
  wdt_reset();

  if (run_once == true) {  // 2ms 마다
    run_once = false;

    digitalWrite(13, HIGH);
    // start
    proc_dimming1();
    proc_dimming2();

    // end
    digitalWrite(13, LOW);
  }
}

ISR(TIMER1_COMPA_vect) {  // 100us마다
  if (countISR++ >= 19) {
    countISR = 0;

    run_once = true;
  }

  if (count1to100++ == 100) count1to100 = 1;

  if (count1to100 > duty1) digitalWrite(10, LOW);
  else digitalWrite(10, HIGH);

  if (count1to100 > duty2) digitalWrite(9, LOW);
  else digitalWrite(9, HIGH);
}

void set_timer_1KHz() {
  //set timer1 interrupt at 1KHz
  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   //initialize counter value to 0
  // set compare match register for 1KHz increments
  OCR1A = 249;  // = (16*10^6) / (1000*64) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 and CS10 bits for 64 prescaler
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void set_timer_10KHz() {
  //set timer1 interrupt at 10KHz
  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   //initialize counter value to 0
  // set compare match register for 10KHz increments
  OCR1A = 199;  // = (16*10^6) / (10000*8) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 and CS10 bits for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void proc_dimming1(void) {
  if (count_dimming1++ >= 9) {
    count_dimming1 = 0;

    switch (seq_dimming1) {
      case 0:
        if (duty1++ >= 100) seq_dimming1 = 1;
        break;

      case 1:
        if (--duty1 == 0) seq_dimming1 = 0;
        break;
    }
  }
}

void proc_dimming2(void) {
  if (count_dimming2++ >= 4) {
    count_dimming2 = 0;

    switch (seq_dimming2) {
      case 0:
        if (duty2++ >= 100) seq_dimming2 = 1;
        break;

      case 1:
        if (--duty2 == 0) seq_dimming2 = 0;
        break;
    }
  }
}