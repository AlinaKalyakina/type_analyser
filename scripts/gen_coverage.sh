#! /bin/bash
EXE=./exe/
COV=./coverage/
LCOV=./lcov/bin/
    git clone https://github.com/linux-test-project/lcov.git
	mkdir -p $COV
    $LCOV/lcov --directory $EXE --capture --output-file $COV/index.info --rc lcov_branch_coverage=1
    $LCOV/genhtml -o $COV/index.html $COV/index.info --branch-coverage
    cd ..
