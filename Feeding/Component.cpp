#include "Component.h"

bool Component::isRedrawRequired()
{
    return true;
}

void Component::print(LCD_1602_RUS *lcd)
{
}

componentType Component::getType() {
    return component;
}