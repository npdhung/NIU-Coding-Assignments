rv32i: main.cpp rv32i_decode.cpp memory.cpp hex.cpp rv32i_decode.h memory.h hex.h 
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o main.o main.cpp 
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o rv32i_decode.o rv32i_decode.cpp
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o memory.o memory.cpp
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o hex.o hex.cpp
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -o rv32i main.o rv32i_decode.o memory.o hex.o
clean: 
	rm *.o *.exe