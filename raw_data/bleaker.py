
import asyncio
from bleak import BleakScanner, BleakClient

service_uuid = "12345678-1234-1234-1234-123456789012"
characteristic_uuid = "87654321-4321-4321-4321-210987654321"
device_name = "Arduino"  # Device name to connect to


async def connect_to_device_by_name(device_name):
    print(f"Scanning for the device with name '{device_name}'...")
    devices = await BleakScanner.discover()
    for device in devices:
        if device.name == device_name:
            print(f"Found {device_name} with address {device.address}")
            return device.address
    return None


async def run(device_name, service_uuid, characteristic_uuid):
    device_address = await connect_to_device_by_name(device_name)
    if device_address is not None:
        async with BleakClient(device_address) as client:
            # Connect to the device.
            await client.connect()
            print(f"Connected to {device_name} at address {device_address}")

            # Loop to send input received from user to the characteristic.
            while True:
                # Get user input
                user_input = input(
                    "Enter a command to send to the device (type 'exit' to quit): ")
                if user_input.lower() == 'exit':
                    print("Exiting input loop.")
                    break

                # Send the input as bytes to the characteristic
                if user_input:
                    await client.write_gatt_char(characteristic_uuid, user_input.encode())
                    print(f"Sent command: {user_input}")

            # Disconnect from the device
            await client.disconnect()
    else:
        print(f"Could not find a device with the name '{device_name}'.")

if __name__ == "__main__":
    asyncio.run(run(device_name, service_uuid, characteristic_uuid))
