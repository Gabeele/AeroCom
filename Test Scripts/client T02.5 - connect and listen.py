import socket

def receive_file_from_client(connection):
    # Receive the initial file size message
    size_message = b""
    while not size_message.endswith(b'\n'):
        chunk = connection.recv(1)  # Receive byte by byte until '\n'
        if not chunk:
            print("Connection closed by the client.")
            return
        size_message += chunk
    
    # Decode the file size message and extract the size
    file_size = int(size_message.decode().rstrip().split(':')[1])
    print(f"Expecting a file of size: {file_size} bytes")
    
    # Receive the file based on the size
    received_bytes = 0
    file_data = bytearray()
    while received_bytes < file_size:
        chunk = connection.recv(min(1024, file_size - received_bytes))
        if not chunk:
            break
        file_data.extend(chunk)
        received_bytes += len(chunk)
    

     with open("received_file.png", "wb") as file:
         file.write(file_data)
    
    print(f"Received file of size: {len(file_data)} bytes")
    
    # Send acknowledgment back to the client
    connection.sendall(b"Flag: A\n")
    print("Acknowledgment sent to the client.")
    
    # Continue listening for more packets
    while True:
        packet = connection.recv(1024)
        if not packet:
            print("Client disconnected.")
            break
        print(f"Received packet: {packet}")

def start_server(host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((host, port))
        server_socket.listen()
        print(f"Server listening on {host}:{port}")
        
        while True:
            connection, client_address = server_socket.accept()
            with connection:
                print(f"Connected to {client_address}")
                receive_file_from_client(connection)

if __name__ == "__main__":
    HOST = '192.168.1.33'  # Server's IP address
    PORT = 4444  # Port to listen on
    start_server(HOST, PORT)
