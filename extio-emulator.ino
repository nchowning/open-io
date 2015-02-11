// FL1, FL2, FL3, FL4, FL5, TEST
int p1PadPin[6] = {2, 3, 4, 5, 6, 7};
int p2PadPin[6] = {8, 9, 10, 11, 12, 13};
int neonPin = 22;

// bitmask for each bit in a byte
unsigned char bitmask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
// Word of clock data (1ms bit length)
unsigned char clock_word = 0x07;

unsigned char pcData[4];
unsigned char crc;

void lights(unsigned char, unsigned char, unsigned char);
void sensorMode(unsigned char);
void writeByte(unsigned char);
void switchSensorUp();
void switchSensorDown();
void switchSensorLeft();
void switchSensorRight();

void setup()
{
    // Pad Light & IO outputs
    for (int i = 0; i < sizeof(p1PadPin); i++)
    {
        pinMode(p1PadPin[i], OUTPUT);
        pinMode(p2PadPin[i], OUTPUT);

        // Set the lights to be on at the beginning
        for (int i = 0; i < 4; i++)
        {
            digitalWrite(p1PadPin[i], LOW);
            digitalWrite(p2PadPin[i], LOW);
        }

        // FL5 should be LOW and TEST should be HIGH by default
        digitalWrite(p1PadPin[4], LOW);
        digitalWrite(p1PadPin[5], HIGH);
        digitalWrite(p2PadPin[4], LOW);
        digitalWrite(p2PadPin[5], HIGH);
    }

    // Neon light output
    pinMode(neonPin, OUTPUT);

    // Main communication to bemaniPC or python2
    Serial.begin(38400);
}

void loop()
{
    // Wait for some serial data and store the 4 bytes we need
    for (int i = 0; i < 4; i++)
    {
        while (!Serial.available()){}
        pcData[i] = Serial.read();
    }
    // Compare the checksum to make sure we have correct data
    crc = ((pcData[0] + pcData[1] + pcData[2]) & 0xFF);
    if (crc == pcData[3] || (crc & 0x7F) == pcData[3])
    {
        // We have to drop the first bit of p1
        lights(pcData[0] & 0x7F, pcData[1], pcData[2]);

        // Dropping the first two bits & passing the rest
        sensorMode(pcData[2] & 0x3F);

        // Tell the PC/Python2 that we've completed our tas
        Serial.write(0x11);
    }
    else
    {
        // TODO figure out how to "gracefully" fail
        // We have a problem
    }
}

void lights(unsigned char p1, unsigned char p2, unsigned char neon)
{
    /* 
     * In the p1 & p2 data, the following bits correspond to arrow lights
     *
     * Up     0x40 (B01000000)
     * Down   0x20 (B00100000)
     * Left   0x10 (B00010000)
     * Right  0x08 (B00001000)
     */
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(p1PadPin[i], !(p1 & bitmask[i + 1]));
        digitalWrite(p2PadPin[i], !(p2 & bitmask[i + 1]));
    }

    /*
     * In the neon data, 0x40 (B01000000) enables or disables neon lights
     */
    digitalWrite(neonPin, !(neon & bitmask[1]));
}

void sensorMode(unsigned char sensor)
{
    switch (sensor)
    {
        case 0x10:
            // Up Sensor
            switchSensorUp();
            break;
        case 0x18:
            // Down Sensor
            switchSensorDown();
            break;
        case 0x20:
            // Left Sensor
            switchSensorLeft();
            break;
        case 0x28:
            // Right Sensor
            switchSensorRight();
            break;
        case 0x08:
            // All Sensors
            break;
    }
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
        // Write the clock bit to TEST pin
        digitalWrite(p1PadPin[5], clock_word & bitmask[clock_count + 4]);
        digitalWrite(p2PadPin[5], clock_word & bitmask[clock_count + 4]);

        // Should the data bit be updated in this ms?
        if (i % 2 == 0)
        {
            // Write the data bit to the FL5 pin
            digitalWrite(p1PadPin[4], byte & bitmask[i / 2]);
            digitalWrite(p2PadPin[4], byte & bitmask[i / 2]);
        }

        // Delay for a millisecond
        delay(1);

        // Iterate or roll-over the clock counter
        clock_count++;
        if (clock_count > 3)
            clock_count = 0;
    }
}

/*
 *  TODO: Figure out when it switches between default off and new mode
 */

void switchSensorUp()
{
    unsigned char bytes[] = {0xD7, 0x5F, 0x75, 0xFD, 0xFD, 0xFD, 0xFD, 0x55, 0x56};
    for (int i; i < sizeof(bytes); i++)
        writeByte(bytes[i]);
}

void switchSensorDown()
{
    unsigned char bytes[] = {0xD7, 0x5F, 0x77, 0x7F, 0x7F, 0x7F, 0x7D, 0x55, 0x56};
    for (int i; i < sizeof(bytes); i++)
        writeByte(bytes[i]);
}

void switchSensorLeft()
{
    unsigned char bytes[] = {0xD7, 0x5F, 0x77, 0xDF, 0xDF, 0xDF, 0xDD, 0x55, 0x56};
    for (int i; i < sizeof(bytes); i++)
        writeByte(bytes[i]);
}

void switchSensorRight()
{
    unsigned char bytes[] = {0xD7, 0x5F, 0x77, 0xF7, 0xF7, 0xF7, 0xFD, 0x55, 0x56};
    for (int i; i < sizeof(bytes); i++)
        writeByte(bytes[i]);
}

void switchSensorAll()
{
    // TODO figure this one out
}
