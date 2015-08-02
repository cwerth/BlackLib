#include "eventhandler.h"

namespace BlackLib
{

    EventHandler::EventHandler(void (*function)())
    {
        this->handler = function;
    }

    void EventHandler::onStartHandler()
    {
        this->function();
    }
}
