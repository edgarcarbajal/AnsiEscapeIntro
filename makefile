CXXFLAGS+=-std=c++17 -Wall -Wextra -Werror -pedantic -I.
NCURSESLINK=-lncurses

all: ansiescape_intro

# ($^) brings all listed files after the colon as arguments for the g++ cmd
ansiescape_intro: ansiescape_intro.cc
	g++ $(CXXFLAGS) $(NCURSESLINK) -o ansiescape_intro $^


# removes any object files and executables when called
clean:
	rm -f ansiescape_intro *.o 