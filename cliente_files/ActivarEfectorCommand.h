//
// Created by Usuario on 09/10/2024.
//

#ifndef ACTIVAREFECTORCOMMAND_H
#define ACTIVAREFECTORCOMMAND_H

#include "Command.h"

class ActivarEfectorCommand : public Command {
public:
    ActivarEfectorCommand(int efectorState);
    void execute() override;
};



#endif //ACTIVAREFECTORCOMMAND_H
