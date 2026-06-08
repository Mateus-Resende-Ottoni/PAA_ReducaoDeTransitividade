CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
TARGET = paa_transitividade

SRC = \
	Codigo/Grafos.cpp \
	Codigo/Bibliotecas/Edge/Edge.cpp \
	Codigo/Bibliotecas/Vertex/Vertex.cpp \
	Codigo/Bibliotecas/GraphInList/GraphInList.cpp \
	Codigo/Bibliotecas/GraphInMatrix/GraphInMatrix.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	find . -name '*.o' -delete

.PHONY: all run clean
