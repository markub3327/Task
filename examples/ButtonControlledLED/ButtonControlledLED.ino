#include "delayedTask.hpp"

#define BUTTON_PIN 2

// LED control task that turns on LED for 10 seconds
class LEDTimerTask final : public DelayedTask {
    const uint8_t ledPin;

    void update() override {
        // Turn off LED when task completes
        digitalWrite(this->ledPin, LOW);
    }

public:
    LEDTimerTask(const uint8_t pin, const unsigned long durationMs)
        : DelayedTask(durationMs), ledPin(pin) {}

    void begin() override {
        // Configure LED pin
        pinMode(this->ledPin, OUTPUT);
        digitalWrite(this->ledPin, HIGH);
        this->start();
    }
};

// Global instances
LEDTimerTask ledTimer(13, 10000); // 10 second timer
bool lastButtonState = HIGH;
bool buttonState = HIGH;

void setup() {
    // Initialize button pin with internal pull-up
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize LED timer task
    ledTimer.begin();
}

void loop() {
    // Read button state
    buttonState = digitalRead(BUTTON_PIN);

    // Detect button press (falling edge)
    if (lastButtonState == HIGH && buttonState == LOW) {
        ledTimer.begin(); // Start/restart the LED timer
        delay(50); // Simple debounce
    }

    lastButtonState = buttonState;

    // Run the LED timer task
    ledTimer.run();

    // do something
}