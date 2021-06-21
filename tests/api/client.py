import socket
import sys

if __name__ == "__main__":
    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect_ex((HOST, PORT))
        s.sendall(b'TEST000001:\n\n')
        data = s.recv(1024)
        print("Received", repr(data))
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect_ex((HOST, PORT))
        s.sendall(b'TEST000002:5000\n\n')
        data = s.recv(1024)
        print("Received", repr(data))
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect_ex((HOST, PORT))
        s.sendall(b'ECHO000001:ping\n\n')
        data = s.recv(1024)
        print("Received", repr(data))