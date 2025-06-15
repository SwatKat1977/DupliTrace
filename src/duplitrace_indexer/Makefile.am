if BUILD_SERVICE

# Compiler and linker flags
AM_CPPFLAGS = -g -std=c++17 -Wall -Wextra -fPIC \
              -I. \
              -I$(DUPLITRACE_ARGPARSE_INCLUDE) \
              -I$(DUPLITRACE_SPDLOG_INCLUDE)

bin_PROGRAMS = duplitrace_svc
duplitrace_svc_SOURCES = main.cpp

.PHONY: copy-bin
copy-bin:
	mkdir -p $(top_builddir)/bin
	cp duplitrace_svc $(top_builddir)/bin/

all-local: copy-bin

endif
