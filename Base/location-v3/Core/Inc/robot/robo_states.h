#ifndef ROBO_STATES_H
#define ROBO_STATES_H

class Robo_State
{
public:
    Robo_State();
    Robo_State(Robo_State &&) = default;
    Robo_State(const Robo_State &) = default;
    Robo_State &operator=(Robo_State &&) = default;
    Robo_State &operator=(const Robo_State &) = default;
    ~Robo_State();

    // bool is_

private:
    
};

class Forest1: public Robo_State
{
public:
    Forest1();
    Forest1(Forest1 &&) = default;
    Forest1(const Forest1 &) = default;
    Forest1 &operator=(Forest1 &&) = default;
    Forest1 &operator=(const Forest1 &) = default;
    ~Forest1();

private:
    
};

Forest1::Forest1()
{
}

Forest1::~Forest1()
{
}

#endif //ROBO_STATES_H
