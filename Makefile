CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
CXXFILES = FireWallManager.cpp  main.cpp
CXXOBJ = $(CXXFILES:.cpp=.o)
program = app

all: ${program}

$(program): $(CXXOBJ)
	$(CXX) $(CXXFLAGS) $(CXXOBJ) -o $(program)

%.o: %.cpp Bureaucrat.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(CXXOBJ)

fclean: clean
	rm -f $(program) *.txt

re: fclean all

test:
	${CXX} FireWallManager.cpp tests.cpp -o test

.PHONY: clean fclean re all
