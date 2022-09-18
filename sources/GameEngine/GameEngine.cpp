

#include <cstdio>
#include "GameEngine.h"
#include <iostream>
#include <string>

GameEngine::GameEngine() {
    _state =  new Start(this);
}

void GameEngine::setState(GameState *state) {
    _state = state;
}

GameState* GameEngine::getState() {
    return _state;
}

void GameEngine::Handle() {
    _state->Handle();
}

///start state
Start::Start(GameEngine *context): _context(context) {
    Name = "Start";
};
void Start::Handle() {
    std::string input;
    std::cout << "Please type a command: valid commands are: loadmap\n";
    std::cin >> input;
    

    if(input == "loadmap"){
        _context->setState(new Map_Loaded(_context));
    }else{
        printf("wrong command, please retry \n");
    }


}

///Map Loaded state
Map_Loaded::Map_Loaded(GameEngine *context): _context(context) {
    Name = "Map Loaded";
};
void Map_Loaded::Handle() {
    std::string input;
    std::cout << "Please type a command: valid commands are: loadmap, validatemap\n";
    std::cin >> input;

    if(input == "loadmap"){
        _context->setState(new Map_Loaded(_context));
    }else if(input == "validatemap"){
        _context->setState(new Map_Validated(_context));
    }else{
        printf("wrong command, please retry \n");
    }
}

///Map validated state
Map_Validated::Map_Validated(GameEngine *context): _context(context) {
    Name = "Map Validated";
};
void Map_Validated::Handle() {
    std::string input;
    std::cout << "Please type a command: valid commands are: addplayer\n";
    std::cin >> input;
    

    if(input == "addplayer"){
        _context->setState(new Players_Added(_context));
    }else{
        printf("wrong command, please retry \n");
    }
}

///Players_Added state
Players_Added::Players_Added(GameEngine *context): _context(context) {
    Name = "Players_Added";
};
void Players_Added::Handle() {
    std::string input;
    std::cout << "Please type a command: valid commands are: addplayer,assigncountries\n";
    std::cin >> input;
    

    if(input == "addplayer"){
        _context->setState(new Players_Added(_context));
    }else if(input == "assigncountries"){
        _context->setState(new Assign_Reinforcement(_context));
    }else{
        printf("wrong command, please retry \n");
    }
}


///Assign_Reinforcement state
Assign_Reinforcement::Assign_Reinforcement(GameEngine *context): _context(context) {
    Name = "Assign_Reinforcement";
};
void Assign_Reinforcement::Handle() {
    std::string input;
    std::cout << "Please type a command: valid commands are: issueorder\n";
    std::cin >> input;
    

    if(input == "issueorder"){
        _context->setState(new Issue_Orders(_context));
    }else{
        printf("wrong command, please retry \n");
    }
}

///Issue_Orders state
Issue_Orders::Issue_Orders(GameEngine *context): _context(context) {
    Name = "Issue_Orders";
};
void Issue_Orders::Handle() {
    std::string input;
    std::cout << "Please type a command: valid commands are: issueorder,endissueorders\n";
    std::cin >> input;
    

    if(input == "issueorder"){
        _context->setState(new Issue_Orders(_context));
    }else if(input == "endissueorders"){
        _context->setState(new Execute_orders(_context));
    }else{
        printf("wrong command, please retry \n");
    }
}

///Execute_ordersstate
Execute_orders::Execute_orders(GameEngine *context): _context(context) {
    Name = "Execute_orders";
};
void Execute_orders::Handle() {
    std::string input;
    std::cout << "Please type a command: valid commands are: execorder,endexecorders,win\n";
    std::cin >> input;
    

    if(input == "execorder"){
        _context->setState(new Execute_orders(_context));
    }else if(input == "endexecorders"){
        _context->setState(new Assign_Reinforcement(_context));
    }else if(input == "win"){
        _context->setState(new Win(_context));
    }else{
        printf("wrong command, please retry \n");
    }
}

///Win
Win::Win(GameEngine *context): _context(context) {
    Name = "Win";
};
void Win::Handle() {
    std::string input;
    std::cout << "Please type a command: valid commands are: end, play\n";
    std::cin >> input;
    

    if(input == "end"){
        _context->setState(new End(_context));
    }else if(input == "play"){
        _context->setState(new Start(_context));
    }else{
        printf("wrong command, please retry \n");
    }
}

///End
End::End(GameEngine *context): _context(context) {
    Name = "End";
};
void End::Handle() {
    std::cout << "Thank you for playing the game";
}
