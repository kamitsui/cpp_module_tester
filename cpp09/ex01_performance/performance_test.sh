#!/bin/bash

# This script runs the RPN program with expressions of varying lengths
# to measure and demonstrate its performance scalability.

# --- Color Definitions ---
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# --- Variables ---
PRJ_DIR="../../../cpp09/ex01"
EXECUTABLE="./$PRJ_DIR/RPN"
EXPR_GENERATOR="./generate_rpn.sh"

# Array of token counts to test
# Start with smaller numbers and increase. 1 million+ may be slow depending on machine.
TEST_SIZES=(10000 50000 100000 200000)

# --- Pre-flight Checks ---
if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${RED}Error: Executable '$EXECUTABLE' not found.${NC}"
    echo -e "${YELLOW}Please compile the program first with 'make'.${NC}"
    exit 1
fi
if [ ! -x "$EXPR_GENERATOR" ]; then
    echo -e "${RED}Error: Expression generator '$EXPR_GENERATOR' is not executable.${NC}"
    echo -e "${YELLOW}Please run 'chmod +x $EXPR_GENERATOR'.${NC}"
    exit 1
fi

# --- Test Execution ---
echo -e "--- Running Performance Tests for RPN ---"
echo -e "This will measure the execution time for expressions of different lengths."
echo "Note: Expression generation and execution may take a moment for larger sizes."
echo "---------------------------------------------------------"
printf "%-20s | %-15s\n" "Expression Tokens (M)" "RPN Exec Time"
echo "---------------------------------------------------------"

for M in "${TEST_SIZES[@]}"; do
    printf "%-20s | " "$M"

    # 1. Generate the long RPN expression
    #EXPRESSION=$("$EXPR_GENERATOR" "$M")
    #EXPRESSION=`$EXPR_GENERATOR $M`
	#$EXPR_GENERATOR $M > x

    # 2. Run the RPN program and measure its execution time
    # The time command captures stderr, so we redirect it.
	#EXEC_TIME=$( (/usr/bin/time -f "%e" echo -n "$EXPRESSION" | xargs "$EXECUTABLE" > /dev/null) 2>&1 )
	#EXEC_TIME=$( (/usr/bin/time -f "%e" echo "`$EXPR_GENERATOR $M`" | xargs "$EXECUTABLE" > /dev/null) 2>&1 )
	#EXEC_TIME=$( (/usr/bin/time -f "%e" cat x | xargs -L 1 "$EXECUTABLE" > /dev/null) 2>&1 )
	#EXEC_TIME=$( (/usr/bin/time -f "%e" cat x | xargs "$EXECUTABLE" > /dev/null) 2>&1 )
    #EXEC_TIME=$( (/usr/bin/time -f "%e" "$EXECUTABLE" '`echo -n $EXPRESSION`' > /dev/null) 2>&1 )
    #EXEC_TIME=$( (/usr/bin/time -f "%e" "$EXECUTABLE" "$EXPRESSION" > /dev/null) 2>&1 )
    EXEC_TIME=$( (/usr/bin/time -f "%e" "$EXECUTABLE" "`$EXPR_GENERATOR $M`" > /dev/null) 2>&1 )
    printf "${CYAN}%-15s${NC}\n" "${EXEC_TIME}s"
done

# --- Summary ---
echo "---------------------------------------------------------"
echo -e "${GREEN}Test finished.${NC}"
echo "Observe how the 'RPN Exec Time' grows roughly linearly with 'Expression Tokens (M)'."
echo "This demonstrates the O(M) efficiency of the RPN algorithm."
