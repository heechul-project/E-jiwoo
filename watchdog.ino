#include <avr/wdt.h>

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("reset");

  wdt_enable(WDTO_1S);

}

void loop() {
  // put your main code here, to run repeatedly:
  wdt_reset();

  delay(2000); // 이 함수를 활성화 시키면 와치도그 타이머가 넘쳐서 리셋됩니다.

}
