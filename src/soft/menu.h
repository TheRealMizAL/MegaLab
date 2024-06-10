#pragma once

#include <stdint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"

#include "../shared/utils.h"

#ifndef MAX_LINES
#define MAX_LINES 4
#endif


class MenuNode {
public:
    MenuNode* parent = nullptr;
    MenuNode** childs = nullptr;
    uint8_t childs_size = 0;
    uint8_t max_childs = 0;

    char content[20] = "";

protected:
    MenuNode();
public:
    MenuNode(char* content, uint8_t max_childs);
    void addChild(MenuNode *child_item);

    virtual char* getTypeName() {
        return "MenuNode";
    }

    virtual char* getContent() {
        return this->content;
    }

};

class BackNode: public MenuNode {
public:
    BackNode();

    char* getTypeName() override {
        return "BackNode";
    }
private:
    void addChild(MenuNode* child_item) = delete;
    void onClick() = delete;
};

class IntValueNode: public MenuNode {
public:
    int8_t* val;
private:
    char buffer[20] = "";
public:
    IntValueNode(int8_t* val, char* content);

    char* getTypeName() override {
        return "IntValueNode";
    }

    char* getContent() override {
        // strcpy(buffer, this->content);
        // itoa(*this->val, buffer + 15, DEC);
        sprintf(buffer, "%s          %d", this->content, *this->val);
        return buffer;
    }
};

class FloatValueNode: public MenuNode {
public:
    float* val;
private:
    char buffer[20] = "";
    char fl_buf[5] = "";
public:
    FloatValueNode(float* val, char* content);

    char* getTypeName() override {
        return "IntValueNode";
    }

    char* getContent() override {
        dtostrf(*this->val, 2, 1, fl_buf);
        sprintf(buffer, "%s          %s", this->content,fl_buf);
        return buffer;
    }
};

class ActionNode: public MenuNode {
private:
    void (*action)() = nullptr;
public:
    ActionNode(char* content,void (*action)());
    char* getTypeName()override{
        return "ActionNode";
    }
    void (*Action())();
};


class Menu {
public:
    uint32_t last_update;
private:
    MenuNode* root;
    MenuNode* current_node;
    static LiquidCrystal_I2C* screen;
    uint8_t _cursor_position = 0;
    uint8_t _selected_child = 0;
    uint8_t _start_child = 0;
public:
    Menu(MenuNode* root);
    void enterNode();

    void drawCursor();
    static void addScreen(LiquidCrystal_I2C* screen);
    void cursorUp();
    void cursorDown();
    void drawChilds();
};


