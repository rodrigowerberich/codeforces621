SEARCH_DIR="tests"
for TEST_FILE in "$SEARCH_DIR"/*.test
do
    echo "Testing file:  $TEST_FILE"
    cat "$TEST_FILE"
    OUTPUT="$(cat "$TEST_FILE" | ./bin/cow)" 
    echo "$OUTPUT"
done
