#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail

file='lstops.csv'
array=("red", "blue", "brown", "green", "orange", "pink", "purple", "yellow")


if [[ "$#" > 1 ]]
then
    exit 1 
elif [[ "$#" -ne 1 ]]
then
    while IFS= read -r line
    do        
        stop="$line"
        lines=$(echo -e "$line\n" | awk -F'[()]' '{print $2}')
        stop=$(echo "$stop" | cut -d "," -f3 | tr -d '"' | cut -d "(" -f1 )

        for x in $lines;do
            x=$(echo $x | tr -d ',')
            if [[ $(echo ${array[@]} | fgrep -wy $x) ]]
            then
                stop=$(echo "$stop" | awk '{$1=$1};1')
                pr="$stop","$x"
                echo "$pr"
            fi
        done
    done <"$file"    

elif ! [[ $(echo ${array[@]} | fgrep -wy "$1") ]]
then
    exit 1   

else
    while IFS= read -r line
    do        
        temp="$line"
        lines=$(echo -e "$line\n" | awk -F'[()]' '{print $2}')
        stop=$(echo "$temp" | cut -d "," -f3 | tr -d '"' | cut -d "(" -f1 )

        if [[ $(echo "$lines" | fgrep -wy $1) ]]
        then
            for x in $lines;do
                x=$(echo $x | tr -d ',')
                if [[ "${x,,}" = "${1,,}" ]]
                then
                    stop=$(echo "$stop" | awk '{$1=$1};1')
                    pr="$stop","$x"
                    echo "$pr"
                fi
            done
        fi
    done <"$file" 
fi


    