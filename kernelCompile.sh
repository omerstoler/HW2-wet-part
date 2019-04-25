#!/bin/bash
#
# Script to automate kernel compilation
#
# Place this script in /usr/src, chmod +x it to make it executable
# This script must be run as root
#
VERSION='2.4.18-14custom' # change to reflect actual kernel version
CWD='/usr/src/' # /usr/src directory

#
# Make the kernel image
cd $CWD/linux-$VERSION
make bzImage 
cp $CWD/linux-$VERSION/arch/i386/boot/bzImage /boot/vmlinuz-$VERSION
