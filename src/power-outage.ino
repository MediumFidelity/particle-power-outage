#include "Particle.h"

SYSTEM_THREAD(ENABLED);

// Enable app TRACE logging
SerialLogHandler logHandler(115200, LOG_LEVEL_ERROR, {
    { "app", LOG_LEVEL_TRACE }, // enable all app messages
});

char variableName[] = "home_power_state";
char publishString[24];
bool lastPowerState = false;
bool powerState = false;
unsigned long outageStartTime = 0;
unsigned long outageTime = 0;


void setup() {
    // Configure the PWR to monitor for external power
	pinMode(PWR, INPUT);
	// Use the built-in LED for signaling power state
	pinMode(D7, OUTPUT);
	
	// Create a cloud variable to publish to
    if(Particle.variable(variableName, powerState) == false) {
        // Failure to register, but how will we tell anyone?
        // TODO: Add LED/LCD notification
        Log.error("Failed to create '%s' Particle variable.", variableName);
    }
	powerState = digitalRead(PWR);
}


void loop() {
    // Read the power state often, to catch power blips
    // as best we can.  The USB power suppply might hide
    // *very* short outages
	bool powerState = digitalRead(PWR);
	if (powerState != lastPowerState) {
	    // State change, remember our new state
		lastPowerState = powerState;
		
		// Clear the string we're going to publish (overkill, but let's be safe)
		memset(publishString, 0, sizeof(publishString));
		
		if(powerState == false) {
		    // The power just went out, hope it's not raining too hard.
		    outageStartTime = millis();
		    strcpy(publishString,  (powerState ? "on" : "off"));
		}
		else {
		    // And we're back, figure out how long power was out
		    outageTime = millis() - outageStartTime;
		    sprintf(publishString,"on after %lu ms", outageTime);
		}
		
		boolean success = Particle.publish(variableName, publishString, PRIVATE);
		if (success == false) {
		    // TODO: Again, going to have to light up an LED or something
		    Log.error("Failed to publish '%s' variable.", variableName);
		}
		
		Log.info("powerState=%d", powerState);
		digitalWrite(D7, powerState);
	}
}