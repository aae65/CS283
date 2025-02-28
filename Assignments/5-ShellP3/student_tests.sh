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
    expected_output="dsh3>cmdloopreturned0"

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
    expected_output="dsh3>dsh3>cmdloopreturned0"

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
    expected_output="dsh3>warning:nocommandsprovideddsh3>cmdloopreturned0"

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
    expected_output="dsh3>cmdloopreturned-3"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "echo handles unquoted spaces" {
    run "./dsh" <<EOF
    echo ur mom
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="urmomdsh3>dsh3>cmdloopreturned0"

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
    expected_output=$(pwd)"dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}
