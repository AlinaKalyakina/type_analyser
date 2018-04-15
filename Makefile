build:
	./scripts/build.sh
test: build
	./scripts/tests.sh
gen_coverage:test
	./scripts/gen_coverage.sh
show_coverage: gen_coverage
	firefox ./coverage/index.html/index.html
clear:
	./scripts/clear.sh
