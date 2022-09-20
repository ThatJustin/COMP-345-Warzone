//
// Created by Alex De Luca on 2022-09-14.
//

#ifndef COMP_345_ORDERS_H
#define COMP_345_ORDERS_H


class Orders {
public:
    Orders();
    bool validate();
    void execute();
};

class Deploy : public Orders{

};

class Advance : public Orders{

};

class Bomb : public Orders{

};

class Blockade : public Orders{

};

class Airlift : public Orders{

};

class Negotiate : public Orders{

};
#endif //COMP_345_ORDERS_H
