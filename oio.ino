int testPin = 2;
int fl5Pin = 3;
int p1PadPin[4] = {5, 6, 7, 8};
int p2PadPin[4] = {9, 10, 11, 12};
int neonPin = 13;

byte pcData[4];
byte crc;

void readData(){
    for (int i=0;i<4;i++)
        pcData[i] = Serial.read();   
}

void padLights(){
    // The first byte is p1, second p2. We also need to remove the 0x80 header
    byte dataP1 = (pcData[0] - 0x80);
    byte dataP2 = pcData[1];
    byte dataNeon = pcData[2];
    // This will be the array which tells us what light to turn on
    // Up, Down, Left, Right
    bool lightsP1[4] = {false, false, false, false};
    bool lightsP2[4] = {false, false, false, false};
    bool lightsNeon = false;
    /*
     *  This next part picks apart the byte to see what arrow needs to be lit
     *  
     *  Up      0x40
     *  Down    0x20
     *  Left    0x10
     *  Right   0x08
     */
    
    // Player 1 *******************************

    if (dataP1 >= 0x40) {
        lightsP1[0] = true;
        dataP1 = (dataP1 - 0x40);
    }
    else
        lightsP1[0] = false;
    if (dataP1 >= 0x20) {
        lightsP1[1] = true;
        dataP1 = (dataP1 - 0x20);
    }
    else  
        lightsP1[1] = false; 
    if (dataP1 >= 0x10) {
        lightsP1[2] = true;
        dataP1 = (dataP1 - 0x10);
    }
    else  
        lightsP1[2] = false; 
    if (dataP1 >= 0x08) {
        lightsP1[3] = true;
        dataP1 = (dataP1 - 0x08);
    }
    else  
        lightsP1[3] = false;   

    // Player 2 ********************************

    if (dataP2 >= 0x40) {
        lightsP2[0] = true;
        dataP2 = (dataP2 - 0x40);
    }
    else
        lightsP2[0] = false;
    if (dataP2 >= 0x20) {
        lightsP2[1] = true;
        dataP2 = (dataP2 - 0x20);
    }
    else
        lightsP2[1] = false;
    if (dataP2 >= 0x10) {
        lightsP2[2] = true;
        dataP2 = (dataP2 - 0x10);
    }
    else
        lightsP2[2] = false;
    if (dataP2 >= 0x08) {
        lightsP2[3] = true;
        dataP2 = (dataP2 - 0x08);
    }
    else
        lightsP2[3] = false;

    // Neon ****************************************
    if (dataNeon >= 0x40) {
        lightsNeon = true;
        dataNeon = (dataNeon - 0x40); 
    }
    else
        lightsNeon = false; 

    // Now lets turn the lights on and off    
    for (x=0;x<4;x++){
        digitalWrite(p1PadPin[x], lightsP1[x]);
        digitalWrite(p2PadPin[x], lightsP2[x]);
    }
    digitalWrite(neonPin, lightsNeon);
}


void sensorMode(){
    // Grabbing the 3rd byte
    byte mode = pcData[2];

    /*
     *  This next part picks apart the byte to determine which mode to set the Pad-IO to
     *
     *  Up      0x28
     *  Down    0x20
     *  Left    0x18
     *  Right   0x10
     *  All     0x08
     *
     */
    
    // Tear off neon data if it is there
    if (mode >= 0x40)
        mode = (mode - 0x40);

    switch (mode) {
        case 0x28:
            //Up Sensor
            //padModeUp();
        case 0x20:
            //Down Sensor
            //padModeDown();
        case 0x18:
            //Left Sensor
            //padModeLeft();
        case 0x10:
            //Right Sensor
            //padModeRight();
        case 0x08:
            //All Sensors
            //padModeAll();
        default:
            break;
}

void init(){
    // The EXT-IO spams 0x22 until it gets something from the PC
    while (!Serial.available())
        Serial.write(0x22);
}

    

void setup(){
    // Pad-IO outputs
    pinMode(testPin, OUTPUT);
    pinMode(fl5Pin, OUTPUT);
    // Main communication to the PC
    Serial.begin(38400);

    // Wait for the PC to send its first request
    init(); 
}

void loop(){
    // Wait for some serial data and store the 4 bytes we need
    while (!Serial.available()){}
    readData();
    // Lets compare the checksum to make sure we have correct data
    crc = ((data[0] + data[1] + data[2] - 0x80) & 0xFF);
    if (crc == data[3]) {
        padLights();
        sensorMode();        
        //Tell the PC that it has completed it's task
        Serial.write(0x11);
    }   
}
