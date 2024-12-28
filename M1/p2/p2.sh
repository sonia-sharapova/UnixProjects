#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail


if [[ "$#" -ne 1 ]]
then
    echo "error: incorrect command line arguments"
    echo "usage p2.sh FILE"
    exit 1 

elif [[ ! -e "$1" ]]
then
    echo "File does not exist."
    exit 1 
else
    FILE="home"
    mkdir -p "$FILE"
    #cd ./home

    #echo "Hello Students" > "$FILE/students.txt"

    while IFS=" " read -r USERNAME CLASS_CAT COURSE_ID GRADE; do
        mkdir -p ./home/"$USERNAME"
        mkdir -p ./home/"$USERNAME"/"$CLASS_CAT"
        touch ./home/"$USERNAME"/"$CLASS_CAT"/MPCS"$COURSE_ID".txt

        echo "$GRADE" > ./home/"$USERNAME"/"$CLASS_CAT"/MPCS"$COURSE_ID".txt

    done < "$1"
fi

