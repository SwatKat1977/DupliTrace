if BUILD_SERVICE

bin_PROGRAMS = duplitrace_svc
duplitrace_svc_SOURCES = main.cpp

.PHONY: copy-bin
copy-bin:
	mkdir -p $(top_builddir)/bin
	cp duplitrace_svc $(top_builddir)/bin/

all-local: copy-bin

endif
