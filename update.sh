#!/bin/bash

set -e

sudo pacman -S --noconfirm --needed avr-libc dfu-programmer make qmk python-pip
sudo pip install pyserial pillow

# sudo make xd75:undead:dfu

sudo qmk compile -kb undeadsplit -km default
sudo qmk flash -kb undeadsplit -km default

sudo pip uninstall -y pyserial pillow
sudo pacman -Rsn --noconfirm avr-libc dfu-programmer make qmk python-pip
