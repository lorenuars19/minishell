#!/bin/bash

if [[ $# -ne 1 ]]
then
	printf "Usage : %s [head -n]\n" $0
	exit 0
fi

cat /dev/urandom | head -n $1 | tr -cd "a |" > random.txt

printf "\nexit\n" >> random.txt

./minishell < random.txt
