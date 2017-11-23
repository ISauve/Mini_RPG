#ifndef CS_247_COMPUTER_H
#define CS_247_COMPUTER_H

#include "Player.h"

class Computer: public Player {

public:
    Computer(std::vector< Card *>, std::vector< Card * >);
    Computer();
    virtual Card * discard() override;
    Card lowest();
};

#endif //CS_247_COMPUTER_H
