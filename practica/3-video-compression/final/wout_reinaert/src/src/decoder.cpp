#include "Codec.h"

/**
 * Main function for decoder executable, calls the codec decode function
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *const argv[]) {
    return Codec::run(Codec::Action::DECODE, argc, argv);
}