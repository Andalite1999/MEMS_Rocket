import asyncio
from bleak import BleakScanner


async def run():
    devices = await BleakScanner.discover()
    for device in devices:
        print(f"Device {device.name} with address {device.address}")

if __name__ == "__main__":
    asyncio.run(run())
