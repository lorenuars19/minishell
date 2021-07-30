#!/bin/bash

if [[ $# -ne 1 ]]
then
	printf "Usage : %s [head -n]\n" $0
	exit 0
fi

cat /dev/random | head -n 2 | tr -cd '[:print:]' > random.txt

printf "\n\n\n\n\n\n\n\nexit" >> random.txt

./minishell < random.txt
