#!/bin/bash
pacaur -S avr-libc dfu-programmer
sudo make xd75:undead:dfu
pacaur -Rsn avr-libc dfu-programmer
