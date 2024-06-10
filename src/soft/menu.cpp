#include "menu.h"
#include <string.h>



MenuNode::MenuNode() {

}

MenuNode::MenuNode(char *content, uint8_t max_childs) {
    this->childs = new MenuNode*[max_childs];
    this->max_childs = max_childs;

    strncpy(this->content, content, 20);
}

void MenuNode::addChild(MenuNode *child_item) {
    if (this->childs_size < this->max_childs) {
        child_item->parent = this;
        this->childs[this->childs_size++] = child_item;
    }
}


BackNode::BackNode() : MenuNode() {
    strncpy(this->content, "Back", 20);
    this->max_childs = 0;
}

IntValueNode::IntValueNode(int8_t* val, char* content) : MenuNode() {
    strncpy(this->content, content, 20);
    this->val = val;
    this->max_childs = 0;
}

FloatValueNode::FloatValueNode(float* val, char* content) : MenuNode() {
    strncpy(this->content, content, 20);
    this->val = val;
    this->max_childs = 0;
}

LiquidCrystal_I2C* Menu::screen = nullptr;


Menu::Menu(MenuNode* root) {
    this->root = root;
    this->current_node = root;
}

void Menu::drawChilds() {
    Menu::screen->clear();
    if (this->current_node->getTypeName() != "IntValueNode") {
        Menu::drawCursor();
        for (uint8_t i = this->_start_child; i < min(this->_start_child + MAX_LINES, this->current_node->max_childs); i++)
        {
            Menu::screen->setCursor(1, i - this->_start_child);
            Menu::screen->print(this->current_node->childs[i]->getContent());
        }
    } else {
        Menu::screen->setCursor(1, 2);
        Menu::screen->print(this->current_node->getContent());
    }
}

static void Menu::addScreen(LiquidCrystal_I2C* screen){
    Menu::screen = screen;
}

void Menu::drawCursor()
{
    Menu::screen->setCursor(0, this->_cursor_position);
    Menu::screen->write(126);
}

void Menu::cursorUp() {
    if (this->current_node->getTypeName() != "IntValueNode") {
        Menu::screen->setCursor(0, this->_cursor_position);
        Menu::screen->write(32);

        if (this->_cursor_position < min(MAX_LINES, this->current_node->max_childs) - 1) this->_cursor_position++;
        else {
            if (this->_start_child < this->current_node->max_childs - MAX_LINES) this->_start_child++;
            this->drawChilds();
        }
        this->_selected_child = min(this->current_node->max_childs - 1, this->_selected_child + 1);
        Menu::drawCursor();
    } else {
        IntValueNode* current = this->current_node;
        (*current->val)++;
        this->drawChilds();
    }
    this->last_update = millis();
}

void Menu::cursorDown() {
    if (this->current_node->getTypeName() != "IntValueNode") {
        Menu::screen->setCursor(0, this->_cursor_position);
        Menu::screen->write(32);

        if (this->_cursor_position > 0) this->_cursor_position--;
        else {
            if (this->_start_child > 0) this->_start_child--;
            this->drawChilds();
        }
        this->_selected_child = max(0, this->_selected_child - 1);
        Menu::drawCursor();
    } else {
        IntValueNode* current = this->current_node;
        (*current->val)--;
        this->drawChilds();
    }
    this->last_update = millis();
}

void Menu::enterNode() {
    if (this->current_node->getTypeName() != "IntValueNode") {
        char* node_type = this->current_node->childs[this->_selected_child]->getTypeName();
        Serial.println(node_type);
        if(node_type == "BackNode")
            this->current_node = this->current_node->parent;
        else if(node_type == "ActionNode"){
            ActionNode* node = this->current_node->childs[this->_selected_child];
            node->Action()();
            this->current_node = this->root;
        }else if(node_type == "IntValueNode") {
            this->current_node = this->current_node->childs[this->_selected_child];
        }
        else if(this->current_node->childs[this->_selected_child]->max_childs > 0)
            this->current_node = this->current_node->childs[this->_selected_child];
    }
    else {
        this->current_node = this->current_node->parent;
    }
    this->_cursor_position = 0;
    this->_selected_child = 0;
    this->_start_child = 0;
    this->drawChilds();
    this->last_update = millis();
}

ActionNode::ActionNode(char *content, void (*action)()) : MenuNode(content,0)
{
    this->action = action;
}

void (*ActionNode::Action())()
{
    return this->action;
}
