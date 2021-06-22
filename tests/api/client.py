import socket
import sys

if __name__ == "__main__":
    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
    STRING = sys.argv[3]
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect_ex((HOST, PORT))
        msg =STRING+":\n\n"
        s.sendall(bytes(msg, "utf-8"))
        data = s.recv(1024)
        print("Received", repr(data))