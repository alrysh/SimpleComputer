#!/bin/bash

echo -e "\E[2J"

echo -e "\E(0"

# tput cup 10 5; echo "lqqqqqqqqqqqqqk"
tput cup 10 5; echo -n "l"
for i in {1..20}; do
    echo -n "q"
done
echo "k"

for i in {1..8}; do
    tput cup $((10 + i)) 5; echo "x                    x"
done

tput cup 9 8; echo "aaaaaaaa"
tput cup 10 8; echo "aa"
tput cup 11 8; echo "aa"
tput cup 12 8; echo "aaaaaaaa"
tput cup 13 8; echo "aa    aa"
tput cup 14 8; echo "aa    aa"
tput cup 15 8; echo "aaaaaaaa"

# tput cup 8 5; echo "mqqqqqqqqqqqqqj"
tput cup 16 5; echo -n "m"
for i in {1..20}; do
    echo -n "q"
done
echo "j"

echo -e "\E(B"

tput cup 20 0