#!/bin/bash

RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
CYAN="\033[1;36m"
ENDCOLOR="\033[0m"

find ./config/testeur -type f -name "*.conf" | while read line; do
    echo -e "${GREEN}[LINE : $line]${ENDCOLOR}"
    ./web_server $line >> rep.txt
    echo ""
done