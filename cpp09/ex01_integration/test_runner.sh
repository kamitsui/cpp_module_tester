#!/bin/bash

# --- Color Definitions ---
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# --- Variables ---
PRJ_DIR="../../../cpp09/ex01"
EXECUTABLE="./$PRJ_DIR/RPN"
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

# Create expected output files if they don't exist
echo "42" > expected_stdout_42.txt
echo "Error" > expected_stderr_error.txt
touch expected_empty.txt

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

    # Remove trailing newline for comparison
    #tr -d '\n' < "$TMP_OUTPUT" > tmp && mv tmp "$TMP_OUTPUT"
    #tr -d '\n' < "$TMP_ERROR" > tmp && mv tmp "$TMP_ERROR"

    local PASS=true

    # Check stdout
    if ! diff -q "$EXPECTED_STDOUT" "$TMP_OUTPUT" > /dev/null; then
        PASS=false
    fi

    # Check stderr
    if ! diff -q "$EXPECTED_STDERR" "$TMP_ERROR" > /dev/null; then
        PASS=false
    fi

    # Print result
    if [ "$PASS" = true ]; then
        echo -e "[ ${GREEN}OK${NC} ] $DESCRIPTION"
        ((PASS_COUNT++))
    else
        echo -e "[ ${RED}KO${NC} ] $DESCRIPTION"
        echo "--- Expected STDOUT ('$EXPECTED_STDOUT') ---"
        cat "$EXPECTED_STDOUT" && echo
        echo "--- Actual STDOUT ---"
        cat "$TMP_OUTPUT" && echo
        echo "--------------------"
        echo "--- Expected STDERR ('$EXPECTED_STDERR') ---"
        cat "$EXPECTED_STDERR" && echo
        echo "--- Actual STDERR ---"
        cat "$TMP_ERROR" && echo
        echo "--------------------"
    fi
}

# --- Test Execution ---
echo "--- Running Integration Tests for RPN ---"

# 1. Normal Cases from subject
run_test "Normal Case 1: Complex expression" \
         "$EXECUTABLE \"8 9 * 9 - 9 - 9 - 4 - 1 +\"" \
         "expected_stdout_42.txt" \
         "expected_empty.txt"

run_test "Normal Case 2: Simple chain" \
         "$EXECUTABLE \"7 7 * 7 -\"" \
         "expected_stdout_42.txt" \
         "expected_empty.txt"

# 2. Argument Errors
run_test "Argument Error: No arguments" \
         "$EXECUTABLE" \
         "expected_empty.txt" \
         "expected_stderr_error.txt"

run_test "Argument Error: Too many arguments" \
         "$EXECUTABLE \"1 2 +\" \"3 4 -\"" \
         "expected_empty.txt" \
         "expected_stderr_error.txt"

# 3. Expression Errors
run_test "Expression Error: Operand surplus" \
         "$EXECUTABLE \"1 2 3 +\"" \
         "expected_empty.txt" \
         "expected_stderr_error.txt"

run_test "Expression Error: Operand shortage" \
         "$EXECUTABLE \"1 +\"" \
         "expected_empty.txt" \
         "expected_stderr_error.txt"

run_test "Expression Error: Division by zero" \
         "$EXECUTABLE \"5 0 /\"" \
         "expected_empty.txt" \
         "expected_stderr_error.txt"

run_test "Expression Error: Invalid token (parentheses)" \
         "$EXECUTABLE \"(1 + 1)\"" \
         "expected_empty.txt" \
         "expected_stderr_error.txt"

run_test "Expression Error: Invalid token (character)" \
         "$EXECUTABLE \"1 a +\"" \
         "expected_empty.txt" \
         "expected_stderr_error.txt"


# --- Cleanup ---
rm -f "$TMP_OUTPUT" "$TMP_ERROR"
rm -f expected_stdout_42.txt expected_stderr_error.txt expected_empty.txt

# --- Summary ---
echo "-------------------------------------------"
if [ "$PASS_COUNT" -eq "$TEST_COUNT" ]; then
    echo -e "${GREEN}All $TEST_COUNT tests passed successfully!${NC}"
else
    echo -e "${RED}$((TEST_COUNT - PASS_COUNT)) out of $TEST_COUNT tests failed.${NC}"
fi
echo "-------------------------------------------"
