// Copyright 2025 Cameron Gagne

#include <iostream>
#include "RandWriter.hpp"

int main(int argc, char const *argv[]) {
    int k = std::stoi(argv[1]);
    int l = std::stoi(argv[2]);
    std::string input((std::istreambuf_iterator<char>(std::cin)),
    std::istreambuf_iterator<char>());

    RandWriter rw(input, k);
    std::cout << rw << std::endl;
    std::cout << rw.generate(input.substr(0, k), l) << std::endl;

    return 0;
}
