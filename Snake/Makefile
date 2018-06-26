# super simple makefile
# call it using 'make NAME=name_of_code_file_without_extension'
# (assumes a .cpp extension)

CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -Werror=vla -I/opt/X11/include
EXEC = snake

OBJECTS = main.o game.o snake.o
DEPENDS = ${OBJECTS:.o=.d}

	# g++ -o $(NAME) $(NAME).cpp -L/opt/X11/lib -lX11 -lstdc++ $(MAC_OPT)
${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -L/opt/X11/lib -lstdc++ -lX11

-include ${DEPENDS}

run: all
	@echo "Running..."
	./${EXEC}

.PHONY: clean

clean:
	${RM} ${OBJECTS} ${EXEC} ${DEPENDS}
