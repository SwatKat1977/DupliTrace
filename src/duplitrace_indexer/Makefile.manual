INCLUDES = -I. -I../common -I../3rd_party/inireader
INCLUDES += -I$(DUPLITRACE_ARGPARSE_INCLUDE)
INCLUDES += -I$(DUPLITRACE_SPDLOG_INCLUDE)

CPPFLAGS = -Wall $(INCLUDES) -std=c++17 -Wall -Wextra

# Conditionally add the -DDEBUG_INDEXER flag to CPPFLAGS
ifeq ($(DEBUG_INDEXER), 1)
    CPPFLAGS += -g
endif

BINARY = ./duplitrace_indexer

$(BINARY): $(OBJS)
	g++ -o $(BINARY) $(INCLUDES) $(OBJS) $(LIBS)

OBJS = Service.o \
	   main.o \
	   ../common/ConfigManager.o \
	   ../common/ConfigSetup.o \
	   ../common/ConfigSetupItem.o \
	   ../common/Platform.o \
	   ../common/Utilities.o \
	   ../cron_parser/CronParser.o

all: $(BINARY)

clean:
	$(RM) $(BINARY) $(OBJS)

BINARY = ./duplitrace_indexer

$(BINARY): $(OBJS)
	echo $(INCLUDES)
	g++ -o $(BINARY) $(INCLUDES) $(OBJS) $(LIBS)
