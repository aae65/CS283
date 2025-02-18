#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Loop exits upon completion" {
    run ./dsh <<EOF
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>cmdloopreturned0"

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
    expected_output="dsh2>dsh2>cmdloopreturned0"

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
    expected_output="dsh2>warning:nocommandsprovideddsh2>cmdloopreturned0"

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
    exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>cmdloopreturned-3"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "echo works" {
    run ./dsh <<EOF
    echo ur mom
    exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>urmomdsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}