#include <string>
#include <cstdio>
#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include "blackgpioevent.h"
#include "../BlackGPIO/BlackGPIO.h"
#include "eventhandler.h"


namespace BlackLib
{

    BlackGPIOEvent::BlackGPIOEvent(gpioName pn, direction pd, workingMode wm) : BlackLib::BlackGPIO(pn, pd, wm)
    {
        pinName = pn;
        pinDirection = pd;
        workMode = wm;
//        fd = -1;
//        interruptThread = NULL;
    }
/*
    BlackGPIOEvent::BlackGPIOEvent(gpioName pn, direction pd, edgeType ed, void (*function)(void), workingMode wm)
    {
        this->pinName = pn;
        this->pinDirection = pd;
        this->workMode = wm;
        this->fd = -1;
        this->interruptThread = NULL;

        this->setEdge();
        this->setupInterrupt(ed, function);
    }

    bool BlackGPIOEvent::setEdge()
    {
        char* name = this->getName().c_str();
        char* edgeExportPath = "/sys/class/gpio/gpio" + name + "/edge";
        char* edgeString = NULL;

        switch (this->edge)
        {
        case NONE:
            edgeString = "none";
            break;
        case EDGE_RISING:
            edgeString = "rising";
            break;
        case EDGE_FALLING:
             edgeString = "falling";
            break;
        case BOTH:
            edgeString = "both";
            break;
        default:
            break;
        }

        int fd = open(edgeExportPath, O_WRONLY);
        if (fd < 0)
            return false;
        write(fd, edgeString, strlen(edgeString) + 1);
        close(fd);
        return true;
    }

    int BlackGPIOEvent::waitForInterrupt()
    {
        pollfd myPoll;
        myPoll.fd = this->handlers[this->pinName].fd;
        myPoll.events = POLLPRI;

        int x = poll (&myPoll, 1, -1);

        if (myPoll.revents & POLLPRI)
        {
            char c;
            int i = read(myPoll.fd, &c, 1);
            return x;
        }

        else
        {
            return 0;
        }
    }

    int BlackGPIOEvent::interruptHandler()
    {
        while (true)
        {
            if (this->waitForInterrupt())
                //this->handlers[this->pinName].handler();
                this->handler();
        }

        return 0;
    }

    bool BlackGPIOEvent::setupInterrupt(edgeType ed, void (*function)())
    {
        this->edge = ed;
        bool didSetEdge = this->setEdge();

        char fn[64];

        sprintf(fn, "/sys/class/gpio/gpio" + this->getName() + "/value");
        if ((this->fd = open(fn, O_RDWR)) < 0)
            return false;

        int count;
        char c;
        ioctl (this->fd, FIONREAD, &count);
        for (int i=0; i<count; i++)
        {
            read(this->fd, &c, 1);
        }

        //this->handlers[this->pinName].handler = function;
        this->handler = function;

        this->interruptThread = new EventHandler(&this);
        this->interruptThread->setPriority(PriorityHIGHEST);
        this->interruptThread->run();

    }
    */
}

