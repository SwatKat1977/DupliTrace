if BUILD_CLI

bin_PROGRAMS = unittests_service

# Compiler and linker flags
AM_CPPFLAGS = -g -std=c++17 -Wall -Wextra -fPIC  \
              -I.                                \
              -I../duplitrace_indexer            \
              -I$(DUPLITRACE_GOOGLETEST_INCLUDE)

unittests_service_LDADD = -L$(DUPLITRACE_GOOGLETEST_LIB) \
                          -lgtest \
                          -lgtest_main \
                          -lpthread

unittests_service_SOURCES = main.cpp

.PHONY: copy-bin
copy-bin:
	mkdir -p $(top_builddir)/bin
	mv unittests_service $(top_builddir)/bin/

all-local: copy-bin

endif
