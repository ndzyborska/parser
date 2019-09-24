CC = gcc
LIBS = -lm
CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi $(LIBS)
DEBUG = -g3 -fsanitize=address -fsanitize=undefined
OPTIM = -O2
SDL = `sdl2-config --cflags --libs`
VALGRIND = --error-exitcode=1 --quiet --leak-check=full
PARSER = parse.c testparse.c
SOURCES = interp.c neillsdl2.c testing.c


interp: $(SOURCES)
	$(CC) $(SOURCES) -o interp $(CFLAGS) $(SDL) $(OPTIM)

logo: neillsdl2.c logo.c 
		$(CC) neillsdl2.c logo.c -o logo $(CFLAGS) $(SDL) $(OPTIM)

parse: $(PARSER)
		$(CC) $(PARSER) -o parse $(CFLAGS) $(OPTIM)

runparse: parse
		./parse nest.txt testall

polish: parse
		./parse nest.txt testall polish.txt

debug:  $(SOURCES)
	$(CC) $(SOURCES) -o interp  $(CFLAGS) $(DEBUG)

checkmem: interp.c testing.c
	valgrind $(VALGRIND) ./interp splits.txt val

split: interp
		./interp split.txt testall

splits: interp
		./interp splits.txt testall

circle: interp
			./interp circle.txt testall

nest: interp
		./interp nest.txt testall

clean:
	rm -f interp

.PHONY: clean run all
