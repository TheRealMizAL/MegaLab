#include "dispatcher.h"

Dispatcher::Dispatcher(const short tickable_size)
{
    tickers = new ITickable*[tickable_size];
    this->tickers_size = tickable_size;
}

void Dispatcher::add(Task *task)
{
    static Node* last = nullptr;
    if(!root){
        root = new Node(task,nullptr);
        last = root;
    }else{
        last->next = new Node(task,nullptr);
        last = last->next;
    }
}

void Dispatcher::add_ticker(ITickable *ticker)
{
    tickers[last] = ticker;
    last++;
}

void Dispatcher::tick()
{
    #ifndef TEMP
    for(unsigned short i = 0 ; i < tickers_size;++i){
        if(!tickers[i])
            continue;
        tickers[i]->tick();
    }
    #endif
    uint32_t current_time = millis();
    Node* it = root;
    while(it != nullptr){
        if(it->obj->is_enabled && current_time - it->obj->last_exec >= it->obj->getDelay()){
            it->obj->run();
            it->obj->last_exec = current_time;
        }
        it = it->next;
    }
}

void Dispatcher::enable_all() {
    Node* it = root;
    while(it != nullptr){
        it->obj->enable();
        it = it->next;
    }
}

Task::Task(short (*work)(), const char *name, uint8_t delay)
{
    this->work = work;
    this->name = name;
    this->delay = delay;
}

void Task::disable()
{
    this->is_enabled = false;
    this->is_running = false;
}

void Task::enable()
{
    this->is_enabled = true;
}

void Task::run()
{
    (is_enabled)? is_running = true: is_running = false;
    if(is_running)
        this->work();
    is_running = false;
}

bool Task::isEnabled() {
    return this->is_enabled;
}

const char * const Task::getName() const
{
    return name;
}

short (*Task::getFunc())()
{
    return work;
}

uint8_t Task::getDelay() const
{
    return this->delay;
}

MegaTask& MegaTask::getSignleObj(short (*work)(), const char *name, uint8_t delay)
{
    static MegaTask obj(work,name,delay);
    return obj;
}

MegaTask::MegaTask(short (*work)(), const char *name, uint8_t delay) : Task(work,name,delay)
{

}