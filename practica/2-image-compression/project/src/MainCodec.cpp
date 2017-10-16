
#include "Init.h"

int main(int argc, char *const argv[]) {
    Init init;
    if (!init.init(argc, argv)) {
        return 1;
    }
    //use <init.getConfig()> for the configuration
}
