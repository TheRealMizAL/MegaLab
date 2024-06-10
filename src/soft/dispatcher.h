#include <stdint.h>
#include "../shared/ITickable.h"
#include "Arduino.h"
#pragma once



class ITask{
public:
    virtual void disable() = 0;
    virtual void enable() = 0;
    virtual void run() = 0;
    virtual const char* const getName()const = 0;
    virtual short (*getFunc())() = 0;
    virtual uint8_t getDelay()const = 0;
    virtual bool isEnabled() = 0;
};

class Task : ITask{
public:
    bool is_running = false;
    bool is_enabled = false;
    uint32_t last_exec = 0;
private:
    short (*work)() = nullptr;
    const char* name = nullptr;
    uint8_t delay = 0;
public:
    Task() {};
    Task(short (*work)(), const char* name,uint8_t delay);
    virtual void disable()override;
    virtual void enable()override;
    virtual void run()override;
    virtual const char* const getName()const override;
    virtual short (*getFunc())()override;
    virtual uint8_t getDelay()const override;
    virtual bool isEnabled() override;
};

class MegaTask: public Task
{
public:
    static MegaTask& getSignleObj(short (*work)() = nullptr, const char* name = nullptr,uint8_t delay = 0);
private:
    MegaTask() = delete;
    MegaTask(const MegaTask&) = delete;
    MegaTask& operator=(const MegaTask& task) = delete;
    MegaTask(short (*work)(), const char* name,uint8_t delay);
};

class Node{
public:
    Task* obj;
    Node* next = nullptr;

    Node(Task* new_obj, Node* next_node) {
        this->obj = new_obj;
        this->next = next_node;
    }
};

class IDispatcher{
public:
    virtual void add(Task* task) = 0;
    virtual void add_ticker(ITickable* ticker) = 0;
    virtual void tick() = 0;
    virtual void enable_all() = 0;
};

class Dispatcher: IDispatcher{
private:
    ITickable ** tickers = nullptr;
    unsigned short last = 0;
    unsigned short tickers_size = 0; 
    Node* root = nullptr;
public:
    Dispatcher() = delete;
    Dispatcher(const short tickable_size);
    virtual void add(Task* task)override;
    virtual void add_ticker(ITickable* ticker)override;
    virtual void tick()override;
    virtual void enable_all() override;
};