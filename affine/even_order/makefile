INTDIR := ${PWD}/src/interface
IMPDIR := ${PWD}/src/implementation
BINDIR := ${PWD}/bin
OBJDIR := ${PWD}/obj
CLIQDIR := ${INTDIR}/cliquer-1.21

CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wconversion -pedantic -O3 -I${INTDIR}/

CC := gcc
CFLAGS := -std=c18 -std=gnu18 -Wall -Wextra -Wconversion -pedantic -O3 -I${INTDIR}/

OBJ := $(addprefix ${OBJDIR}/,matrix.o ag.o)
INT := $(addprefix ${INTDIR}/,main.h matrix.h ag.h)

CLIQOBJ := $(addprefix ${CLIQDIR}/,cliquer.o graph.o reorder.o)

vpath %.c++ ${IMPDIR}/
vpath %.c ${IMPDIR}/

.SILENT:

${OBJDIR}/%.o: %.c++
	${CXX} ${CXXFLAGS} -c $< -o $@

all: transversal raw_search #mutually_orthogoval

transversal: transversal.c++ ${OBJ} ${INT} | ${BINDIR}
	${CXX} ${CXXFLAGS} $< ${OBJ} -o ${BINDIR}/$@

raw_search: raw_search.c++ ${OBJ} ${INT} | ${BINDIR}
	${CXX} ${CXXFLAGS} $< ${OBJ} -o ${BINDIR}/$@

mutually_orthogoval: mutually_orthogoval.c ${CLIQOBJ} | ${BINDIR}
	${CC} ${CFLAGS} $< ${CLIQOBJ} -o ${BINDIR}/$@

${OBJ}: ${INT} | ${OBJDIR}

${BINDIR}:
	mkdir -p ${BINDIR}/

${OBJDIR}:
	mkdir -p ${OBJDIR}/

.PHONY: clean clean-all
clean:
	rm -rf ${OBJDIR}

clean-all: clean
	rm -rf ${BINDIR}
