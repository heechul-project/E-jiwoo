// 3-5-0 서서히 밝아지고 어두워지게
// 100Hz, 100scale PWM
// 루프 주기는 2ms

#include <avr/wdt.h>

bool run_once = false;

uint8_t countISR = 0;
uint8_t count1to100 = 0;
uint8_t count_dimming = 0;
uint8_t duty = 0;
uint8_t seq_dimming = 0;


void setup() {
  //set pins as outputs
  Serial.begin(9600);
  Serial.println("reset");

  pinMode(13, OUTPUT);  // built in led
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
    proc_dimming();

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

  if (count1to100 > duty) digitalWrite(10, LOW);
  else digitalWrite(10, HIGH);
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

void proc_dimming(void) {
  if (count_dimming++ >= 9) {
    count_dimming = 0;

    switch (seq_dimming) {
      case 0:
        if (duty++ >= 100) seq_dimming = 1;
        break;

      case 1:
        if (--duty == 0) seq_dimming = 0;
        break;
    }
  }
}