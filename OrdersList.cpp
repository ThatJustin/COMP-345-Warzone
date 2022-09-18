//
// Created by Alex De Luca on 2022-09-14.
//

#include "OrdersList.h"
#include <vector>
OrdersList::OrdersList(const OrdersList& ol){
    list = ol.list;
    list_size = ol.list_size;
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

void OrdersList::remove(){

}


