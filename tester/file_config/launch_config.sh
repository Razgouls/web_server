#!/bin/bash

RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
CYAN="\033[1;36m"
ENDCOLOR="\033[0m"

echo -e "${GREEN}"
echo "====================================================================="
echo "=========================== LAUNCH TESTER ==========================="
echo "====================================================================="
echo -ne "${ENDCOLOR}"

read $VAR
clear

test_file_config()
{
	cmp=1
	find $1 -type f -name "*.conf" | while read line; do
	./webserv $line & >> /dev/null 2>> /dev/null
	ps -a | grep "webserv" >> /dev/null 2>> /dev/null
	if [[ $? -ge 1 ]]; then
		if [[ $cmp -ge 10 ]]; then
			echo -e "${YELLOW}TEST $cmp :  ${GREEN}[ OK ]${ENDCOLOR}"
		else
			echo -e "${YELLOW}TEST $cmp  :  ${GREEN}[ OK ]${ENDCOLOR}"
		fi
	else
		if [[ $cmp -ge 10 ]]; then
			echo -e "${YELLOW}TEST $cmp :  ${RED}[ KO ]${ENDCOLOR} -> $line"
		else
			echo -e "${YELLOW}TEST $cmp  :  ${RED}[ KO ]${ENDCOLOR} -> $line"
		fi
		var=$(pgrep webserv)
		kill $var
	fi
	cmp=$((cmp+1))
	done
}

echo -e "${CYAN}"
	echo "====================================================================="
	echo "========================= TEST CONFIG SERVER ========================"
	echo "====================================================================="
	echo -e "${ENDCOLOR}"

test_brackets()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "======================== TEST CONFIG BRACKETS ======================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/brackets
	
	read $VAR
}

test_entete()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "========================= TEST CONFIG ENTETE ========================"
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_server/server

	read $VAR
}

test_listen()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "========================= TEST CONFIG LISTEN ========================"
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_server/listen

	read $VAR
}

test_semicolon()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "======================= TEST CONFIG SEMICOLOM ======================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_server/semicolon

	read $VAR
}

test_host()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "========================== TEST CONFIG HOST ========================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_server/host

	read $VAR
}

test_server_name()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "====================== TEST CONFIG SERVER_NAME ======================"
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_server/server_name

	read $VAR
}

test_root()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "========================== TEST CONFIG ROOT ========================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_server/root

	read $VAR
}

test_limit()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "========================= TEST CONFIG LIMIT ========================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_server/limit

	read $VAR
}

test_error()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "========================= TEST CONFIG ERROR ========================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_server/error

	read $VAR
}

test_empty()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "===================== TEST CONFIG LOCATION EMPTY ==================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/empty/empty.conf

	read $VAR
}

test_same_port()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "====================== TEST CONFIG POLY SERVERS ====================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/config_poly_servers

	read $VAR
}



test_location()
{
	clear
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "======================== TEST CONFIG LOCATION ======================="
	echo "====================================================================="
	echo -e "${ENDCOLOR}"

	echo -e "${CYAN}"
	echo "====================================================================="
	echo "==================== TEST CONFIG LOCATION ENTETE ===================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_location/location

	read $VAR
}

test_path()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "===================== TEST CONFIG LOCATION PATH ====================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_location/path

	read $VAR
}

test_methods()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "==================== TEST CONFIG LOCATION METHODS ==================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_location/methods

	read $VAR
}

test_autoindex()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "=================== TEST CONFIG LOCATION AUTOINDEX =================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_location/autoindex

	read $VAR
}

test_extension()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "=================== TEST CONFIG LOCATION EXTENSION =================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_location/extension

	read $VAR
}

test_bin()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "====================== TEST CONFIG LOCATION BIN ====================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_location/bin

	read $VAR
}

test_upload_dir()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "===================== TEST CONFIG LOCATION UPLOAD ==================="
	echo -ne "${ENDCOLOR}"
	test_file_config ./tester/file_config/config_error/conf_location/upload

	read $VAR
}


test_file_config_ok()
{
	cmp=1
	find $1 -type f -name "*.conf" | while read line; do
	./webserv $line & >> /dev/null 2>> /dev/null
	ps -a | grep "webserv" >> /dev/null 2>> /dev/null
	if [[ $? -eq 0 ]]; then
		if [[ $cmp -ge 10 ]]; then
			echo -e "${YELLOW}TEST $cmp :  ${GREEN}[ OK ]${ENDCOLOR}"
		else
			echo -e "${YELLOW}TEST $cmp  :  ${GREEN}[ OK ]${ENDCOLOR}"
		fi
		var=$(pgrep webserv)
		kill $var
	else
		if [[ $cmp -ge 10 ]]; then
			echo -e "${YELLOW}TEST $cmp :  ${RED}[ KO ]${ENDCOLOR} -> $line"
		else
			echo -e "${YELLOW}TEST $cmp  :  ${RED}[ KO ]${ENDCOLOR} -> $line"
		fi
	fi
	cmp=$((cmp+1))
	done
}

test_poly_simple()
{
	clear
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "=========================== TEST CONFIG OK =========================="
	echo "====================================================================="
	echo -e "${ENDCOLOR}"

	echo -e "${CYAN}"
	echo "====================================================================="
	echo "======================= TEST CONFIG POLY SIMPLE ====================="
	echo -ne "${ENDCOLOR}"
	test_file_config_ok ./tester/file_config/config_ok/config_poly_simple

	read $VAR
}

test_blank()
{
	echo -e "${CYAN}"
	echo "====================================================================="
	echo "========================== TEST CONFIG BLANK ========================"
	echo -ne "${ENDCOLOR}"
	test_file_config_ok ./tester/file_config/config_ok/config_blank

	read $VAR
}


test_brackets
test_entete
test_listen
test_semicolon
test_host
test_server_name
test_root
test_limit
test_error
test_empty
test_same_port

test_location
test_path
test_methods
test_autoindex
test_extension
test_bin
test_upload_dir

test_poly_simple
test_blank