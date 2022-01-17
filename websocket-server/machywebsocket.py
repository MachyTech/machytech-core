#!/usr/bin/env python

import asyncio
import websockets
import socket

# enums
GUI_CLIENT = 0
VISION_CLIENT = 1
GRAPHIX_CLIENT = 2
UNDEFINED_CLIENT = 3
MACHYTECHCORE = 4

class machytech_graphix():
    def __init__(self):
        self.graphix_status
        self.vision_status
        self.core_status
        self.gui_status

    async def check_status(self, reader):
        if reader:
            self.status = True
        else:
            self.status = False




class chat_message():
    def __init__(self, message):
        self.header_length = 4
        self.max_body_length = 512
        self.body_length = 5
        self.target_length = 4

        # treat as private variables (do not write!)
        self.data_ = message.decode()
        self.type_
        self.target_
        self.body_
        self.body_length_


    def decode_body(self):
        target_index = self.target_length + self.header_length
        self.body_ = self.data[target_index:-1]
        print("body : ", self.body_)
    
    def decode_header(self):
        header = self.data[self.header_length]
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
        target = self.data[self.header_length:target_index]
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


async def wait_for_data(reader):
    data = await reader.read(100)
    msg = chat_message(data)
    msg.decode_header()
    if msg.target_ == GRAPHIX_CLIENT:
        msg.decode_body()
        print("body : ", msg.body_)

async def send_message(incoming_message, ip = '0.0.0.0', port=8000):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((ip, port))
        message = "MGRX"
        message += "MCOR"
        message += incoming_message        
        s.sendall(message.encode())


async def socket_server(websocket):
    grx = machytech_graphix()

    reader, writer = await asyncio.open_connection('0.0.0.0', 8000)
    await grx.check_status(reader)

    await wait_for_data(reader)
    async for message in websocket:
        print(message)
        await send_message(message)
        await websocket.send(message)

async def main():
    async with websockets.serve(socket_server, "0.0.0.0", 8800):
        await asyncio.Future()  # run forever

if __name__ == "__main__":
    asyncio.run(main())