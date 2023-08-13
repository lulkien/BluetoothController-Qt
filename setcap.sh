#!/usr/bin/env bash
DIR_NAME=$(dirname "$BASH_SOURCE")
sudo setcap cap_net_admin+eip $DIR_NAME/build/Bluetooth
