//
// Created by Usuario on 09/10/2024.
//

#ifndef MOVEEFECTORCOMMAND_H
#define MOVEEFECTORCOMMAND_H
#include "Command.h"
#include <vector>
class MoveEfectorCommand : public Command{
public:
    MoveEfectorCommand(std::vector<float> position, float speed);
    explicit MoveEfectorCommand(std::vector<float> position);
    void execute() override;
};



#endif //MOVEEFECTORCOMMAND_H
