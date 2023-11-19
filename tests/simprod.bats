setup() {
    DIR="$( cd "$( dirname "$BATS_TEST_FILENAME" )" >/dev/null 2>&1 && pwd )"
    PATH="$DIR/../src:$PATH"
    load '../external/bats-support/load'
    load '../external/bats-assert/load'
}

function no_stderr {
    "$@" 2>/dev/null
}

# Without argument
# ----------------

@test "simprod without argument succeeds" {
    run ./simprod
    assert_success
}

@test "simprod without argument prints the program name" {
    run ./simprod
    assert_line --partial "NAME"
    assert_line --partial "simprod"
}

@test "simprod without argument prints the synopsis" {
    run ./simprod
    assert_line --partial "SYNOPSIS"
}

@test "simprod without argument prints a description" {
    run ./simprod
    assert_line --partial "DESCRIPTION"
}

@test "simprod without argument prints help about subcommand plan" {
    run ./simprod
    assert_line --partial "target is 'plan'"
}

@test "simprod without argument prints help about subcommand scenario" {
    run ./simprod
    assert_line --partial "target is 'scenario'"
}

# With wrong argument
# -------------------

@test "simprod with wrong first argument fails" {
    run ./simprod wrong-argument
    assert_failure
}

@test "simprod with wrong first argument reports error" {
    run ./simprod wrong-argument
    assert_line --partial "Unrecognized target"
}

@test "simprod with wrong first argument prints help" {
    run ./simprod wrong-argument
    assert_line --partial "NAME"
    assert_line --partial "SYNOPSIS"
    assert_line --partial "DESCRIPTION"
}
