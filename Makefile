CC = gcc
CXX = g++
CFLAGS = -g -O2
CXXFLAGS = -g -O2

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

HEADERS = $(wildcard *.h)
HEADERS += $(wildcard *.hpp)

EXE = edge

all : ${EXE}

${EXE} : ${OBJS}
	${CXX} $^ -o $@

${OBJS} : %.o : %.cpp ${HEADERS} Makefile
	${CXX} -c $< -o $@

clean :
	rm -rf edge ${OBJS}