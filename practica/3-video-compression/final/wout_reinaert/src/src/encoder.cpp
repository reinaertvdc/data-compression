#include "Codec.h"

/**
 * Main function for encoder executable, calls the codec encode function
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *const argv[]) {
    return Codec::run(Codec::Action::ENCODE, argc, argv);
}