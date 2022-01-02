# chat-server
This folder includes all the code needed to run the chat-server. The chat-server is the layer necessary to combine all different machytech processes. It reads and sends message in a certain format defined in the header file "message.hpp".

## building
It uses the meson buildsystem. The building script is defined in meson.build.
"meson setup builddir && cd builddir"
"meson compile"

## dependecies
it heavily relies on the boost libraries. For now on any linux system < 18.01 installing boost with apt will suffice.

## usage
"Usage: chat_server <port> [<port> ...]"

