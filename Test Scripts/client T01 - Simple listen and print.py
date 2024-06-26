import socket

def main():
    host = '127.0.0.1' 
    port = 5555  

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()

        print(f"Server listening on {host}:{port}")

        while True:  # Keep listening for new connections indefinitely
            conn, addr = s.accept()
            with conn:
                print(f"Connected by {addr}")
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break
                    print(f"Received message: {data.decode()}")

if __name__ == "__main__":
    main()
