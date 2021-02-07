CC=g++
STD=c++11
CFLAGS=-Wall

SRC=BasicTimer.cpp
SRC+=Core.cpp
SRC+=DataGenerator.cpp
SRC+=SumFinder.cpp
SRC+=BankAccount.cpp

HDR=$(SRC:.cpp=.hpp)

# its headerfile not yet implemented
MAIN_SRC=OperationUsingThreads.cpp

main: $(SRC) $(HDR) $(MAIN_SRC)
	$(CC) -std=$(STD) $(CFLAGS) $(SRC) $(MAIN_SRC) -o main

clean:
	-@rm main 2> /dev/null || true;
