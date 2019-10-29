ifeq ($(OS), Windows_NT)
	suffix = .exe
else
	suffix = .out
endif

all: $(patsubst %.cpp, %$(suffix), $(wildcard *.cpp))

%$(suffix): %.cpp
	g++ -I ../include -o $@ $<
