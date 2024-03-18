// #include <Adafruit_SPIFlash.h>
// #include <SPI.h>
// // #include <Adafruit_TinyUSB.h> // For USB serial
// // Include your Bluetooth library here

// // Assuming you're using a generic SPI flash chip
// // #define FLASH_CS   SS
// // #define FLASH_SCK  SCK
// // #define FLASH_MISO MISO
// // #define FLASH_MOSI MOSI
// #define BLOCK_DEVICE_SIZE 1024 * 8 // 8 KB
// #define PARTITION_TYPE 0x0B // FAT 32

// Adafruit_FlashTransport_SPI flashTransport(FLASH_CS, FLASH_SCK, FLASH_MISO, FLASH_MOSI);
// Adafruit_SPIFlash flash(&flashTransport);
// bool collect_data = false;
// float collect_time_start;
// int time_limit = 20;

// // Bluetooth Serial object
// // Replace with your Bluetooth library's serial object
// BluetoothSerial SerialBT;

// void data_handler() {
//   // trigger based on time or absolute acceleration?
//   double thresh = 5;
//   if (sqrt(accel_x*accel_x + accel_y*accel_y + accel_y*accel_y)) > thresh) {
//     collect_data = true;
//     collect_time_start = currentTime;
//   }
//   float collect_time_total = currentTime - collect_time_start;
//   if (collect_data && collect_time_total < time_limit*1000) {
//     write_snapshot();
//   }
//   //stop collecting data after a certain amount of time to prevent overflow
//   //could it trigger fast enough on high g-loads?  That prevents a lot of issues involved with 
// }

// void write_snapshot() {
//   // https://docs.arduino.cc/tutorials/portenta-h7/reading-writing-flash-memory/
// }

// void init_bluetooth() {
//   Serial.begin(115200);
//   while (!Serial) { delay(10); } // for native USB port only

//   // Initialize flash library
//   if (!flash.begin()) {
//     Serial.println("Could not find a valid flash chip");
//     while (1) delay(10);
//   }

//   // Initialize Bluetooth
//   SerialBT.begin("ArduinoRocket"); // Bluetooth device name
//   Serial.println("The device started, now you can pair it with bluetooth!");

//   // Add your code to initialize QSPI flash reading here
// }

// void t_bluetooth() {
//   // Wait for Bluetooth connection
//   if (SerialBT.hasClient()) {
//     // Once a Bluetooth device is connected, start transmitting data
//     Serial.println("Device connected via Bluetooth");

//     // Example: Reading and transmitting data from flash
//     uint8_t buf[256]; // Adjust buffer size as needed
//     uint32_t addr = 0; // Start address for reading
//     while (addr < flash.size()) {
//       // Read a chunk of data from flash
//       flash.readBuffer(addr, buf, sizeof(buf));
//       // Transmit data over Bluetooth
//       SerialBT.write(buf, sizeof(buf));
//       // Increment address
//       addr += sizeof(buf);
//     }

//     Serial.println("Data transmission complete");
//     // Wait a bit before next check
//     delay(1000);
//   }
// }

// void data_init() {
  

//   // Create a block device on the available space of the flash
//   QSPIFBlockDevice root(PD_11, PD_12, PF_7, PD_13,  PF_10, PG_6, QSPIF_POLARITY_MODE_1, 40000000);
//   MBRBlockDevice blockDevice(&root, 1);  

//   // Initialize the Flash IAP block device and print the memory layout
//   if(blockDevice.init() != 0 || blockDevice.size() != BLOCK_DEVICE_SIZE) {    
//     Serial.println("Partitioning block device...");
//     blockDevice.deinit();
//     // Allocate a FAT 32 partition
//     MBRBlockDevice::partition(&root, 1, PARTITION_TYPE, 0, BLOCK_DEVICE_SIZE);
//     blockDevice.init();
//   }
// }