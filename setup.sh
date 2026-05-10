#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "Run this script as root! Exiting."
   exit 1
fi

if [ -z "$1" ]; then
  echo -n "Enter Interface Name [tap0]: "
  read -r vInterfaceName
  if [ -z "$vInterfaceName" ]; then
    vInterfaceName="tap0"
  fi
  echo ""
else
  vInterfaceName="$1"
fi

sUserName=${SUDO_USER:-$USER}

echo "Using user as: $sUserName"
echo "Chosen Virtual TUN/TAP interface name: $vInterfaceName"

if [[ $(ip tuntap list | grep -wc "$vInterfaceName" ) -ge 1 ]]; then
  echo "Virtual TUN/TAP Interface already exists! Exiting."
  exit 1
fi

ip tuntap add mode tap user "$sUserName" group "$sUserName" name "$vInterfaceName"

if [ $? -ne 0 ]; then
  echo "Some error was encountered! Sorry creation of virtual interface failed!"
  exit 1
fi

exit 0




