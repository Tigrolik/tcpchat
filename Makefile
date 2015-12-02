# ------------------------------ VARIABLES ------------------------------ #

# Compiler to use
COMPILER = gcc
# Flags for compiler
CFLAGS   = -Wall -pedantic -std=gnu99
# Define libraries to link into executable
# LIBS     = -lm
# Flags for debugging (combine multiple flags with spaces)
DFLAGS   = -D__DEBUG_EN -g
# Source files (only .c, no .h)
SOURCES_GEN = tcputils.c sigutils.c
SOURCES_SERVER = serverTCP.c
SOURCES_CLIENT = clientTCP.c
# Output file
SERVERNAME = serverTCP
CLIENTNAME = clientTCP

# ------------------------------ COMMANDS ------------------------------ #

# Build all ("make")
all: build

server:
	$(COMPILER) $(SOURCES_GEN) $(SOURCES_SERVER) $(CFLAGS) -o $(SERVERNAME)

client:
	$(COMPILER) $(SOURCES_GEN) $(SOURCES_CLIENT) $(CFLAGS) -o $(CLIENTNAME)

# Build application "make build"
build:
	$(COMPILER) $(SOURCES_GEN) $(SOURCES_SERVER) $(CFLAGS) -o $(SERVERNAME)
	$(COMPILER) $(SOURCES_GEN) $(SOURCES_CLIENT) $(CFLAGS) -o $(CLIENTNAME)

# Build application with debugging info # "make debug"
debug:
	$(COMPILER) $(SOURCES_GEN) $(SOURCES_SERVER) $(CFLAGS) $(DFLAGS) -o
	$(SERVERNAME)
	$(COMPILER) $(SOURCES_GEN) $(SOURCES_CLIENT) $(CFLAGS) $(DFLAGS) -o
	$(CLIENTNAME)

# Remove generated files: "make clean"
clean:
	@if [ $(SERVERNAME) ] ; then rm -rf $(SERVERNAME) ; fi
	@if [ $(CLIENTNAME) ] ; then rm -rf $(CLIENTNAME) ; fi

