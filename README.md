# -- THIS BRANCH WILL NOT BE MERGED WITH MASTER --
# -- USE BINARY IN RELEASE --

## Getting started
copy the shaders and export environment parameters
```
GLSL_APP_VERT #vertex shader directory
GLSL_APP_FRAG #fragment shader directory
IP_ADDR #ip
PORT_ADDR #port
```
```
xinit
export DISPLAY=:0
```
You can start a test http server
```
python3 -m http.server 5000
```
```
./app
```
