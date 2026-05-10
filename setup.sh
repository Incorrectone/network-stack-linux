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

echo -n "Enter IPv4 Address Name [10.0.0.1/24]: "
read -r viIPv4Address
if [ -z "$viIPv4Address" ]; then
  viIPv4Address="10.0.0.1/24"
fi
echo ""

sUserName=${SUDO_USER:-$USER}

echo "Using user as: $sUserName"
echo "Chosen Virtual TUN/TAP interface name: $vInterfaceName"
echo "Will try and assign Local IP: $viIPv4Address"

if [[ $(ip tuntap list | grep -wc "$vInterfaceName" ) -ge 1 ]]; then
  echo "Virtual TUN/TAP Interface already exists! Exiting..."
  exit 1
fi

if [[ $(ip -o -4 addr | grep -wc "$viIPv4Address" ) -ge 1 ]]; then
  echo "IP Address block already Assigned! Exiting..."
  exit 1
fi

ip tuntap add mode tap user "$sUserName" group "$sUserName" name "$vInterfaceName"

if [ $? -ne 0 ]; then
  echo "Some error was while creating a new interface!"
  exit 1
  else
    printf "%s created successfully!\n" "$vInterfaceName"
  fi

ip link set dev "$vInterfaceName" up

if [ $? -ne 0 ]; then
  echo "Some error was while trying to turn UP the interface!"
  exit 1
else
  printf "%s turned UP successfully!\n" "$vInterfaceName"
fi

ip addr add "$viIPv4Address" dev "$vInterfaceName"

if [ $? -ne 0 ]; then
  echo "Some error was encountered! Sorry creation of virtual interface failed!"
  exit 1
else
  printf "%s assigned %s successfully!\n" "$vInterfaceName" "$viIPv4Address"
fi

exit 0




