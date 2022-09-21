//
// Created by Alex De Luca on 2022-09-14.
//
#include "Orders.h"
#include <vector>
#include <string>
#include <sstream>

bool Orders::validate() {

}

void Orders::execute() {

}

OrdersList::OrdersList() {
    this->list = std::vector<Orders*>();
}

OrdersList::OrdersList(const OrdersList& ol){
    list = ol.list;
}

void OrdersList::move(int to_move, int move_to) {
    Orders* temp;
    if(to_move<move_to){
        temp = list[move_to];
        list[move_to]=list[to_move];
        list[to_move]=temp;
        list.push_back(temp);
        list.erase(list.begin()+to_move);
        list.push_back(list[move_to]);
        list.erase(list.begin()+move_to);
        temp = list[move_to];
        list[move_to]=list[move_to-1];
        list[move_to-1]=temp;
    }
    if(to_move>move_to){
        temp = list[to_move];
        list[to_move]=list[move_to];
        list[move_to]=temp;
        list.push_back(temp);
        list.erase(list.begin()+move_to);
        list.push_back(list[to_move]);
        list.erase(list.begin()+to_move);
        temp = list[to_move];
        list[to_move]=list[to_move-1];
        list[to_move-1]=temp;
    }
}

void OrdersList::remove(int order){
    list.erase(list.begin()+order);
}

void OrdersList::add(Orders* o){
    list.push_back(o);
}

OrdersList::~OrdersList() {
    for(Orders *ol: list){
        delete ol;
        ol = nullptr;
    }
}

std::string Deploy::toString() const {
    std::stringstream ss;
    ss<<"move a certain number of army units from the current player’s reinforcement pool to one of the current player’s territories";
    return ss.str();
}

std::string Advance::toString() const {
    std::stringstream ss;
    ss<<"move a certain number of army units from one of the current player’s territories (source) to another\n"
        "territory (target) that is adjacent to the source territory. If the target territory belongs to the current\n"
        "player, the armies are moved from the source territory to the target territory. If the target territory\n"
        "belongs to another player, an attack happens between the two territories. An attack is simulated by\n"
        "the following battle simulation mechanism: First, the attacking player decides how many armies in\n"
        "the source territory are involved in the attack. Then, each attacking army unit involved has 60%\n"
        "chances of destroying one defending army. At the same time, each defending army unit has 70%\n"
        "chances of destroying one attacking army unit. If all the army units on the target territory are\n"
        "destroyed as a result of the battle, the remaining army units left from the attacking army units from\n"
        "the source territory are moved to the target territory and the target territory now belongs to the player\n"
        "that declared the advance order, i.e. the player has conquered this territory. In any given turn, a\n"
        "player receives a card if they conquered at least one territory during this turn.";
    return ss.str();
}

std::string Bomb::toString() const {
    std::stringstream ss;
    ss<<"destroy half of the army units located on an opponent’s territory that is adjacent to one of the current\n"
        "player’s territories.";
    return ss.str();
}

std::string Blockade::toString() const {
    std::stringstream ss;
    ss<<"triple the number of army units on one of the current player’s territories and make it a neutral territory.";
    return ss.str();
}

std::string Airlift::toString() const {
    std::stringstream ss;
    ss<<"advance a certain number of army units from one of the current player’s territories to any another\n"
        "territory.";
    return ss.str();
}

std::string Negotiate::toString() const {
    std::stringstream ss;
    ss<<"prevent attacks between the current player and the player targeted by the negotiate order until the\n"
        "end of the turn.";
    return ss.str();
}