#!/bin/bash

set -e

sudo pacman -S --noconfirm --needed avr-libc dfu-programmer make qmk python-pip python-pyserial python-pillow

# sudo make xd75:undead:dfu

sudo qmk compile -kb undeadsplit -km default
sudo qmk flash -kb undeadsplit -km default

sudo pacman -Rsn --noconfirm avr-libc dfu-programmer make qmk python-pip python-pyserial python-pillow
