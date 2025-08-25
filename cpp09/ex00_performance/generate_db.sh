#!/bin/bash

# This script generates a large dummy data.csv file for performance testing.
# Usage: ./generate_db.sh <number_of_rows> > data.csv

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number_of_rows>" >&2
    exit 1
fi

NUM_ROWS=$1

# Print header
echo "date,exchange_rate"

# Use awk for efficient date generation and random numbers
# It starts from 2000-01-01 and increments the day for each row.
awk -v rows="$NUM_ROWS" '
BEGIN {
    srand();
    # Initial date components
    year = 2000;
    month = 1;
    day = 1;
    rate = 500.0;

    for (i = 1; i <= rows; i++) {
        # Print current row
        printf("%04d-%02d-%02d,%.2f\n", year, month, day, rate);

        # Increment day
        day++;
        rate += (rand() - 0.5) * 10;
        if (rate < 0) { rate = 0; }

        # Handle month and year rollovers (simplified, doesn''t account for leap years perfectly but good enough for test data)
        days_in_month = 31;
        if (month == 2) { days_in_month = 28; }
        if (month == 4 || month == 6 || month == 9 || month == 11) { days_in_month = 30; }

        if (day > days_in_month) {
            day = 1;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }
}'
