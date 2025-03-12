#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

#previous made tests to ensure code still works
@test "Loop exits upon completion" {
    run ./dsh <<EOF
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="localmodedsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}

@test "Empty cd command does nothing" {
    run ./dsh <<EOF
cd
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="localmodedsh4>dsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "No commands is warned" {
    run ./dsh <<EOF

exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="localmodedsh4>warning:nocommandsprovideddsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Too many arguments" {
    run ./dsh <<EOF
    test1 a1 a2 a3 a4 a5 a6 a7 a8 a9
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="localmodedsh4>cmdloopreturned-3"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "pwd works" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
    pwd
EOF

    stripped_output=$(echo $output | tr -d '[:space:]')
    expected_output=$(pwd)"localmodedsh4>dsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}