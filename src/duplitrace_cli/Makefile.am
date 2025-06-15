if BUILD_CLI

bin_PROGRAMS = duplitrace_cli
duplitrace_cli_SOURCES = main.cpp


.PHONY: copy-bin
copy-bin:
	mkdir -p $(top_builddir)/bin
	mv duplitrace_cli $(top_builddir)/bin/

all-local: copy-bin

endif
