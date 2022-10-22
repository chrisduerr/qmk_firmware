#!/bin/bash
sudo pacman -S avr-libc dfu-programmer make
sudo make xd75:undead:dfu
sudo pacman -Rsn avr-libc dfu-programmer make
