#include "Codec.h"

int main(int argc, char *const argv[]) {
    return Codec::run(Codec::Action::ENCODE, argc, argv);
}