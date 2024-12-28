#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail

# Initialize default values for options (can change  later)
num_lines=10
e_users=""
process_command=""
input_file=""

# Case and shift to process command-line options
while [[ $# -gt 0 ]]; do
  case "$1" in
    -n)
      num_lines="$2"
      shift 2
      ;;
    -e)
      e_users="$2"
      shift 2
      ;;
    -p)
      process_command="$2"
      shift 2
      ;;
    *)
      if [[ $# -gt 0 ]]; then
        input_file="$1"
        break
      fi
      ;;
  esac
done

# Read input
if [ -n "$input_file" ]; then
  line_contents=$(cat "$input_file")
else
  line_contents=$(cat -)
fi

line_contents=$(echo "$line_contents" | sed 1d)

# (-p option)
if [ -n "$process_command" ]; then
  line_contents=$(echo "$line_contents" | grep -F "$process_command")
fi

# Get and count user instances 
process=$(echo "$line_contents" | grep -oE '^[^[:space:]]+' | sort | uniq -c | sort -k1,1nr -k2,2)

# For -e
if [ -n "$e_users" ]; then
  for user in $(echo "$e_users" | tr ',' ' '); do
    process=$(echo "$process" | grep -vE "^[[:space:]]*[0-9]+[[:space:]]+$user\b")
  done
fi

# for -n option
echo "$process" | head -n "$num_lines" | while read -r line; do
  user=$(echo "$line" | cut -d' ' -f2)
  count=$(echo "$line" | cut -d' ' -f1)
  echo "$user,$count"
done
