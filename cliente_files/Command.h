//
// Created by Usuario on 09/10/2024.
//

#ifndef COMMAND_H
#define COMMAND_H

class Command {
public:
    virtual ~Command();
    virtual void execute() = 0;
};
#endif //COMMAND_H
