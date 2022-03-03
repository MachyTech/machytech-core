import websockets
import asyncio
import json
from threading import Thread
import functools

from websocketserver.utils import logger, arguments, environment

connections = {}

async def msg_handler(websocket, port, dummy=None):
    connections[port].add(websocket)
    async for message in websocket:
        websockets.broadcast(connections[port], message)


async def echo_handler(websocket):
    async for message in websocket:
        await websocket.send(message)

async def vision_handler(websocket, port, dummy=None):
    connections[port].add(websocket)
    async for message in websocket:
        event = json.loads(message)
        x = event["x"]
        y = event["y"]
        print(event)
        event = {
            "type": "tiangle",
            "x": x,
            "y": y
        }
        websockets.broadcast(connections[port], json.dumps(event))

async def server(it, environment, logger):
    if environment.HANDLERS[it]=="message":
        try:
            async with websockets.serve(functools.partial(msg_handler, port=environment.PORTS[it]), 
                    environment.HOST, environment.PORTS[it]):
                logger.info("Serving {} MachyCore on: {}:{} with {} handler".format(it, environment.HOST, 
                    environment.PORTS[it], environment.HANDLERS[it]))
                await asyncio.Future()
        except Exception as e:
            logger.error(e)
    if environment.HANDLERS[it]=="vision":
        try:
            async with websockets.serve(functools.partial(vision_handler, port=environment.PORTS[it]), 
                    environment.HOST, environment.PORTS[it]):
                logger.info("Serving {} MachyCore on: {}:{} with {} handler".format(it, environment.HOST, 
                    environment.PORTS[it], environment.HANDLERS[it]))
                await asyncio.Future()
        except Exception as e:
            logger.error(e)
    else:
        logger.error("handler not recognized")


def main():
    arg = arguments()
    log = logger(arg)
    env = environment(arg, log)

    for i in range(len(env.PORTS)):
        print(env.PORTS[i])
        connections[env.PORTS[i]] = set()
        thread = Thread(target=asyncio.run, args=(server(i, env, log),))
        thread.start()