import asyncio
import logging
import websockets
from websockets import WebSocketServerProtocol
import time
import threading

'''
websocket server for data distribution
'''

logging.basicConfig(level=logging.INFO)
debug = True

class webSocketServer:
    clients = set()
    logging.info(f'starting up ...')

    def __init__(self):
        logging.info(f'init happened ...')

    async def ws_handler(self, ws: WebSocketServerProtocol, url: str) -> None:
        print("run handler")
        print(f"url : {url}")
        if url == "lakehouse":
            await self.register(ws)
            try:
                await self.distribute(ws)
            finally:
                await self.unregister(ws)

    async def register(self, ws: WebSocketServerProtocol) -> None:
        self.clients.add(ws)
        logging.info(f'{ws.remote_address} connects')

    async def unregister(self, ws: WebSocketServerProtocol) -> None:
        self.clients.remove(ws)
        logging.info(f'{ws.remote_address} disconnects')

    async def distribute(self, ws: WebSocketServerProtocol) -> None:
        async for message in ws:
            await self.sendDataToClient(ws, message)
            #await self.send_to_clients(message)

    async def sendDataToClient(self,  ws: WebSocketServerProtocol, message) -> None:
        logging.info(f"sending data: {message}")
        for client in self.clients:
            if ws.remote_address != client.remote_address:
                logging.info(f"sending data to {ws.remote_address}")
                await client.send(f"{message}")

    async def send_to_clients(self, message: str) -> None:
        if self.clients:
            logging.info(f"sending message: {message}")
            await asyncio.wait([client.send(message) for client in self.clients])


async def timerThread(server, counterX):
    counterX = 0
    while True:
        await checkAndSend(server, counterX)
        print(f"step  {counterX}")
        time.sleep(30)
        counterX += 1


async def checkAndSend(server, counter):
    # check something
    # send message
    logging.info("in check and send")
    await server.send_to_clients(f"check in: {counter}")


# helper routine to allow thread to call async function
def between_callback(server, counter):
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.run_until_complete(timerThread(server, counter))
    loop.close()


# start server
server = webSocketServer()
start_server = websockets.serve(server.ws_handler, '0.0.0.0', 5000)
counter = 0

# start timer thread
threading.Thread(target=between_callback, args=(server, counter,)).start()

# start main event loop
loop = asyncio.get_event_loop()
loop.run_until_complete(start_server)
loop.run_forever()
