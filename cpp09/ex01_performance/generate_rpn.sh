#!/bin/bash

# This script generates a long, valid RPN expression for performance testing.
# The expression will be of the form "1 2 + 2 + 2 + ..."
# Usage: ./generate_rpn.sh <number_of_tokens>

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number_of_tokens>" >&2
    exit 1
fi

NUM_TOKENS=$1

# Ensure an even number of tokens for simplicity, as we add pairs of "+ 2"
if (( NUM_TOKENS % 2 != 0 )); then
    ((NUM_TOKENS++))
fi

# Use awk for efficient string generation
# It starts with "1 2" (2 tokens) and adds pairs of "+ 2" until the desired length is reached.
awk -v count="$NUM_TOKENS" '
BEGIN {
    printf "1 2 +";
    # We already have 2 tokens, so we need (count - 2) / 2 pairs of "+ 2"
    for (i = 1; i <= (count - 2) / 2; i++) {
        printf " 2 +";
    }
}'
