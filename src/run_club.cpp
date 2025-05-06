#include <iostream>
#include <ComputerClub.hpp>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: program_name file_name" << std::endl;
    }
    std::cout << "hello" << std::endl;
    return 0;
}
