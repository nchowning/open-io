// bitmask for each bit in a byte (starting with most-significant bit)
unsigned char bitmask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
// Word of clock data (1ms bit length)
unsigned char clock_word = 0x07;

// Function declarations
void writeByte(unsigned char);
void switchSensorUp();
void switchSensorDown();
void switchSensorLeft();
void switchSensorRight();

// Buttons for mode-switch testing
int buttonPinUp = 4;
int buttonPinDown = 5;
int buttonPinLeft = 6;
int buttonPinLeft = 7;
// Button debounce variables
int lastReading = LOW;
long lastDebounceTime = 0;
long debounceDelay = 100;

void setup()
{
    // Output Pins
    pinMode(2, OUTPUT);  // TEST pin (clock)
    pinMode(3, OUTPUT);  // FL5 pin (data)
    digitalWrite(2, HIGH);  // TEST defaults to HIGH
    digitalWrite(3, LOW);  // FL5 defaults to LOW

    // Input Pins
    pinMode(buttonPinUp, INPUT);  // Up Button
    pinMode(buttonPinDown, INPUT);  // Down Button
    pinMode(buttonPinLeft, INPUT);  // Left Button
    pinMode(buttonPinRight, INPUT);  // Right Button
}

void loop()
{
    int reading = 0;

    // Are any buttons pressed?
    if (digitalRead(buttonPinUp) || digitalRead(buttonPinDown) ||
            digitalRead(buttonPinLeft) || digitalRead(buttonPinRight))
        reading = 1;

    // Has our reading changed since the last loop?
    if (reading != lastReading)
        lastDebounceTime = millis();

    // Switch sensor modes if enough debounce time has passed
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (digitalRead(buttonPinUp))
            switchSensorUp();
        else if (digitalRead(buttonPinDown))
            switchSensorDown();
        else if (digitalRead(buttonPinLeft))
            switchSensorLeft();
        else if (digitalRead(buttonPinRight))
            switchSensorRight();
    }

    // Save this loop iteration's reading
    lastReading = reading;
}

void writeByte(unsigned char byte)
{
    // Initialize our cloc
    int clock_count = 0;

    /* Our "clock" operates on a 1ms bit length wereas our "data" operates on
     * a 2ms bit length.
     * We'll be iterating individual milliseconds equivalent to one "data"
     * byte (8 bits @ 2ms per bit = 16ms)
     */
    for (int i = 0; i< 16; i++)
    {
        // Write the clock bit
        if (clock_word & bitmask[clock_count + 4])
            digitalWrite(2, HIGH);
        else
            digitalWrite(2, LOW);

        // Should the data bit be updated in this ms?
        if (i % 2 == 0)
        {
            if (byte & bitmask[i / 2])
                digitalWrite(3, HIGH);
            else
                digitalWrite(3, LOW);
        }

        // Delay for a millisecond
        delay(1);

        // Iterate or roll-over the clock counter
        clock_count++;
        if (clock_count > 3)
            clock_count = 0;
    }
}

void switchSensorUp()
{
    writeByte(0xD7);
    writeByte(0x5F);
    writeByte(0x75);
    writeByte(0xFD);
    writeByte(0xFD);
    writeByte(0xFD);
    writeByte(0xFD);
    writeByte(0x55);
    writeByte(0x56);
}

void switchSensorDown()
{
    writeByte(0xD7);
    writeByte(0x5F);
    writeByte(0x77);
    writeByte(0x7F);
    writeByte(0x7F);
    writeByte(0x7F);
    writeByte(0x7D);
    writeByte(0x55);
    writeByte(0x56);
}

void switchSensorLeft()
{
    writeByte(0xD7);
    writeByte(0x5F);
    writeByte(0x77);
    writeByte(0xDF);
    writeByte(0xDF);
    writeByte(0xDF);
    writeByte(0xDD);
    writeByte(0x55);
    writeByte(0x56);
}

void switchSensorRight()
{
    writeByte(0xD7);
    writeByte(0x5F);
    writeByte(0x77);
    writeByte(0xF7);
    writeByte(0xF7);
    writeByte(0xF7);
    writeByte(0xFD);
    writeByte(0x55);
    writeByte(0x56);
}
