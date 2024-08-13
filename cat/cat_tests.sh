#!/bin/bash

EXECUTABLE=./s21_cat
INPUT_FILE_1=1.txt
INPUT_FILE_2=2.txt

FLAGS=(-b -e -n -s -t -v --number-nonblank --number --squeeze-blank)

echo "
╔══╗╔══╗╔════╗───╔════╗╔═══╗╔══╗╔════╗╔══╗
║╔═╝║╔╗║╚═╗╔═╝───╚═╗╔═╝║╔══╝║╔═╝╚═╗╔═╝║╔═╝
║║──║╚╝║──║║───────║║──║╚══╗║╚═╗──║║──║╚═╗
║║──║╔╗║──║║───────║║──║╔══╝╚═╗║──║║──╚═╗║
║╚═╗║║║║──║║───────║║──║╚══╗╔═╝║──║║──╔═╝║
╚══╝╚╝╚╝──╚╝───────╚╝──╚═══╝╚══╝──╚╝──╚══╝
"

run_single_file_test() {
    local flag="$@"
    local file="$INPUT_FILE_1"

    cat "${FLAGS[@]}" "$INPUT_FILE_1" > expected_output.txt
    $EXECUTABLE "${FLAGS[@]}" "$INPUT_FILE_1" > output.txt

    diff -u expected_output.txt output.txt
    local result=$?
    if [ $result -eq 0 ]; then
        echo "Test passed: $flag - File: $file"
    else
        echo "Test failed: $flag - File: $file"
    fi

    rm -f output.txt expected_output.txt
}

run_multiple_files_test() {
    local flags="$@"
    local files="$INPUT_FILE_1 $INPUT_FILE_2"

    cat "${FLAGS[@]}" "$INPUT_FILE_1" "$INPUT_FILE_2" > expected_output.txt
    $EXECUTABLE "${FLAGS[@]}" "$INPUT_FILE_1" "$INPUT_FILE_2" > output.txt

    diff -u expected_output.txt output.txt
    local result=$?
    if [ $result -eq 0 ]; then
        echo "Test passed: $flags - Files: $files"
    else
        echo "Test failed: $flags - Files: $files"
    fi

    rm -f output.txt expected_output.txt
}

run_no_flag_single_file_test() {
    local file="$INPUT_FILE_1"

    cat "$INPUT_FILE_1" > expected_output.txt
    $EXECUTABLE "$INPUT_FILE_1" > output.txt

    diff -u expected_output.txt output.txt
    local result=$?
    if [ $result -eq 0 ]; then
        echo "Test passed: No flag - File: $file"
    else
        echo "Test failed: No flag - File: $file"
    fi

    rm -f output.txt expected_output.txt
}

run_no_flag_multiple_files_test() {
    local files="$INPUT_FILE_1 $INPUT_FILE_2"

    cat "$INPUT_FILE_1" "$INPUT_FILE_2" > expected_output.txt
    $EXECUTABLE "$INPUT_FILE_1" "$INPUT_FILE_2" > output.txt

    diff -u expected_output.txt output.txt
    local result=$?
    if [ $result -eq 0 ]; then
        echo "Test passed: No flag - Files: $files"
    else
        echo "Test failed: No flag - Files: $files"
    fi

    rm -f output.txt expected_output.txt
}

for flag in "${FLAGS[@]}"; do
    run_single_file_test "$flag"
done

for ((i = 0; i < ${#FLAGS[@]}; i++)); do
    for ((j = i + 1; j < ${#FLAGS[@]}; j++)); do
        run_single_file_test "${FLAGS[$i]}" "${FLAGS[$j]}"
    done
done

for ((i = 0; i < ${#FLAGS[@]}; i++)); do
    for ((j = i + 1; j < ${#FLAGS[@]}; j++)); do
        for ((k = j + 1; k < ${#FLAGS[@]}; k++)); do
            run_single_file_test "${FLAGS[$i]}" "${FLAGS[$j]}" "${FLAGS[$k]}"
        done
    done
done

for flag in "${FLAGS[@]}"; do
    run_multiple_files_test "$flag"
done

for ((i = 0; i < ${#FLAGS[@]}; i++)); do
    for ((j = i + 1; j < ${#FLAGS[@]}; j++)); do
        run_multiple_files_test "${FLAGS[$i]}" "${FLAGS[$j]}"
    done
done

for ((i = 0; i < ${#FLAGS[@]}; i++)); do
    for ((j = i + 1; j < ${#FLAGS[@]}; j++)); do
        for ((k = j + 1; k < ${#FLAGS[@]}; k++)); do
            run_multiple_files_test "${FLAGS[$i]}" "${FLAGS[$j]}" "${FLAGS[$k]}"
        done
    done
done

run_no_flag_single_file_test

run_no_flag_multiple_files_test