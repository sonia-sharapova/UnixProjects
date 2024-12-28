#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail

# TODO: Error-Check the command line argument is supplied.
if [[ "$#" -ne 1 ]]
then
    exit 1 
fi

# ONE_LINE_COMMAND

array=$'Homework Projects Exams \n';one=$(cat "$1" | awk --field-separator="," '{print $1;}' );two=$(cat "$1" | awk --field-separator="," '{print $2;}' );three=$(cat "$1" | awk --field-separator="," '{print $3;}' );hw='for i in $(seq "$one"); do echo "hw""$i"; done';c1=$'Homework\n'$(eval "$hw");proj='for i in $(seq "$two"); do echo "proj""$i"; done';c2=$'Projects\n'$(eval "$proj");exam='for i in $(seq "$three"); do echo "exam""$i"; done';c3=$'Exams\n'$(eval "$exam");echo "$c1" > col1.txt;echo "$c2" > col2.txt;echo "$c3" > col3.txt;x=$(paste <(echo "$c1") <(echo "$c2") <(echo "$c3") );echo "$x" > all.txt | column -t all.txt

