INCLUDES = -I. -I../common -I../3rd_party/inireader
INCLUDES += -I$(GOOGLETEST_INCLUDE)

CPPFLAGS = -Wall $(INCLUDES) -std=c++17 -Wall -Wextra -fprofile-arcs -ftest-coverage

LIBS=-L$(GOOGLETEST_LIB) -lgtest -lgcov

BINARY = ./unittests_common

OBJS = ConfigManagerTests.o \
	   main.o \
	   ../common/ConfigManager.o \
	   ../common/ConfigSetup.o \
	   ../common/ConfigSetupItem.o \
	   ../common/Platform.o \
	   ../common/Utilities.o \

all: $(BINARY)

clean:
	$(RM) $(DUPLITRACE_OUTDIR)/$(BINARY) $(OBJS)

$(BINARY): $(OBJS)
	@mkdir -p $(DUPLITRACE_OUTDIR)
	g++ -o $(DUPLITRACE_OUTDIR)/$(BINARY) $(INCLUDES) $(OBJS) $(LIBS)
	cp test_configs/*.cfg $(DUPLITRACE_OUTDIR)
