#!/bin/bash

# --- Color Definitions ---
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# --- Variables ---
PRJ_DIR="../../../cpp09/ex00"
EXECUTABLE="./$PRJ_DIR/btc"
TMP_OUTPUT="tmp_output.log"
TMP_ERROR="tmp_error.log"
TEST_COUNT=0
PASS_COUNT=0

# --- Pre-flight Check ---
if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${RED}Error: Executable '$EXECUTABLE' not found.${NC}"
    echo -e "${YELLOW}Please compile the program first with 'make'.${NC}"
    exit 1
fi

# --- Test Function ---
# $1: Test case description
# $2: Command to execute
# $3: Expected stdout file
# $4: Expected stderr file
run_test() {
    ((TEST_COUNT++))
    DESCRIPTION=$1
    COMMAND=$2
    EXPECTED_STDOUT=$3
    EXPECTED_STDERR=$4

    # Execute the command and capture output/error
    eval "$COMMAND" > "$TMP_OUTPUT" 2> "$TMP_ERROR"

    # Assume pass until a check fails
    local PASS=true

    # Check stdout if an expected file is provided
    if [ -n "$EXPECTED_STDOUT" ]; then
        if ! diff -q "$EXPECTED_STDOUT" "$TMP_OUTPUT" > /dev/null; then
            PASS=false
        fi
    fi

    # Check stderr if an expected file is provided
    if [ -n "$EXPECTED_STDERR" ]; then
        if ! diff -q "$EXPECTED_STDERR" "$TMP_ERROR" > /dev/null; then
            PASS=false
        fi
    fi

    # Print result
    if [ "$PASS" = true ]; then
        echo -e "[ ${GREEN}OK${NC} ] $DESCRIPTION"
        ((PASS_COUNT++))
    else
        echo -e "[ ${RED}KO${NC} ] $DESCRIPTION"
        echo "--- Expected STDOUT ($EXPECTED_STDOUT) ---"
        cat "$EXPECTED_STDOUT"
        echo "--- Actual STDOUT ---"
        cat "$TMP_OUTPUT"
        echo "--------------------"
        echo "--- Expected STDERR ($EXPECTED_STDERR) ---"
        cat "$EXPECTED_STDERR"
        echo "--- Actual STDERR ---"
        cat "$TMP_ERROR"
        echo "--------------------"
    fi
}

# --- Test Execution ---
echo "--- Running Integration Tests for BitcoinExchange ---"

# 1. Normal Case: Valid input file
run_test "Normal operation with valid data" \
         "$EXECUTABLE input_normal.txt" \
         "expected_normal_stdout.txt" \
         "expected_empty.txt"

# 2. Argument Errors: No arguments
run_test "Argument Error: No arguments" \
         "$EXECUTABLE" \
         "expected_empty.txt" \
         "expected_arg_error.txt"

# 3. File Errors: Non-existent input file
run_test "File Error: Non-existent input file" \
         "$EXECUTABLE no_such_file.txt" \
         "expected_empty.txt" \
         "expected_file_error.txt"

# 4. Data Errors: Mixed valid and invalid data
run_test "Data Handling: Mixed valid and invalid data" \
         "$EXECUTABLE input_mixed_errors.txt" \
         "expected_mixed_stdout.txt" \
         "expected_mixed_stderr.txt"

# 5. Boundary Case: Empty input file
run_test "Boundary Case: Empty input file" \
         "$EXECUTABLE input_empty.txt" \
         "expected_empty.txt" \
         "expected_empty.txt"

# --- Cleanup ---
rm -f "$TMP_OUTPUT" "$TMP_ERROR"

# --- Summary ---
echo "---------------------------------------------------"
if [ "$PASS_COUNT" -eq "$TEST_COUNT" ]; then
    echo -e "${GREEN}All $TEST_COUNT tests passed successfully!${NC}"
else
    echo -e "${RED}$((TEST_COUNT - PASS_COUNT)) out of $TEST_COUNT tests failed.${NC}"
fi
echo "---------------------------------------------------"
