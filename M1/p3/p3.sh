#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail


if [[ "$#" -lt 1 ]]
then
    echo "error: incorrect command line arguments"
    echo "usage p2.sh FILE"
    exit 1 
elif [[ ! -e "$1" ]]
then
    echo "File does not exist."
    exit 1 
else

    FILE="logs"
    mkdir -p "$FILE"

    for suser in "${@:2}"; do
        mkdir -p -- ./logs/"$suser"

        NEW="$(grep "$suser" "$1" | cut -f2,6,7 -d,)"
        columns=$(seq -f '"col_%g"' -s', ' 1 1001)

        while IFS= read -r line; do
            
            DEVICEPRODUCT="$(echo "$line" |cut -f1 -d,)"
            NAME="$(echo "$line" |cut -f2 -d,)"
            START="$(echo "$line" |cut -f3 -d,)"

            if [[ ! (-d ./logs/"$suser"/"$DEVICEPRODUCT") ]]
            then
                mkdir ./logs/"$suser"/"$DEVICEPRODUCT"
                echo "start,name" > ./logs/"$suser"/"$DEVICEPRODUCT"/log.csv
            fi

            L=$(printf "%s,$NAME %s\n" "$START")
            echo "$L" >> ./logs/"$suser"/"$DEVICEPRODUCT"/log.csv


        done <<< "$NEW"

    done

fi


#-f14-
#-f2- Device product
#-f6- Name
#-f7- Start