#!/bin/bash

EXECUTABLE=./s21_grep
INPUT_FILE_1=1.txt
INPUT_FILE_2=2.txt

FLAGS=(-e -i -c -v -l -n -s -h -o)

echo "
╔═══╗╔═══╗╔═══╗╔═══╗───╔════╗╔═══╗╔══╗╔════╗╔══╗
║╔══╝║╔═╗║║╔══╝║╔═╗║───╚═╗╔═╝║╔══╝║╔═╝╚═╗╔═╝║╔═╝
║║╔═╗║╚═╝║║╚══╗║╚═╝║─────║║──║╚══╗║╚═╗──║║──║╚═╗
║║╚╗║║╔╗╔╝║╔══╝║╔══╝─────║║──║╔══╝╚═╗║──║║──╚═╗║
║╚═╝║║║║║─║╚══╗║║────────║║──║╚══╗╔═╝║──║║──╔═╝║
╚═══╝╚╝╚╝─╚═══╝╚╝────────╚╝──╚═══╝╚══╝──╚╝──╚══╝
"

conflict_flags() {
    local flag1="$1"
    local flag2="$2"

    if [[ "$flag1$flag2" == *-e-* ]]; then
        return 0
    else
        return 1
    fi
}

run_single_file_test() {
    local flag="$1"
    local template="test"

    grep "$flag" "$template" "$INPUT_FILE_1" > "expected_output.txt"
    ./s21_grep "$flag" "$template" "$INPUT_FILE_1" > "output.txt"

    diff -u expected_output.txt output.txt
    local result=$?
    if [ $result -eq 0 ]; then
        echo "Test passed: $flag - File: $INPUT_FILE_1"
    else
        echo "Test failed: $flag - File: $INPUT_FILE_1"
    fi

    rm -f output.txt expected_output.txt
}

run_multiple_files_test() {
    local flag1="$1"
    local flag2="$2"
    local template="test"

    if conflict_flags "$flag1" "$flag2"; then
        return
    fi

    grep "$flag1" "$flag2" "$template" "$INPUT_FILE_1" "$INPUT_FILE_2" > "expected_output.txt"
    ./s21_grep "$flag1" "$flag2" "$template" "$INPUT_FILE_1" "$INPUT_FILE_2" > "output.txt"

    diff -u expected_output.txt output.txt
    local result=$?
    if [ $result -eq 0 ]; then
        echo "Test passed: $flag1 $flag2 - Files: $INPUT_FILE_1 $INPUT_FILE_2"
    else
        echo "Test failed: $flag1 $flag2 - Files: $INPUT_FILE_1 $INPUT_FILE_2"
    fi

    rm -f "expected_output.txt" "output.txt"
}

run_no_flag_single_file_test() {
    local template="test"

    grep "$template" "$INPUT_FILE_1" > "expected_output.txt"
    ./s21_grep "$template" "$INPUT_FILE_1" > "output.txt"

    diff -u expected_output.txt output.txt
    local result=$?
    if [ $result -eq 0 ]; then
        echo "Test passed: No flag - File: $INPUT_FILE_1"
    else
        echo "Test failed: No flag - File: $INPUT_FILE_1"
    fi

    rm -f output.txt expected_output.txt
}

run_no_flag_multiple_files_test() {
    local template="test"

    grep "$template" "$INPUT_FILE_1" "$INPUT_FILE_2" > "expected_output.txt"
    ./s21_grep "$template" "$INPUT_FILE_1" "$INPUT_FILE_2" > "output.txt"

    diff -u expected_output.txt output.txt
    local result=$?
    if [ $result -eq 0 ]; then
        echo "Test passed: No flag - Files: $INPUT_FILE_1 $INPUT_FILE_2"
    else
        echo "Test failed: No flag - Files: $INPUT_FILE_1 $INPUT_FILE_2"
    fi

    rm -f "expected_output.txt" "output.txt"
}

for flag in "${FLAGS[@]}"; do
    run_single_file_test "$flag"
done

for ((i = 0; i < ${#FLAGS[@]}; i++)); do
    for ((j = i + 1; j < ${#FLAGS[@]}; j++)); do
        run_multiple_files_test "${FLAGS[$i]}" "${FLAGS[$j]}"
    done
done

run_no_flag_single_file_test

run_no_flag_multiple_files_test