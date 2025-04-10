import bluetooth

def scan_devices():
    print("Scanning for Bluetooth devices...")
    devices = bluetooth.discover_devices(duration=8, lookup_names=True, flush_cache=True)
    if devices:
        print("Found devices:")
        for addr, name in devices:
            print(f"  {addr} - {name}")
    else:
        print("No Bluetooth devices found.")
    return devices

def main():
    # Option 1: If you know the MAC address, hard-code it:
    # server_mac_address = "XX:XX:XX:XX:XX:XX"  # replace with your ESP32's MAC address

    # Option 2: Scan for devices to find the ESP32:
    devices = scan_devices()
    server_mac_address = None
    for addr, name in devices:
        if "ESP32_BT" in name:  # or match a part of your device name
            server_mac_address = addr
            break
    if server_mac_address is None:
        print("ESP32_BT not found. Please pair your ESP32 and try again.")
        return

    port = 1  # SPP typically uses RFCOMM port 1

    # Create an RFCOMM Bluetooth socket and connect
    sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    try:
        print(f"Connecting to ESP32 at {server_mac_address} on port {port}...")
        sock.connect((server_mac_address, port))
        print("Connected to ESP32_BT")

        # Send a test message
        message = "Hello from Python via BT!"
        sock.send(message)
        print("Sent:", message)

        # Receive a reply (this will block until data is received)
        data = sock.recv(1024)
        print("Received:", data.decode('utf-8'))
    except bluetooth.btcommon.BluetoothError as err:
        print("Bluetooth Error:", err)
    finally:
        sock.close()
        print("Connection closed.")

if __name__ == '__main__':
    main()
