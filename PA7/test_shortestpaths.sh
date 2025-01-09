#!/bin/bash

targetfile=shortestpaths.cpp
cppfile=shortestpaths.cpp
maxtime=5

if [ ! -f "$targetfile" ]; then
    echo "Error: file $targetfile not found"
    echo "Final score: score - penalties = 0 - 0 = 0"
    exit 1
fi

# Required by the Honor System
missing_name=0
head -n 20 "$targetfile" | egrep -i "author.*[a-zA-Z]+"
if [ $? -ne 0 ]; then
    echo "Student name missing"
    missing_name=1
fi

# Required by the Honor System
missing_pledge=0
head -n 20 "$targetfile" | egrep -i "I.*pledge.*my.*honor.*that.*I.*have.*abided.*by.*the.*Stevens.*Honor.*System"
if [ $? -ne 0 ]; then
    echo -e "Pledge missing"
    missing_pledge=1
fi

distributor="Ubuntu"
release="22.04"
which lsb_release &> /dev/null
if [ "$?" -ne 0 ] \
    || [ "$(lsb_release -i -s)" != "$distributor" \
      -o "$(lsb_release -r -s)" != "$release" ]; then
    echo "WARNING: this script must be executed on $distributor $release!"
fi

# Compiling
echo
results=$(make 2>&1)
if [ $? -ne 0 ]; then
    echo "$results"
    echo "Final score: score - penalties = 0 - 0 = 0"
    exit 1
fi

num_tests=0
num_right=0
memory_problems=0
command="./${cppfile%.*}"

run_test_with_args_and_input() {
    ((num_tests++))
    echo -n "Test $num_tests..."

    args="$1"
    input="$2"
    expected_output="$3"

    outputfile=$(mktemp)
    inputfile=$(mktemp)
    statusfile=$(mktemp)

    echo -e "$input" > "$inputfile"

    start=$(date +%s.%N)
    # Run run run, little program!
    (timeout --preserve-status "$maxtime" "$command" $args < "$inputfile" &> "$outputfile"; echo $? > "$statusfile") &> /dev/null
    end=$(date +%s.%N)
    status=$(cat "$statusfile")

    case $status in
        # $command: 128 + SIGBUS = 128 + 7 = 135 (rare on x86)
        135)
            echo "failed (bus error)"
            ;;
        # $command: 128 + SIGSEGV = 128 + 11 = 139
        139)
            echo "failed (segmentation fault)"
            ;;
        # $command: 128 + SIGTERM (sent by timeout(1)) = 128 + 15 = 143
        143)
            echo "failed (time out)"
            ;;
        *)
            # bash doesn't like null bytes so we substitute by hand.
            computed_output=$(sed -e 's/\x0/(NULL BYTE)/g' "$outputfile")
            if [ "$computed_output" = "$expected_output" ]; then
                ((num_right++))
                echo $start $end | awk '{printf "ok (%.3fs)\tvalgrind...", $2 - $1}'
                # Why 93?  Why not 93!
                (valgrind --leak-check=full --error-exitcode=93 $command $args < "$inputfile" &> /dev/null; echo $? > "$statusfile") &> /dev/null
                vgstatus=$(cat "$statusfile")
                case $vgstatus in
                    # valgrind detected an error when running $command
                    93)
                        ((memory_problems++))
                        echo "failed"
                        ;;
                    # valgrind not installed or not in $PATH
                    127)
                        echo "not found"
                        ;;
                    # valgrind: 128 + SIGBUS = 128 + 7 = 135 (rare on x86)
                    135)
                        ((memory_problems++))
                        echo "failed (bus error)"
                        ;;
                    # valgrind: 128 + SIGSEGV = 128 + 11 = 139
                    139)
                        ((memory_problems++))
                        echo "failed (segmentation fault)"
                        ;;
                    # compare with expected status from running $command without valgrind
                    $status)
                        echo "ok"
                        ;;
                    *)
                        ((memory_problems++))
                        echo "unknown status $vgstatus"
                        ;;
                esac
            else
                echo "failed"
                echo "==================== Expected ===================="
                echo "$expected_output"
                echo "==================== Received ===================="
                echo "$computed_output"
                echo "=================================================="
            fi
            ;;
    esac
    rm -f "$inputfile" "$outputfile" "$statusfile"
}

run_test_with_args() {
    run_test_with_args_and_input "$1" "" "$2"
}
run_test_with_input() {
    run_test_with_args_and_input "" "$1" "$2"
}

############################################################
graphfile=$(mktemp)

# Test 1
run_test_with_args "" "Usage: ./shortestpaths <filename>"

# Test 2
run_test_with_args "notfound.txt" "Error: Cannot open file 'notfound.txt'."

# Test 3
cat << ENDOFINPUT > "$graphfile"
0
X Y 1
Y Z 4
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Invalid number of vertices '0' on line 1."

# Test 4
cat << ENDOFINPUT > "$graphfile"
three
X Y 1
Y Z 4
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Invalid number of vertices 'three' on line 1."

# Test 5
cat << ENDOFINPUT > "$graphfile"
4
A C 2
A D 10
B A 7
B C
B D 1
C B 3
D A 2
D B 6
D C 6
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Invalid edge data 'B C' on line 5."

# Test 6
cat << ENDOFINPUT > "$graphfile"
3
X Y 1
Y Z 4
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Starting vertex 'X' on line 2 is not among valid values A-C."

# Test 7
cat << ENDOFINPUT > "$graphfile"
4
A C 2
A D 10
B A 7
B C 5
b D 1
C B 3
D A 2
D B 6
D C 6
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Starting vertex 'b' on line 6 is not among valid values A-D."

# Test 8
cat << ENDOFINPUT > "$graphfile"
4
A C 2
A D 10
B A 7
B C 5
Hoboken Weehawken 1
C B 3
D A 2
D B 6
D C 6
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Starting vertex 'Hoboken' on line 6 is not among valid values A-D."

# Test 9
cat << ENDOFINPUT > "$graphfile"
6
A C 2
A . 10
B A 7
B C 5
B D 1
C B 3
D A 2
D B 6
D C 6
E F 8
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Ending vertex '.' on line 3 is not among valid values A-F."

# Test 10
cat << ENDOFINPUT > "$graphfile"
4
A C 2
A D 10
B A 7
B C 5
B D 1
C B 3
D A 2
D B 6
D NYC 6
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Ending vertex 'NYC' on line 10 is not among valid values A-D."

# Test 11
cat << ENDOFINPUT > "$graphfile"
4
A C 2
A D 10
B A -7
B C 5
B D 1
C B 3
D A 2
D B 6
D C 6
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Invalid edge weight '-7' on line 4."

# Test 12
cat << ENDOFINPUT > "$graphfile"
4
A C 2
A D 10
B A 7
B C 5
B D 1
C B 3
D A 0
D B 6
D C 6
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Invalid edge weight '0' on line 8."

# Test 13
cat << ENDOFINPUT > "$graphfile"
4
A C 2
A D 10
B A 7
B C 5
B D 1
C B 3
D A 2
D B sixteen
D C 6
ENDOFINPUT
run_test_with_args "$graphfile" "Error: Invalid edge weight 'sixteen' on line 9."

# Test 14
cat << ENDOFINPUT > "$graphfile"
1
ENDOFINPUT
run_test_with_args "$graphfile" "Distance matrix:
  A
A 0

Path lengths:
  A
A 0

Intermediate vertices:
  A
A -

A -> A, distance: 0, path: A"

# Test 15
cat << ENDOFINPUT > "$graphfile"
4
A C 2
A D 10
B A 7
B C 5
B D 1
C B 3
D A 2
D B 6
D C 6
ENDOFINPUT
run_test_with_args "$graphfile" "Distance matrix:
   A  B  C  D
A  0  -  2 10
B  7  0  5  1
C  -  3  0  -
D  2  6  6  0

Path lengths:
  A B C D
A 0 5 2 6
B 3 0 5 1
C 6 3 0 4
D 2 6 4 0

Intermediate vertices:
  A B C D
A - C - C
B D - - -
C D - - B
D - - A -

A -> A, distance: 0, path: A
A -> B, distance: 5, path: A -> C -> B
A -> C, distance: 2, path: A -> C
A -> D, distance: 6, path: A -> C -> B -> D
B -> A, distance: 3, path: B -> D -> A
B -> B, distance: 0, path: B
B -> C, distance: 5, path: B -> C
B -> D, distance: 1, path: B -> D
C -> A, distance: 6, path: C -> B -> D -> A
C -> B, distance: 3, path: C -> B
C -> C, distance: 0, path: C
C -> D, distance: 4, path: C -> B -> D
D -> A, distance: 2, path: D -> A
D -> B, distance: 6, path: D -> B
D -> C, distance: 4, path: D -> A -> C
D -> D, distance: 0, path: D"

# Test 16
cat << ENDOFINPUT > "$graphfile"
5
A C 2
A D 10
A E 13
B A 7
B C 5
B D 1
B E 5
C B 3
C D 4
D A 2
D B 6
D C 6
D E 8
ENDOFINPUT
run_test_with_args "$graphfile" "Distance matrix:
   A  B  C  D  E
A  0  -  2 10 13
B  7  0  5  1  5
C  -  3  0  4  -
D  2  6  6  0  8
E  -  -  -  -  0

Path lengths:
   A  B  C  D  E
A  0  5  2  6 10
B  3  0  5  1  5
C  6  3  0  4  8
D  2  6  4  0  8
E  -  -  -  -  0

Intermediate vertices:
  A B C D E
A - C - C C
B D - - - -
C D - - - B
D - - A - -
E - - - - -

A -> A, distance: 0, path: A
A -> B, distance: 5, path: A -> C -> B
A -> C, distance: 2, path: A -> C
A -> D, distance: 6, path: A -> C -> D
A -> E, distance: 10, path: A -> C -> B -> E
B -> A, distance: 3, path: B -> D -> A
B -> B, distance: 0, path: B
B -> C, distance: 5, path: B -> C
B -> D, distance: 1, path: B -> D
B -> E, distance: 5, path: B -> E
C -> A, distance: 6, path: C -> D -> A
C -> B, distance: 3, path: C -> B
C -> C, distance: 0, path: C
C -> D, distance: 4, path: C -> D
C -> E, distance: 8, path: C -> B -> E
D -> A, distance: 2, path: D -> A
D -> B, distance: 6, path: D -> B
D -> C, distance: 4, path: D -> A -> C
D -> D, distance: 0, path: D
D -> E, distance: 8, path: D -> E
E -> A, distance: infinity, path: none
E -> B, distance: infinity, path: none
E -> C, distance: infinity, path: none
E -> D, distance: infinity, path: none
E -> E, distance: 0, path: E"

# Test 17
cat << ENDOFINPUT > "$graphfile"
5
A C 2
A E 13
B A 7
B C 5
B D 2147483647
B E 5
C B 3
D A 2
D B 6
D C 6
D E 8
E B 2147483647
ENDOFINPUT
run_test_with_args "$graphfile" "Distance matrix:
           A          B          C          D          E
A          0          -          2          -         13
B          7          0          5 2147483647          5
C          -          3          0          -          -
D          2          6          6          0          8
E          - 2147483647          -          -          0

Path lengths:
           A          B          C          D          E
A          0          5          2 2147483652         10
B          7          0          5 2147483647          5
C         10          3          0 2147483650          8
D          2          6          4          0          8
E 2147483654 2147483647 2147483652 4294967294          0

Intermediate vertices:
  A B C D E
A - C - C C
B - - - - -
C B - - B B
D - - A - -
E B - B B -

A -> A, distance: 0, path: A
A -> B, distance: 5, path: A -> C -> B
A -> C, distance: 2, path: A -> C
A -> D, distance: 2147483652, path: A -> C -> B -> D
A -> E, distance: 10, path: A -> C -> B -> E
B -> A, distance: 7, path: B -> A
B -> B, distance: 0, path: B
B -> C, distance: 5, path: B -> C
B -> D, distance: 2147483647, path: B -> D
B -> E, distance: 5, path: B -> E
C -> A, distance: 10, path: C -> B -> A
C -> B, distance: 3, path: C -> B
C -> C, distance: 0, path: C
C -> D, distance: 2147483650, path: C -> B -> D
C -> E, distance: 8, path: C -> B -> E
D -> A, distance: 2, path: D -> A
D -> B, distance: 6, path: D -> B
D -> C, distance: 4, path: D -> A -> C
D -> D, distance: 0, path: D
D -> E, distance: 8, path: D -> E
E -> A, distance: 2147483654, path: E -> B -> A
E -> B, distance: 2147483647, path: E -> B
E -> C, distance: 2147483652, path: E -> B -> C
E -> D, distance: 4294967294, path: E -> B -> D
E -> E, distance: 0, path: E"

# Test 18
cat << ENDOFINPUT > "$graphfile"
7
A B 11
A D 13
B A 7
B C 4
D E 8
D G 9
E A 9
E F 10
E G 2
F B 12
F C 19
G E 29
G C 5
ENDOFINPUT
run_test_with_args "$graphfile" "Distance matrix:
   A  B  C  D  E  F  G
A  0 11  - 13  -  -  -
B  7  0  4  -  -  -  -
C  -  -  0  -  -  -  -
D  -  -  -  0  8  -  9
E  9  -  -  -  0 10  2
F  - 12 19  -  -  0  -
G  -  -  5  - 29  -  0

Path lengths:
   A  B  C  D  E  F  G
A  0 11 15 13 21 31 22
B  7  0  4 20 28 38 29
C  -  -  0  -  -  -  -
D 17 28 14  0  8 18  9
E  9 20  7 22  0 10  2
F 19 12 16 32 40  0 41
G 38 49  5 51 29 39  0

Intermediate vertices:
  A B C D E F G
A - - B - D E D
B - - - A D E D
C - - - - - - -
D E E G - - E -
E - A G A - - -
F B - B B D - D
G E E - E - E -

A -> A, distance: 0, path: A
A -> B, distance: 11, path: A -> B
A -> C, distance: 15, path: A -> B -> C
A -> D, distance: 13, path: A -> D
A -> E, distance: 21, path: A -> D -> E
A -> F, distance: 31, path: A -> D -> E -> F
A -> G, distance: 22, path: A -> D -> G
B -> A, distance: 7, path: B -> A
B -> B, distance: 0, path: B
B -> C, distance: 4, path: B -> C
B -> D, distance: 20, path: B -> A -> D
B -> E, distance: 28, path: B -> A -> D -> E
B -> F, distance: 38, path: B -> A -> D -> E -> F
B -> G, distance: 29, path: B -> A -> D -> G
C -> A, distance: infinity, path: none
C -> B, distance: infinity, path: none
C -> C, distance: 0, path: C
C -> D, distance: infinity, path: none
C -> E, distance: infinity, path: none
C -> F, distance: infinity, path: none
C -> G, distance: infinity, path: none
D -> A, distance: 17, path: D -> E -> A
D -> B, distance: 28, path: D -> E -> A -> B
D -> C, distance: 14, path: D -> G -> C
D -> D, distance: 0, path: D
D -> E, distance: 8, path: D -> E
D -> F, distance: 18, path: D -> E -> F
D -> G, distance: 9, path: D -> G
E -> A, distance: 9, path: E -> A
E -> B, distance: 20, path: E -> A -> B
E -> C, distance: 7, path: E -> G -> C
E -> D, distance: 22, path: E -> A -> D
E -> E, distance: 0, path: E
E -> F, distance: 10, path: E -> F
E -> G, distance: 2, path: E -> G
F -> A, distance: 19, path: F -> B -> A
F -> B, distance: 12, path: F -> B
F -> C, distance: 16, path: F -> B -> C
F -> D, distance: 32, path: F -> B -> A -> D
F -> E, distance: 40, path: F -> B -> A -> D -> E
F -> F, distance: 0, path: F
F -> G, distance: 41, path: F -> B -> A -> D -> G
G -> A, distance: 38, path: G -> E -> A
G -> B, distance: 49, path: G -> E -> A -> B
G -> C, distance: 5, path: G -> C
G -> D, distance: 51, path: G -> E -> A -> D
G -> E, distance: 29, path: G -> E
G -> F, distance: 39, path: G -> E -> F
G -> G, distance: 0, path: G"

rm -f "$graphfile"
############################################################
echo
echo "Total tests run: $num_tests"
echo "Number correct : $num_right"
score=$((100 * $num_right / $num_tests))
echo "Percent correct: $score%"
if [ $missing_name == 1 ]; then
    echo "Missing Name: -5"
fi
if [ $missing_pledge == 1 ]; then
    echo "Missing or incorrect pledge: -5"
fi

if [ $memory_problems -gt 1 ]; then
    echo "Memory problems: $memory_problems (-5 each, max of -15)"
    if [ $memory_problems -gt 3 ]; then
        memory_problems=3
    fi
fi

penalties=$((5 * $missing_name + 5 * $missing_pledge + 5 * $memory_problems))
final_score=$(($score - $penalties))
if [ $final_score -lt 0 ]; then
    final_score=0
fi
echo "Final score: score - penalties = $score - $penalties = $final_score"

make clean > /dev/null 2>&1
