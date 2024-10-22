const int MySerialRX = 16;
const int MySerialTX = 17;

void setup() 
{
	// initialize the Serial to the pins
    Serial2.begin(9600, SERIAL_8N1, MySerialRX, MySerialTX);
    Serial.begin(9600);
}

void loop() 
{
    uint8_t one = 1;
    uint8_t two = 2;
    Serial.print("sending 2");
    Serial2.write(0b10);
    delay(1000);
    Serial.print("sending 1");
    Serial2.write(0b1);
    delay(1000);
}
