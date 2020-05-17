#!/bin/bash
sudo pacman -S avr-libc dfu-programmer
sudo make xd75:undead:dfu
sudo pacman -Rsn avr-libc dfu-programmer
