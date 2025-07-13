#include "periodicTask.hpp"

// Periodic LED blinking task
class LEDBlinkTask final : public PeriodicTask {
    const uint8_t ledPin;
    bool ledState;

    void update() override {
        // Toggle LED state
        ledState = !ledState;
        digitalWrite(this->ledPin, ledState ? HIGH : LOW);
    }

public:
    LEDBlinkTask(const uint8_t pin, const unsigned long blinkPeriod)
        : PeriodicTask(blinkPeriod), ledPin(pin), ledState(false) {}

    void begin() override {
        // Configure pin and initialize LED state
        pinMode(this->ledPin, OUTPUT);
        ledState = false;
    }
};

// Global task instance
LEDBlinkTask ledBlink(13, 1000); // Blink every 1 second

void setup() {
    // Initialize the LED blink task
    ledBlink.begin();
}

void loop() {
    // Run the blinking task
    ledBlink.run();

    // do something
}