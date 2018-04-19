build:
	./scripts/build.sh
test: build
	./scripts/tests.sh
coverage:test
	./scripts/gen_coverage.sh
gen_coverage:
	./scripts/gen_coverage.sh
show_coverage: gen_coverage
	firefox ./coverage/index.html/index.html
run:
	valgrind --quiet --leak-check=full ./exe/analyser.out
clear:
	./scripts/clear.sh
