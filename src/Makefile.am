DIST_SUBDIRS = duplitrace_cli duplitrace_indexer

SUBDIRS =

if BUILD_CLI
SUBDIRS += duplitrace_cli
endif

if BUILD_SERVICE
SUBDIRS += duplitrace_indexer
endif

if BUILD_SERVICE_UNITTESTS
SUBDIRS += unittests_service
endif
