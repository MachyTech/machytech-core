import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import json
import socket
from queue import Queue
import threading
import time
import sys
import math

#HOST =  '127.0.0.1' # localhost
#PORT = 8085

# width and height of the projection
WIDTH = 0.2/2
HEIGHT = 0.2
# number of timesteps that are projects
T = 100
#Select drawfunction
switch = 1        # 0 = line, 1 = arrow, 2 = circle

def Arrowheadfun(new_coordx, new_coordy, Varray, arrowsize=10, arrowwidth=0.01):
    arrowsize=-int(arrowsize)

    #Create Arrowhead
    diffx=new_coordx[arrowsize]-new_coordx[-1]
    diffy=new_coordy[arrowsize]-new_coordy[-1]
    PerpendicularRC = -diffx/diffy
    B = new_coordy[arrowsize]-PerpendicularRC*new_coordx[arrowsize]

    Y0 = new_coordy[-1]
    Y1 = new_coordy[arrowsize]-arrowwidth
    Y2 = new_coordy[arrowsize]+arrowwidth
    Y3 = new_coordy[-1]

    X0 = new_coordx[-1]
    X1 = (Y1-B)/PerpendicularRC
    X2 = (Y2-B)/PerpendicularRC
    X3 = new_coordx[-1]

    V0 = Varray[-1]
    V1 = Varray[arrowsize]
    V2 = Varray[arrowsize]
    V3 = Varray[-1]

    Xarrow = [X0, X1, X2, X3]
    Yarrow = [Y0, Y1, Y2, Y3]
    Varrow = [V0, V1, V2, V3]

    return Xarrow, Yarrow, Varrow

class sockets(threading.Thread):
    def __init__(self, host, port, q1, q2):
        threading.Thread.__init__(self)
        self.host = host
        self.port = port
        self.q1 = q1
        self.q2 = q2
    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.host, self.port))
            s.listen()
            self.conn, self.addr = s.accept()
            with self.conn:
                while True:
                    print('connected by', self.addr)
                    # let simulation know we connected
                    self.q1.put("connected")
                    self.data_client = self.conn.recv(4096)
                    if not self.data_client:
                        break

                    # get the data from the simulation
                    self.data = self.q2.get()

                    # send data over the socket
                    self.data_x = self.data[0]
                    self.data_y = self.data[1]
                    self.data_v = self.data[2]

                    # json format
                    self.data_json = json.dumps({"x": self.data_x.tolist(), "y": self.data_y.tolist(), "v": self.data_v.tolist()})
                    # send json array
                    self.conn.send(self.data_json.encode())
            self.conn.close()

class rest(threading.Thread):
    def __init__(self, x, y, q1, q2):
        threading.Thread.__init__(self)
        self.q1 = q1
        self.q2 = q2

        self.x = x
        self.y = y
    def run(self):
            # check if a request is send
            while not self.q1.empty():
                msg = self.q1.get()
                data = [ self.x, self.y, 1.0 ]
                print(data)
                self.q2.put(data)


class simulation(threading.Thread):
    def __init__(self,  width, height, t, q1, q2):
        threading.Thread.__init__(self)
        self.data = pd.read_csv("trajectories/trajectory_1_fpg_out.txt", skiprows=11)
        self.width = width
        self.height = height
        self.t = t
        self.q1 = q1
        self.q2 = q2

        self.theta = 0
        self.angle = self.data["yaw_angle"]
        self.velocity = self.data["velocity"]
        self.x_robot = self.data["x_out"]
        self.y_robot = self.data["y_out"]
        self.time = self.data["time"]
        self.kernel = np.array([[0, self.width],[self.height, self.width], [self.height, -self.width], [0, -self.width], [0, self.width]])


        print("[message] Initializing simulation...")
        print("[message] Showing data type:")
        print(self.data.shape)
        print(self.data.ndim)
        print(self.data)

    def run(self):
        for i in range(len(self.x_robot)-1):
            #print("Iteration: ", i)
            # read current angle and create the rotation matrix
            self.theta = self.angle[i]
            self.rot_mat = np.array([[np.cos(self.theta), np.sin(self.theta)],[-np.sin(self.theta), np.cos(self.theta)]])

            # rotate the kernel to the robot-frame
            self.kernel_rot = np.matmul(self.kernel, self.rot_mat) + [self.x_robot[i], self.y_robot[i]]

            # read the coordinate of the 't' following time steps (future path)
            self.coord = np.array([self.x_robot[1+i:self.t+i], self.y_robot[1+i:self.t+i]])

            # create the rotation matrix again
            self.theta = -self.theta
            self.rot_mat = np.array([[np.cos(self.theta), np.sin(self.theta)],[-np.sin(self.theta), np.cos(self.theta)]])

            # rotate the coordinates to the robot-frame
            self.rot_coord = np.matmul(np.transpose(self.coord), self.rot_mat)

            # get the velocity data
            self.v_array = np.array(self.velocity[1+i:i+self.t])


            # check if a request is send
            while not self.q1.empty():
                msg = self.q1.get()
                self.new_coordx = np.transpose(self.rot_coord)[0]-np.transpose(self.rot_coord)[0][0]
                self.new_coordy = np.transpose(self.rot_coord)[1]-np.transpose(self.rot_coord)[1][0]
                if switch == 1:
                    self.Arrowheadx, self.Arrowheady, self.Varrow = Arrowheadfun(self.new_coordx, self.new_coordy, self.v_array, self.t/4, 0.03)
                    self.new_coordx = np.append(self.new_coordx, self.Arrowheadx)
                    self.new_coordy = np.append(self.new_coordy, self.Arrowheady)
                    self.v_array = np.append(self.v_array, self.Varrow)

                data = [ self.new_coordx, self.new_coordy, np.transpose(self.v_array) ]
                # print(data)
                self.q2.put(data)
                #input("press any key")
            # get accurate timing
            delta_time = self.time[i+1] - self.time[i]
            time.sleep(delta_time)
        print("[message] simulation is finished...")


if __name__ == "__main__":
    # get command line input
    print("Number of arguments: ", len(sys.argv), "arguments\n")
    print("Argument List: ", str(sys.argv))
    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
    # initiate request (Q1) and message (Q2) queue
    request_queue = Queue()
    msg_queue = Queue()
    # creating objects of classes
    socket_programming = sockets(HOST, PORT, request_queue, msg_queue)
    path_simulation = simulation(WIDTH, HEIGHT, T, request_queue, msg_queue)

    # Start new threads
    socket_programming.start()
    path_simulation.start()

    path_simulation.join()
    x_data = np.array([0.0, 0.5, 0.5, 0.0])
    y_data = np.array([0.0, 0.0, 0.5, 0.5])
    rest_projection = rest(x_data, y_data, request_queue, msg_queue)
    rest_projection.start()

    time.sleep(20)
    print("[message] starting next trajectory")

    # Start new threads
    path_simulation_1 = simulation(WIDTH, HEIGHT, T, request_queue, msg_queue)
    path_simulation_1.start()
