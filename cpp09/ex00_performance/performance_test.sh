#!/bin/bash

# This script runs the btc program against databases of varying sizes
# to measure and demonstrate its performance scalability.

# --- Color Definitions ---
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# --- Variables ---
PRJ_DIR="../../../cpp09/ex00"
EXECUTABLE="./$PRJ_DIR/btc"
DB_GENERATOR="./generate_db.sh"
INPUT_FILE="input_perf.txt"
DB_FILE="data.csv"

# Array of database sizes to test
TEST_SIZES=(1000 10000 100000 500000) # You can add more sizes like 1000000

# --- Pre-flight Checks ---
if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${RED}Error: Executable '$EXECUTABLE' not found.${NC}"
    echo -e "${YELLOW}Please compile the program first with 'make'.${NC}"
    exit 1
fi
if [ ! -x "$DB_GENERATOR" ]; then
    echo -e "${RED}Error: Database generator '$DB_GENERATOR' is not executable.${NC}"
    echo -e "${YELLOW}Please run 'chmod +x $DB_GENERATOR'.${NC}"
    exit 1
fi
if [ ! -f "$INPUT_FILE" ]; then
    echo -e "${RED}Error: Input file '$INPUT_FILE' not found.${NC}"
    exit 1
fi

# --- Test Execution ---
echo -e "--- Running Performance Tests for BitcoinExchange ---"
echo -e "This will measure the execution time for searching in databases of different sizes."
echo "Note: Database generation may take a moment for larger sizes."
echo "---------------------------------------------------------"
printf "%-15s | %-20s | %-15s\n" "DB Rows (N)" "DB Generation Time" "BTC Exec Time"
echo "---------------------------------------------------------"

for N in "${TEST_SIZES[@]}"; do
    printf "%-15s | " "$N"

    # 1. Generate the database and measure generation time
    # Use /usr/bin/time to get clean time output
    GEN_TIME=$( (/usr/bin/time -f "%e" "$DB_GENERATOR" "$N" > "$DB_FILE") 2>&1 )
    printf "%-20s | " "${GEN_TIME}s"

    # 2. Run the btc program and measure its execution time
    # We only care about the execution time of btc, not the output.
    # The time command captures stderr, so we redirect it.
    EXEC_TIME=$( (/usr/bin/time -f "%e" "$EXECUTABLE" "$INPUT_FILE" > /dev/null) 2>&1 )
    printf "${CYAN}%-15s${NC}\n" "${EXEC_TIME}s"
done

# --- Summary ---
echo "---------------------------------------------------------"
echo -e "${GREEN}Test finished.${NC}"
echo "Observe how the 'BTC Exec Time' does not grow linearly with 'DB Rows (N)'."
echo "This demonstrates the O(log N) efficiency of the search algorithm."

# --- Cleanup ---
rm -f "$DB_FILE"
