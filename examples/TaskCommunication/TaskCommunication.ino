#include "periodicTask.hpp"
#include "messageQueue.hpp"

// Shared message queue for button state communication
MessageQueue<bool> buttonStateQueue;

// Task 1: Button Reader Task - reads button every 100ms
class ButtonReaderTask final : public PeriodicTask {
    const uint8_t buttonPin;

    void update() override {
        bool currentButtonState = digitalRead(this->buttonPin);

        // Send button state to queue (inverted because of pull-up)
        buttonStateQueue.send(currentButtonState);
    }

public:
    ButtonReaderTask(const uint8_t pin, const unsigned long period)
        : PeriodicTask(period), buttonPin(pin) {}

    void begin() override {
        pinMode(this->buttonPin, INPUT_PULLUP);
    }
};

// Task 2: LED Controller Task - processes queue and controls LED
class LEDControllerTask final : public PeriodicTask {
    const uint8_t ledPin;

    void update() override {
        bool buttonState;

        // Try to read from message queue
        if (buttonStateQueue.read(buttonState)) {
            // Update LED based on button state from queue
            digitalWrite(this->ledPin, buttonState ? HIGH : LOW);
        }
    }

public:
    LEDControllerTask(const uint8_t pin, const unsigned long period)
        : PeriodicTask(period), ledPin(pin) {}

    void begin() override {
        pinMode(this->ledPin, OUTPUT);
    }
};

// Global task instances
ButtonReaderTask buttonReader(2, 100);  // Read button every 100ms
LEDControllerTask ledController(LED_BUILTIN, 100);    // Process queue every 100ms

void setup() {
    // Initialize both tasks
    buttonReader.begin();
    ledController.begin();
}

void loop() {
    // Run both tasks
    buttonReader.run();
    ledController.run();

    // do something
}