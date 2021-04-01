import pandas as pd
import matplotlib.pyplot as plt 
import numpy as np
import json
import socket
from queue import Queue
import threading
import time
import sys


if __name__ == "__main__":
    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        while s.connect_ex((HOST, PORT)) != 0:
            print("-not connected-")
            time.sleep(1)
        print("yay! connected!")
        while(1):
            s.sendall(b'Hello, world')
            data = s.recv(1024)
            print("Received", repr(data))
            time.sleep(1)