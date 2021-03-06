#ifndef MINI_RPG_EVENTPACKAGE_H
#define MINI_RPG_EVENTPACKAGE_H

struct EventPackage {
    enum Type {
        MOVE_PLAYER, PLAYER_ATTACK, CHANGE_PLAYER, SELECT_PLAYER, VIEW_STATS,
        QUIT, RESET, EXIT_SPECIAL_SCREEN, QUICK_ACCESS
    };

    EventPackage() {};
    EventPackage(Type t) : type(t){};
    EventPackage(Type t, int q) : type(t){
        quickAccess = q;
    };
    EventPackage(Type t, int a, int b) : type(t) {
        switch (t) {
            case MOVE_PLAYER:
                x = a;
                y = b;
                break;
            case SELECT_PLAYER:
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
    int quickAccess;
};

#endif //MINI_RPG_EVENTPACKAGE_H
