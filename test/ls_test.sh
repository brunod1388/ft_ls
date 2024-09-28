#!/bin/bash

# Paths to the commands
FT_CMD="./ft_ls -la"
LS_CMD="/bin/ls -la"

# Directories to test
TEST_DIRS=("." "/bin" "/usr" "/etc")

# Temporary files to store outputs
RESULT_FT="ft_ls_output.txt"
RESULT_LS="real_ls_output.txt"

# Function to run and time a command
run_and_time() {
    local dir=$1
    local out_file=$2
    local cmd=$3

    { time $cmd $dir > $out_file; } 2>&1 | grep real
}

# Run tests
for dir in "${TEST_DIRS[@]}"; do
    echo "Testing directory: $dir"

    # Run and time ft_ls
    echo "ft_ls: "
    run_and_time $dir $RESULT_FT "$FT_CMD"

    # Run and time real ls
    echo "real ls: "
    run_and_time $dir $RESULT_LS "$LS_CMD"

    # Compare outputs
    if diff -q $RESULT_FT $RESULT_LS > /dev/null; then
        echo "Outputs match"
    else
        echo "Outputs differ"
    fi

    echo
done

# Clean up
rm -f $RESULT_FT $RESULT_LS