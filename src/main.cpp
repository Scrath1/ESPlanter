#include <Arduino.h>
#include "drivers/pump.h"
#include "global.h"

void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("ESPlanter");
    // put your setup code here, to run once:
    init_pump();
    Serial.println("Setup complete");
}

void loop() {
    // put your main code here, to run repeatedly:
    if(Serial.read() == 'p'){
        run_pump(3000);
    }
}
