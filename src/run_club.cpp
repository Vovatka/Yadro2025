#include <iostream>
#include <ComputerClub.hpp>
#include <exception>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: program_name file_name" << std::endl;
        return 1;
    }
    try {
        ComputerClub club(argv[1]);
        club.StartLoop();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
