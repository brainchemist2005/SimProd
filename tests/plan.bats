setup() {
    DIR="$( cd "$( dirname "$BATS_TEST_FILENAME" )" >/dev/null 2>&1 && pwd )"
    PATH="$DIR/../src:$PATH"
    load '../external/bats-support/load'
    load '../external/bats-assert/load'
}

function no_stderr {
    "$@" 2>/dev/null
}

# Basic usage
# -----------

@test "simprod plan succeeds" {
    run ./simprod plan
    assert_success
}

@test "simprod plan prints an empty JSON plan" {
    run ./simprod plan
    assert_line "{"
    assert_line --partial '"productions": {}'
    assert_line "}"
}

@test "simprod plan examples/plan.json prints the content of plan.json" {
    ./simprod plan examples/plan.json > $BATS_TMPDIR/plan.json
    diff -s examples/plan.json $BATS_TMPDIR/plan.json
}

# With wrong argument
# -------------------

@test "simprod plan with too many arguments fails" {
    run ./simprod plan a b
    assert_failure
    assert_line --partial 'Too many arguments'
}
