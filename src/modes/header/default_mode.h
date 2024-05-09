#pragma once

#include "../header/mode_base.h"

class default_mode : public mode {
private:
    bool internet_access;

public:
    void init();
    void process();

};