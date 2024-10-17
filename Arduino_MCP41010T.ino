
// Define the pins for software SPI
const int DATAOUT = 8;  // MOSI
const int DATAIN  = 9;  // MISO (not used in this example)
const int SPICLOCK = 7; // SCK
const int CHIPSELECT = 10; // CS

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  
  // Set the pin modes
  pinMode(DATAOUT, OUTPUT);
  pinMode(SPICLOCK, OUTPUT);
  pinMode(CHIPSELECT, OUTPUT);
  digitalWrite(CHIPSELECT, LOW); // Ensure CS is high initially

  setPotentiometer(128);         // inital middle wiper value

  // Print an initial message
  Serial.println("Enter a value between 0 and 255 to set the potentiometer:");
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming data as a string
    String input = Serial.readStringUntil('\n');
    // Convert the string to an integer
    int value = input.toInt();

    // Check if the value is within the valid range
    if (value >= 0 && value <= 255) {
      setPotentiometer(value);
      Serial.print("Potentiometer set to: ");
      Serial.println(value);
    } else {
      Serial.println("Invalid value. Please enter a number between 0 and 255.");
    }
  }
}

void setPotentiometer(byte value) {
  // Select the MCP41010
  digitalWrite(CHIPSELECT, HIGH);
  delayMicroseconds(100); // Short delay for clock high time
  // Send the command byte (00010000) followed by the wiper value
  softwareSPITransfer(0x11);  // Command to write to pot register (00010000)
  softwareSPITransfer(value); // Wiper value (0-255)
  // Deselect the MCP41010
  digitalWrite(CHIPSELECT, LOW);
}

void softwareSPITransfer(byte data) {
  for (int i = 0; i < 8; i++) {
    // Write each bit of the data
    digitalWrite(DATAOUT, !!(data & (1 << (7 - i))));
    // Toggle the clock
    digitalWrite(SPICLOCK, HIGH);
    delayMicroseconds(100); // Short delay for clock high time
    digitalWrite(SPICLOCK, LOW);
    delayMicroseconds(100); // Short delay for clock low time
  }
}
