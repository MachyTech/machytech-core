import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# reading csv file
data = pd.read_csv("trajectories/trajectory_1_fpg_out.txt", skiprows= 11)

# USER VARIABLES
WIDTH = 0.2 / 2
HEIGHT = 0.2
t = 40

# show shape
print(data)
print(data.shape)
print(data.ndim)

# assign memory
theta = 0
angle = data["yaw_angle"]
velocity = data["velocity"]
x_robot = data["x_out"]
y_robot = data["y_out"]
plt.plot(data["x_out"], data["y_out"])
kernel = np.array([[0, WIDTH],[HEIGHT, WIDTH], [HEIGHT, -WIDTH], [0, -WIDTH], [0, WIDTH]])

#rot_mat = np.array([[np.cos(theta), np.sin(theta)],[-np.sin(theta), np.cos(theta)]])
print(x_robot, y_robot)
for i in range(len(x_robot)):
    # read current angle and create the rotation matrix
    theta = angle[i]
    rot_mat = np.array([[np.cos(theta), np.sin(theta)],[-np.sin(theta), np.cos(theta)]])
    
    # rotate the kernel to the robot-frame
    kernel_rot = np.matmul(kernel, rot_mat) + [x_robot[i], y_robot[i]]
    # read the coordinate of the 't' following time steps
    coord = np.array([x_robot[1+i:t+i], y_robot[1+i:t+i]])

    # create the rotation matrix again
    theta = -theta
    rot_mat = np.array([[np.cos(theta), np.sin(theta)],[-np.sin(theta), np.cos(theta)]])
    # rotate the coordinates to the robot-frame
    rot_coord = np.matmul(np.transpose(coord), rot_mat)

    # show the state of the robot-frame and projection on a certain time point
    if i == 350:
        print(rot_coord)
        plt.plot([kernel_rot[0][0], kernel_rot[1][0], kernel_rot[2][0], kernel_rot[3][0], kernel_rot[4][0]],[kernel_rot[0][1], kernel_rot[1][1], kernel_rot[2][1], kernel_rot[3][1], kernel_rot[4][1]])
        plt.plot(coord[0], coord[1])
        plt.show()
        plt.plot(np.transpose(rot_coord)[0]-np.transpose(rot_coord)[0][0], np.transpose(rot_coord)[1]-np.transpose(rot_coord)[1][0])
        plt.plot([kernel[0][0], kernel[1][0], kernel[2][0], kernel[3][0], kernel[4][0]],[kernel[0][1], kernel[1][1], kernel[2][1], kernel[3][1], kernel[4][1]])
        plt.show()
    # check for overflow
    if i == 3209:
        print("[message] simulation finished...")
        break