
#include <iostream>
#include "Init.h"

int main(int argc, char *const argv[]) {
    std::cout << "DECODER" << std::endl << std::endl;
    Init init(argc, argv);
    if (!init.isInitialized()) {
        return 1;
    }
}
