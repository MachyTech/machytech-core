#!/bin/bash
# include commands in visudo
sudo systemctl stop gdm.service
sudo systemctl stop gdm3.service
xinit -geometry =720x400 -fn 8x13 -j -fg white -bg navy -ac -noreset -nolisten tcp
#sleep 5
#export DISPLAY=:0
#xrandr --output HDMI-0 --mode 720x400 --rate 70