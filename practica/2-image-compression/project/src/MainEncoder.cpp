
#include "Init.h"

int main(int argc, char *const argv[]) {
    Init init(argc, argv);
    if (!init.isInitialized()) {
        return 1;
    }
    //use <init.getConfig()> for the configuration
}
