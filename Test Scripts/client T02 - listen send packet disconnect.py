import socket
import threading
import time

def send_special_packet(conn):
    """Sends a special packet after a 30-second delay."""
    time.sleep(30)
    special_packet = b"Flag: H Frequency: 192.168.1.33 Channel: HF"
    try:
        conn.sendall(special_packet)
        print("Special packet sent.")
    except ConnectionResetError:
        print("Connection was closed by the client before the special packet could be sent.")

def receive_file(conn):
    # Read the initial message containing the file size, ending with '\n'
    initial_msg = b""
    while not initial_msg.endswith(b'\n'):
        chunk = conn.recv(1)  # Read one byte at a time to find the '\n'
        if not chunk:
            return  # Connection closed unexpectedly
        initial_msg += chunk

    # Decode the initial message to get the file size
    try:
        initial_msg_decoded = initial_msg.decode().strip()
        command, filesize_str = initial_msg_decoded.split(':')
        if command.lower() != "filesize":
            print("Unexpected initial message format.")
            return
        filesize = int(filesize_str.strip())
    except (ValueError, UnicodeDecodeError) as e:
        print(f"Failed to process the initial message. Error: {e}")
        return

    # Receive the file based on the specified size
    received_bytes = 0
    file_data = bytearray()
    while received_bytes < filesize:
        chunk = conn.recv(min(1024, filesize - received_bytes))
        if not chunk:
            break  # Connection closed
        file_data.extend(chunk)
        received_bytes += len(chunk)
    
    # Assuming the file data is received successfully, send acknowledgment
    conn.sendall(b"Flag: A\n")

    # Listen for additional text packets
    print("Picture received. Listening for additional packets...")
    while True:
        packet = conn.recv(1024)
        if not packet:
            break
        print(f"Packet received: {packet.decode()}")

def handle_connection(conn, addr):
    print(f"Connected by {addr}")
    # Start a thread to send the special packet after 30 seconds
    threading.Thread(target=send_special_packet, args=(conn,)).start()
    
    # Handle receiving the file and any subsequent packets
    receive_file(conn)
    print("Connection closed.")

def main():
    host = '127.0.0.1'
    port = 5555

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print(f"Server listening on {host}:{port}")

        while True:
            conn, addr = s.accept()
            threading.Thread(target=handle_connection, args=(conn, addr)).start()

if __name__ == "__main__":
    main()
