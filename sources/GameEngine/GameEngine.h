
#ifndef COMP_345_WARZONE_GAMEENGINE_H
#define COMP_345_WARZONE_GAMEENGINE_H

#import "string"


class GameState {
public:
    std::string Name;
    virtual void Handle() = 0;
    virtual ~GameState();
};

class GameEngine {
private:
    GameState* _state;

public:
    GameEngine();
    ~GameEngine();
    void setState(GameState* state);
    GameState* getState();
    void Handle();
};

class Start : public GameState {
private:
    GameEngine* _context;
public:
    explicit Start(GameEngine* context);
    ~Start();
    void Handle();
};

class Map_Loaded : public GameState {
private:
    GameEngine* _context;
public:
    explicit Map_Loaded(GameEngine* context);
    ~Map_Loaded();
    void Handle();
};

class Map_Validated : public GameState {
private:
    GameEngine* _context;
public:
    explicit Map_Validated(GameEngine* context);
    ~Map_Validated();
    void Handle();
};

class Players_Added : public GameState {
private:
    GameEngine* _context;
public:
    explicit Players_Added(GameEngine* context);
    ~Players_Added();
    void Handle();
};


class Assign_Reinforcement : public GameState {
private:
    GameEngine* _context;
public:
    explicit Assign_Reinforcement(GameEngine* context);
    ~Assign_Reinforcement();
    void Handle();
};

class Issue_Orders : public GameState {
private:
    GameEngine* _context;
public:
    explicit Issue_Orders(GameEngine* context);
    ~Issue_Orders();
    void Handle();
};

class Execute_orders : public GameState {
private:
    GameEngine* _context;
public:
    explicit Execute_orders(GameEngine* context);
    ~Execute_orders();
    void Handle();
};

class Win : public GameState {
private:
    GameEngine* _context;
public:
    explicit Win(GameEngine* context);
    ~Win();
    void Handle();
};

class End : public GameState {
private:
    GameEngine* _context;
public:
    explicit End(GameEngine* context);
    ~End();
    void Handle();
};


#endif //COMP_345_WARZONE_GAMEENGINE_H
