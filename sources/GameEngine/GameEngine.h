
#ifndef COMP_345_WARZONE_GAMEENGINE_H
#define COMP_345_WARZONE_GAMEENGINE_H

#import "string"


class GameState{
    public: std::string Name;
    public: virtual void Handle() = 0;
};

class GameEngine {
    private: GameState* _state;

public:
    GameEngine();
    void setState(GameState* state);
    GameState* getState();
    void Handle();
};

class Start: public GameState{
    private:
        GameEngine* _context;
    public:
        Start(GameEngine* context);
        void Handle();
};

class Map_Loaded: public GameState{
    private:
        GameEngine* _context;
    public:
        Map_Loaded(GameEngine* context);
        void Handle();
};

class Map_Validated: public GameState{
private:
    GameEngine* _context;
public:
    Map_Validated(GameEngine* context);
    void Handle();
};

class Players_Added: public GameState{
private:
    GameEngine* _context;
public:
    Players_Added(GameEngine* context);
    void Handle();
};


class Assign_Reinforcement: public GameState{
private:
    GameEngine* _context;
public:
    Assign_Reinforcement(GameEngine* context);
    void Handle();
};

class Issue_Orders: public GameState{
private:
    GameEngine* _context;
public:
    Issue_Orders(GameEngine* context);
    void Handle();
};

class Execute_orders: public GameState{
private:
    GameEngine* _context;
public:
    Execute_orders(GameEngine* context);
    void Handle();
};

class Win: public GameState{
private:
    GameEngine* _context;
public:
    Win(GameEngine* context);
    void Handle();
};

class End: public GameState{
private:
    GameEngine* _context;
public:
    End(GameEngine* context);
    void Handle();
};



#endif //COMP_345_WARZONE_GAMEENGINE_H
