#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include "../BlackThread/BlackThread.h"


namespace BlackLib
{

    class EventHandler : public BlackThread
    {
    private:
        void (*handler)();
        void onStartHandler();


    public:
        EventHandler(void (*function)());

    };

}
#endif // EVENTHANDLER_H
