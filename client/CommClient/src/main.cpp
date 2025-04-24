#include <Arduino.h>
#include <CommClient.h>

void onStart(bool start) { // Event when the communication module requests to start
	if (start)
		Serial.println("Communication module requested start");
}

void setup() {
    setupCommClient(MODULE_ACTUATOR_1, onStart); // Add this to setup communication
}

void loop() {
    delay(1000);
    requestCommState(CURIOUS); // Request the communication module to express a reaction
}
