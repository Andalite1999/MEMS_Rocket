// #include "QSPIFBlockDevice.h"
// #include "MBRBlockDevice.h"

// using namespace mbed;

// #define BLOCK_DEVICE_SIZE 1024 * 8 // 8 KB
// #define PARTITION_TYPE 0x0B // FAT 32

// void setup() {
//   Serial.begin(115200);
//   while (!Serial);

//   Serial.println("QSPI Block Device Test");
//   Serial.println("------------------------");  

//   // Feed the random number generator for later content generation
//   randomSeed(analogRead(0));

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
  
//   const auto eraseBlockSize = blockDevice.get_erase_size();
//   const auto programBlockSize = blockDevice.get_program_size();

//   Serial.println("Block device size: " + String((unsigned int) blockDevice.size() / 1024) + " KB");  
//   Serial.println("Readable block size: " + String((unsigned int) blockDevice.get_read_size())  + " bytes");
//   Serial.println("Programmable block size: " + String((unsigned int) programBlockSize) + " bytes");
//   Serial.println("Erasable block size: " + String((unsigned int) eraseBlockSize / 1024) + " KB");
     
//   String newMessage = "Random number: " + String(random(1024));
  
//   // Calculate the amount of bytes needed to store the message
//   // This has to be a multiple of the program block size
//   const auto messageSize = newMessage.length() + 1; // C String takes 1 byte for NULL termination
//   const unsigned int requiredEraseBlocks = ceil(messageSize / (float)  eraseBlockSize);
//   const unsigned int requiredBlocks = ceil(messageSize / (float)  programBlockSize);
//   const auto dataSize = requiredBlocks * programBlockSize;  
//   char buffer[dataSize] {};  

//   // Read back what was stored at previous execution  
//   Serial.println("Reading previous message...");
//   blockDevice.read(buffer, 0, dataSize);
//   Serial.println(buffer);

//   // Erase a block starting at the offset 0 relative
//   // to the block device start address
//   blockDevice.erase(0, requiredEraseBlocks * eraseBlockSize);

//   // Write an updated message to the first block
//   Serial.println("Writing new message...");
//   Serial.println(newMessage);  
//   blockDevice.program(newMessage.c_str(), 0, dataSize);

//   // Deinitialize the device
//   blockDevice.deinit();
//   Serial.println("Done.");
// }

// void loop() {}