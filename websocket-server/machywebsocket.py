#!/usr/bin/env python

import asyncio
import websockets
import socket

async def send_message(message, ip = '0.0.0.0', port=5000):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((ip, port))
        s.sendall(message.encode())

async def echo(websocket):
    async for message in websocket:
        print(message)
        await send_message(message)
        await websocket.send(message)

async def main():
    async with websockets.serve(echo, "0.0.0.0", 8000):
        await asyncio.Future()  # run forever

if __name__ == "__main__":
    asyncio.run(main())