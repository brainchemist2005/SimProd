setup() {
    dir="$( cd "$( dirname "$BATS_TEST_FILENAME" )" >/dev/null 2>&1 && pwd )"
    PATH="$dir/../src:$PATH"
    load '../external/bats-support/load'
    load '../external/bats-assert/load'
}

function no_stderr {
    "$@" 2>/dev/null
}

# Basic usage
# -----------

@test "simprod scenario succeeds" {
    run ./simprod scenario
    assert_success
}

@test "simprod scenario prints an empty JSON scenario" {
    run ./simprod scenario
    assert_line "{"
    assert_line --partial '"timeline": {'
    assert_line --partial '"future-durations": []'
    assert_line --partial '"plants": []'
    assert_line --partial '"zones": []'
    assert_line "}"
}

@test "simprod scenario scenario.json prints the content of scenario.json" {
    ./simprod scenario examples/scenario.json > $BATS_TMPDIR/scenario.json
    diff -s examples/scenario.json $BATS_TMPDIR/scenario.json
}

# With wrong argument
# -------------------

@test "simprod scenario with too many arguments fails" {
    run ./simprod scenario a b
    assert_failure
    assert_line --partial 'Too many arguments'
}

@test "simprod scenario with invalid JSON filepath fails" {
    run ./simprod scenario non-existing.json
    assert_failure
    assert_line --partial 'No such file'
}
