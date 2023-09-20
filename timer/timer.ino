
// 타이머1 를 사용해서 1ms 마다 타이머 인터럽트 발생

bool run_once = false;
bool toggle0 = false;

void setup() {
  //set pins as outputs
  pinMode(8, OUTPUT);
  pinMode(13, OUTPUT);

  cli();  //stop interrupts

  set_timer_1KHz();

  sei();  //allow interrupts

}  //end setup

void loop() {
  //do other things here
  if (run_once == true) {
    run_once = false;

    digitalWrite(13, HIGH);

    if (toggle0) {
      digitalWrite(8, HIGH);
      toggle0 = false;
    } else {
      digitalWrite(8, LOW);
      toggle0 = true;
    }

    digitalWrite(13, LOW);
  }
}

ISR(TIMER1_COMPA_vect) {  //timer0 interrupt 1kHz toggles pin 8
                          //generates pulse wave of frequency 1kHz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  run_once = true;
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
