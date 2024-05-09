#pragma once

class mode {
private:
    
public:
    virtual void init() {}
    virtual void process() {}
    virtual bool mode_call_has_arguments() { return false; }
};