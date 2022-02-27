#!/usr/bin/env python

import asyncio
import websockets
import socket
import sys
import threading

# enums
GUI_CLIENT = 0
VISION_CLIENT = 1
GRAPHIX_CLIENT = 2
UNDEFINED_CLIENT = 3
MACHYTECHCORE = 4

MACHYSOCKET_HEARTBEAT = 0x00
GET_MACHYSOCKET_STATUS = 0x01


class machytech_graphix():
    def __init__(self):
        self.graphix_status = 0
        self.vision_status = 0
        self.core_status = 0
        self.gui_status = 0
        self.websocket_status = 0

    async def check_status(self, reader):
        if reader:
            self.websocket_status = 1
        else:
            self.websocket_status = 0

    def get_status(self):
        return self.websocket_status


class chat_message():
    def __init__(self, message):
        self.header_length = 4
        self.max_body_length = 512
        self.body_length = 5
        self.target_length = 4

        # treat as private variables (do not write!)
        self.data_ = message.decode()
        self.type_ = UNDEFINED_CLIENT
        self.target_ = UNDEFINED_CLIENT
        self.body_ = UNDEFINED_CLIENT
        self.body_length_ = UNDEFINED_CLIENT


    def decode_body(self):
        target_index = self.target_length + self.header_length
        self.body_ = self.data_[target_index:-1]
        print("body : ", self.body_)

    def decode_header(self):
        header = self.data_[0:self.header_length]
        print("header : ", header)

        # some really ugly if statements to determine the connection type
        if (header=="MGUI"):
            print("message from gui")
            # set the connection type
            self.type_ = GUI_CLIENT
        if (header=="MVIS"):
            print("message from vision")
            # set the connection type
            self.type_ = VISION_CLIENT
        if (header=="MGRX"):
            print("message from graphix")
            # set the connection type
            self.type_ = GRAPHIX_CLIENT
        if (header=="MCOR"):
            print("message from core")
            # set the connection type
            self.type_ = MACHYTECHCORE
        
        target_index = self.target_length + self.header_length
        target = self.data_[self.header_length:target_index]
        print("target : ", target)

        # some more ugly if statements to determine the target
        if (target=="MGUI"):
            print("message for gui")
            # set the target
            self.target_ = GUI_CLIENT
        if (target=="MVIS"):
            print("message for vision")
            # set the target
            self.target_ = VISION_CLIENT
        if (target=="MGRX"):
            print("message for graphix")
            # set the target
            self.target_ = GRAPHIX_CLIENT
        if (target=="MCOR"):
            print("message for core")
            # set the target
            self.target_ = MACHYTECHCORE
        else:
            self.type_ = UNDEFINED_CLIENT
            self.target_ = UNDEFINED_CLIENT


async def wait_for_data(reader, machygraphix):
    data = await reader.read(100)
    msg = chat_message(data)
    msg.decode_header()
    if msg.target_ == GRAPHIX_CLIENT:
        msg.decode_body()
        print("body : ", msg.body_)
    if msg.target_ == GUI_CLIENT:
        print("machy gui connected")
        machygraphix.gui_status = 1
    if msg.target_ == VISION_CLIENT:
        print("machy graphix connected")
        machygraphix.vis_status = 1

async def tcp_client(host, port, machygraphix):
    reader, writer = await asyncio.open_connection(host, port)
    if reader:
        machygraphix.core_status = 1
    try:
        while not reader.at_eof():
            data = await asyncio.wait_for(reader.read(100), 3.0)
            print("raw data received: {}".format(data))
    finally:
        machygraphix.core_status = 0
        writer.close()

async def tcp_continuous_client(host, port, machygraphix):
    reader, writer = await asyncio.open_connection("0.0.0.0", 8000)
    while reader:
        data = await reader.read(100)
        msg = chat_message(data)
        msg.decode_header()
        if msg.target_ == GRAPHIX_CLIENT:
            msg.decode_body()
            print("body : ", msg.body_)
        if msg.target_ == GUI_CLIENT:
            print("machy gui connected")
            machygraphix.gui_status = 1
        if msg.target_ == VISION_CLIENT:
            print("machy graphix connected")
            machygraphix.vis_status = 1
    
    reader.close()

async def send_message(incoming_message, target, ip = '0.0.0.0', port=8000):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((ip, port))
        message = "MGRX"
        message += target
        message += incoming_message        
        s.sendall(message.encode())

async def read_message(host, port, graphix):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect_ex((host, port))
        data = s.recv(1024)
        print("Recieved", repr(data))

async def socket_server(websocket):
    grx = machytech_graphix()
    t = thread()
    t.start(tcp_continuous_client("0.0.0.0", 8000, grx))    

    async for message in websocket:
        print("recieved : ", message)
        if message == MACHYSOCKET_HEARTBEAT:
            await send_message("alive", "MCOR")
            await websocket.send("alive")
        if message == "AAAAA":
            # get machycore status
            response = grx.core_status
            print("websocket status : ", response)
            if response == 1:
                await websocket.send("machycore-connected")
        if message == "AAAAB":
            # get machyvision status
            response = grx.vision_status
            print("machyvision status : ", response)
            if response == 1:
                await websocket.send("machyvision-connected")
        if message == "AAAAC":
            response = grx.vision_status
            print("machyvision status : ", response)
            if response == 1:
                await websocket.send("machygui-connected")
        if message == "AAAAD":
            response = grx.gui_status
            print("machygui status : ", response)
            if response == 1:
                await websocket.send("machyvision-connected")

async def main():
    async with websockets.serve(socket_server, "0.0.0.0", 8800):
        await asyncio.Future()  # run forever

if __name__ == "__main__":
    asyncio.run(main())