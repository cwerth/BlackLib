#ifndef BLACKGPIOEVENT_H
#define BLACKGPIOEVENT_H

#include "../BlackCore.h"
#include "../BlackGPIO/BlackGPIO.h"
#include "eventhandler.h"

namespace BlackLib
{
//#define NUM_PINS 125

    /*!
    * This enum is used for setting a pin's edge.
    */
    enum edgeType
    {
        NONE,
        EDGE_RISING,
        EDGE_FALLING,
        BOTH
    };

    struct interruptHandlerStruct
    {
        int fd;
        void (*handler)(void);
    };

    class BlackGPIOEvent : private BlackGPIO
    {
    private:
        //static interruptHandlerStruct handlers[NUM_PINS];         /*!< @brief is used to hold the interrupt handlers for the pins */

 //       EventHandler* interruptThread;                        /*!< @brief is used to wait for the interrupt to be generated (non-blocking) */

//        int fd;

//        edgeType edge;

        /*! @brief sets the edge for a pin
        *  This method sets the pin's edge type to either none, rising, falling, or both
        *  This determines on which edge causes an interrupt to be generated.
        *  @return True if the edge is successfully set, else false.
        */
//        bool            setEdge();

        /*!  @brief waits for the pin interrupt
        *  This method sets the thread to wait for the pin's interrupt to occur
        *  @return a positive value if success, a negative value on failure                     ******************verify*********************8
        */
//        int             waitForInterrupt();

//        int             interruptHandler();

//        void            (*handler)(void);

    public:
        BlackGPIOEvent(gpioName pn, direction pd, workingMode wm = SecureMode);
//        BlackGPIOEvent(gpioName pn, direction pd, edgeType ed, void (*function)(void), workingMode wm = SecureMode);
//        bool setupInterrupt(edgeType edge, void (*function)(void));
    virtual ~BlackGPIOEvent();

    };
}

#endif // BLACKGPIOEVENT_H
