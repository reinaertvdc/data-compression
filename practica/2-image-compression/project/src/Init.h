//
// Created by cwout on 16/10/17.
//

#ifndef PROJECT_INIT_H
#define PROJECT_INIT_H


#include "Config.h"

class Init {
private:
    Config conf;
public:
    bool init(int argc, char *const *argv);
    Config getConfig();
};


#endif //PROJECT_INIT_H
