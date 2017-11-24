#ifndef MINI_RPG_EVENTPACKAGE_H
#define MINI_RPG_EVENTPACKAGE_H


enum Type {MOVE_PLAYER, END_GAME, ATTACK, RESET_STATE, CHANGE_CHAR, SET_CHAR};

struct EventPackage {
    EventPackage() {};
    EventPackage(Type t) : type(t){};
    EventPackage(Type t, int a, int b) : type(t) {
        switch (t) {
            case MOVE_PLAYER:
                x = a;
                y = b;
                break;
            case SET_CHAR:
                row = a;
                col = b;
                break;
            default: break;
        }

    };

    Type type;
    int x;
    int y;
    int row;
    int col;
};

#endif //MINI_RPG_EVENTPACKAGE_H
