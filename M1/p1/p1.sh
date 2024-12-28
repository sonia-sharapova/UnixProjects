#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail


if [[ "$#" -ne 3 ]]
then
  echo "error: incorrect command line arguments"
  echo "usage p1.sh FILE WARD SECTION"
  exit 1 

elif [[ "$2" -gt 50 ]] || [[ "$2" -lt 1 ]]
then  
  echo "error: WARD out of range"
  echo "WARD is beteen 1 and 50 (inclusive)"
  exit 1 

elif [[ "$3" -gt 32 ]] || [[ "$3" -lt 1 ]]
then  
  echo "error: SECTION out of range"
  echo "SECTION is beteen 1 and 50 (inclusive)"
  exit 1 

else
    WARDSECTION="$(printf "%02d%02d" $2 $3)"
    WARDSCHED="$(grep "$WARDSECTION" sweepSched.csv | cut -f5- -d,)"

    while IFS= read -r line; do
      #echo "$line"
      
      MONTH="$(echo "$line" |cut -f1 -d,)"
      DAY1="$(echo "$line" |cut -f2 -d, | tr -d \")"
      DAY2="$(echo "$line" |cut -f3 -d, | tr -d \")"
      echo $(printf "%02d" "$MONTH")/$(printf "%02d" "$DAY1")/"23"
      echo $(printf "%02d" "$MONTH")/$(printf "%02d" "$DAY2")/"23"
      
    done <<< "$WARDSCHED"

    #echo "$WARDSCHED"
    exit 0
fi

#1-32
#1-50
