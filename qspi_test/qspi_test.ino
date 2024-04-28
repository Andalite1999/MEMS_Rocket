#include "QSPIFBlockDevice.h"
#include "MBRBlockDevice.h"
#include "BluetoothSerial.h"  // Include the BluetoothSerial library

using namespace mbed;
// BluetoothSerial btSerial; // Create a BluetoothSerial object

#define BLOCK_DEVICE_SIZE 1024 * 8  // 8 KB
// #define BLOCK_DEVICE_SIZE 1024 * 8 * 1024 // 8 MB
#define PARTITION_TYPE 0x0B  // FAT 32

float accel_x, accel_y, accel_z = 0;
float g_x, g_y, g_z = 0;
double twist_x, twist_y, twist_z = 0;
double yaw_corr_1s = .65;             //subtract yaw by this value every 1000 micro
double pitch = 0, roll = 0, yaw = 0;  // Euler angles
unsigned long lastUpdateTime = 0;     // Last update time in microseconds
unsigned long currentTime = 0;
float deltaTime = 0;
unsigned long collection_limit = 10 * 1000;  //5 seconds
long collection_start_time;
QSPIFBlockDevice root(PD_11, PF_9, PE_2, PD_13, PF_10, PG_6, QSPIF_POLARITY_MODE_0, 40000000);
MBRBlockDevice blockDevice(&root, 1);
int dataSize;
bool debug = false;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  // yaw_corr_1s = 0;
  // setupIMU();
  // setupServos();
  lastUpdateTime = millis();

  Serial.println("QSPI Block Device Test");
  Serial.println("------------------------");

  // Feed the random number generator for later content generation
  randomSeed(analogRead(0));

  // Create a block device on the available space of the flash
  // QSPIFBlockDevice root(PD_11, PD_12, PF_7, PD_13,  PF_10, PG_6, QSPIF_POLARITY_MODE_1, 40000000);


  // Initialize the Flash IAP block device and print the memory layout
  if (blockDevice.init() != 0 || blockDevice.size() != BLOCK_DEVICE_SIZE) {
    Serial.println("Partitioning block device...");
    blockDevice.deinit();
    // Allocate a FAT 32 partition
    MBRBlockDevice::partition(&root, 1, PARTITION_TYPE, 0, BLOCK_DEVICE_SIZE);
    blockDevice.init();
  }
  collection_start_time = millis();
  //BEGIN PROGRAM HERE

  loop_demo(blockDevice);
  Serial.println("Finished with loop");
  // Serial.println("Done with init.");
  // readRandomNumbersFromQPSI(blockDevice, 3);
  // clearQSPIMemory(blockDevice);
  // writeRandomNumbersToQSPI(blockDevice, 3);
  // sendDataFromQSPIOverBluetooth(blockDevice);
  // Deinitialize the device
  blockDevice.deinit();
}

void writeRandomNumbersToQSPI(MBRBlockDevice& blockDevice, size_t count, auto dataSize) {
  String newMessage = "Random number: " + String(random(1024));
  // size_t dataSize = blockDevice.get_program_size(); // Get the program block size
  char buffer[dataSize];  // Buffer to hold data to be written

  for (size_t i = 0; i < count; i++) {
    // Generate a random number and convert it to a string
    // String number = String(random(1024));
    String message = "Pitch: " + String(random(1024)) + " Roll: " + String(random(1024)) + " Yaw: " + String(random(1024)) + " Time: " + String(random(1024));
    message.toCharArray(buffer, dataSize);
    Serial.print("Written message ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(message);
    // Write the number to the QSPI memory
    blockDevice.program(buffer, i * dataSize, dataSize);
  }
  Serial.println("Done writing random numbers");
}

void readRandomNumbersFromQSPI(MBRBlockDevice& blockDevice, size_t count, auto dataSize) {
  // size_t dataSize = blockDevice.get_program_size(); // Get the program block size
  char buffer[dataSize];  // Buffer to hold data to be written
  for (size_t i = 0; i < count; i++) {
    // Generate a random number and convert it to a string
    // String message;
    // number.toCharArray(buffer, dataSize);
    // Write the number to the QSPI memory
    blockDevice.read(buffer, i * dataSize, dataSize);
    // buffer.toNumber(number, dataSize)
    Serial.print("Read message ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(buffer);
  }
  Serial.println("Done reading random numbers");
}

void clearQSPIMemory(MBRBlockDevice& blockDevice) {
  blockDevice.erase(0, blockDevice.size());
  Serial.println("Blocks cleared");
}

void loop_demo(MBRBlockDevice& blockDevice) {
  String lineEntry = "Pitch: " + String(random(1024)) + "Roll: " + String(random(1024)) + "Yaw: " + String(random(1024)) + "Time: " + String(random(1024));
  const auto eraseBlockSize = blockDevice.get_erase_size();
  const auto programBlockSize = blockDevice.get_program_size();
  const auto padding = 10;
  // const auto messageSize = lineEntry.length() + padding + 1;  // C String takes 1 byte for NULL termination
  const auto messageSize = 50; //50 bytes seems to be the ideal
  Serial.print("Message Size: ");
  Serial.print(messageSize);
  Serial.println(" bytes");
  const unsigned int requiredEraseBlocks = ceil(messageSize / (float)eraseBlockSize);
  const unsigned int requiredBlocks = ceil(messageSize / (float)programBlockSize);
  dataSize = requiredBlocks * programBlockSize;
  char buffer[dataSize]{};

  Serial.println("Block device size: " + String((unsigned int)blockDevice.size() / 1024) + " KB");
  Serial.println("Readable block size: " + String((unsigned int)blockDevice.get_read_size()) + " bytes");
  Serial.println("Programmable block size: " + String((unsigned int)programBlockSize) + " bytes");
  Serial.println("Erasable block size: " + String((unsigned int)eraseBlockSize / 1024) + " KB");

  if (debug){
    readRandomNumbersFromQSPI(blockDevice, 5, dataSize);
    clearQSPIMemory(blockDevice);
    writeRandomNumbersToQSPI(blockDevice, 5, dataSize);
  }
}

void loop() {
  if (!debug) {
    if (Serial.available() > 0) {
      String command = Serial.readString();
        if (command == "send\n") {
        readRandomNumbersFromQSPI(blockDevice, 5, dataSize);
        Serial.println("spirou");
        clearQSPIMemory(blockDevice);
        writeRandomNumbersToQSPI(blockDevice, 5, dataSize);
        // Serial.println("done");
      } else {
        Serial.println("Failed to send data");
      }    
    }
    delay(1000);
    // Serial.println("nothing sent in");
    }  
}