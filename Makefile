CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude
SRC_MODULOS = src/cadastros.cpp src/consultas.cpp src/ordenacao.cpp src/recursao.cpp src/simulacao.cpp

.PHONY: all run test clean

all: build/sistema

build/sistema: src/main.cpp $(SRC_MODULOS) include/*.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/main.cpp $(SRC_MODULOS) -o build/sistema

run: build/sistema
	./build/sistema

test: tests/testes_automatizados.cpp $(SRC_MODULOS) include/*.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) tests/testes_automatizados.cpp $(SRC_MODULOS) -o build/testes
	./build/testes

clean:
	rm -rf build/*
