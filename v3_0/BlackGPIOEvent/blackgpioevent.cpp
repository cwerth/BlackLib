#include <poll.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "blackgpioevent.h"
#include "eventhandler.h"
#include "../BlackThread/BlackThread.h"


namespace BlackLib
{


    BlackGPIOEvent::BlackGPIOEvent(gpioName pin, direction dir, workingMode wm) : BlackCoreGPIO(pin, dir)
    {
        this->pinName       = pin;
        this->pinDirection  = dir;
        this->workMode      = wm;
        this->edge          = NONE;
        this->gpioErrors    = new errorGPIO( this->getErrorsFromCoreGPIO() );
        this->callback      = NULL;
        this->valuePath     = this->getValueFilePath();
        this->edgePath      = this->getEdgeFilePath();
    }

    BlackGPIOEvent::BlackGPIOEvent(gpioName pin, direction dir, edgeType ed, void (*function)(), workingMode wm) : BlackCoreGPIO(pin, dir)
    {
        this->pinName       = pin;
        this->pinDirection  = dir;
        this->workMode      = wm;
        this->edge          = ed;
        this->gpioErrors    = new errorGPIO( this->getErrorsFromCoreGPIO() );
        this->callback      = function;
        this->valuePath     = this->getValueFilePath();
        this->edgePath      = this->getEdgeFilePath();
        this->setupInterrupt(ed, function);
    }

    BlackGPIOEvent::~BlackGPIOEvent()
    {
        delete this->gpioErrors;
    }


    bool        BlackGPIOEvent::isReady()
    {
        return (this->isExported() and this->isDirectionSet());
    }

    bool        BlackGPIOEvent::isExported()
    {
        std::ifstream exportCheck;

        exportCheck.open(valuePath.c_str(),std::ios::in|std::ios::binary);
        if(exportCheck.fail())
        {
            this->gpioErrors->exportError = true;
            return false;
        }
        else
        {
            this->gpioErrors->exportError = false;
            exportCheck.close();
            return true;
        }
    }

    bool        BlackGPIOEvent::isDirectionSet()
    {
        std::ifstream directionCheck;

        directionCheck.open((this->getDirectionFilePath()).c_str(), std::ios::in|std::ios::binary);
        if(directionCheck.fail())
        {
            directionCheck.close();
            this->gpioErrors->directionError = true;
            return false;
        }
        else
        {
            std::string readValue;
            directionCheck >> readValue;

            if( (this->pinDirection == input and readValue=="in") or (this->pinDirection == output and readValue=="out") )
            {
                directionCheck.close();
                this->gpioErrors->directionError = false;
                return true;
            }
            else
            {
                directionCheck.close();
                this->gpioErrors->directionError = true;
                return false;
            }
        }
    }


    std::string BlackGPIOEvent::getValue()
    {
        if( this->workMode == SecureMode )
        {
            if( ! this->isReady())
            {
                return GPIO_PIN_NOT_READY_STRING;
            }
        }


        std::ifstream valueFile;

        valueFile.open(valuePath.c_str(),std::ios::in);
        if(valueFile.fail())
        {
            valueFile.close();
            this->gpioErrors->readError = true;
            return FILE_COULD_NOT_OPEN_STRING;
        }
        else
        {
            std::string readValue;
            valueFile >> readValue;

            valueFile.close();
            this->gpioErrors->readError = false;
            return readValue;
        }
    }

    std::string BlackGPIOEvent::getEdgeFilePath()
    {
        return ("/sys/class/gpio/gpio" + tostr(pinName) + "/edge");
    }

    int         BlackGPIOEvent::getNumericValue()
    {
        if( this->workMode == SecureMode )
        {
            if( ! this->isReady())
            {
                return GPIO_PIN_NOT_READY_INT;
            }
        }


        std::ifstream valueFile;

        valueFile.open(valuePath.c_str(),std::ios::in);
        if(valueFile.fail())
        {
            valueFile.close();
            this->gpioErrors->readError = true;
            return FILE_COULD_NOT_OPEN_INT;
        }
        else
        {
            int readValue;
            valueFile >> readValue;

            valueFile.close();
            this->gpioErrors->readError = false;
            return readValue;
        }
    }

    gpioName    BlackGPIOEvent::getName()
    {
        return this->pinName;
    }

    direction   BlackGPIOEvent::getDirection()
    {
        return this->pinDirection;
    }

    bool        BlackGPIOEvent::setValue(digitalValue status)
    {
        if( !(this->pinDirection == output) )
        {
            this->gpioErrors->writeError = true;
            this->gpioErrors->forcingError = true;
            return false;
        }



        this->gpioErrors->forcingError = false;

        if( this->workMode == SecureMode )
        {
            if( ! this->isReady())
            {
                this->gpioErrors->writeError = true;
                return false;
            }
        }



        std::ofstream valueFile;
        valueFile.open(this->valuePath.c_str(), std::ios::out);
        if(valueFile.fail())
        {
            valueFile.close();
            this->gpioErrors->writeError = true;
            return false;
        }
        else
        {
            if( status == high )
            {
                valueFile << "1";
            }
            else
            {
                valueFile << "0";
            }

            valueFile.close();
            this->gpioErrors->writeError = false;
            return true;
        }
    }



    bool        BlackGPIOEvent::isHigh()
    {
        return (this->getNumericValue() == 1);
    }

    void        BlackGPIOEvent::toggleValue()
    {
        if( !(this->pinDirection == output) )
        {
            this->gpioErrors->forcingError = true;
        }
        else
        {
            this->gpioErrors->forcingError = false;
            if( (this->getNumericValue() == 1) )
            {
                this->setValue(low);
            }
            else
            {
                this->setValue(high);
            }
        }
    }


    void        BlackGPIOEvent::setWorkingMode(workingMode newWM)
    {
        this->workMode = newWM;
    }

    workingMode BlackGPIOEvent::getWorkingMode()
    {
        return this->workMode;
    }



    bool        BlackGPIOEvent::fail()
    {
        return (this->gpioErrors->readError or
                this->gpioErrors->writeError or
                this->gpioErrors->exportError or
                this->gpioErrors->forcingError or
                this->gpioErrors->directionError
                );
    }

    bool        BlackGPIOEvent::fail(BlackGPIOEvent::flags f)
    {
        if(f==readErr)          { return this->gpioErrors->readError;                           }
        if(f==writeErr)         { return this->gpioErrors->writeError;                          }
        if(f==exportErr)        { return this->gpioErrors->exportError;                         }
        if(f==forcingErr)       { return this->gpioErrors->forcingError;                        }
        if(f==directionErr)     { return this->gpioErrors->directionError;                      }
        if(f==exportFileErr)    { return this->gpioErrors->gpioCoreErrors->exportFileError;     }
        if(f==directionFileErr) { return this->gpioErrors->gpioCoreErrors->directionFileError;  }

        return true;
    }




    BlackGPIOEvent&  BlackGPIOEvent::operator>>(std::string &readToThis)
    {
        if( this->workMode == SecureMode )
        {
            if( ! this->isReady())
            {
                readToThis = GPIO_PIN_NOT_READY_STRING;
                return *this;
            }
        }


        std::string readValue;
        std::ifstream valueFile;

        valueFile.open(valuePath.c_str(),std::ios::in);
        if(valueFile.fail())
        {
            valueFile.close();
            this->gpioErrors->readError = true;
            readValue = FILE_COULD_NOT_OPEN_STRING;
        }
        else
        {
            valueFile >> readValue;

            valueFile.close();
            this->gpioErrors->readError = false;
        }

        readToThis = readValue;
        return *this;
    }


    BlackGPIOEvent&  BlackGPIOEvent::operator>>(int &readToThis)
    {
        if( this->workMode == SecureMode )
        {
            if( ! this->isReady())
            {
                readToThis = GPIO_PIN_NOT_READY_INT;
                return *this;
            }
        }


        int readValue;
        std::ifstream valueFile;

        valueFile.open(valuePath.c_str(),std::ios::in);
        if(valueFile.fail())
        {
            valueFile.close();
            this->gpioErrors->readError = true;
            readValue = FILE_COULD_NOT_OPEN_INT;
        }
        else
        {
            valueFile >> readValue;

            valueFile.close();
            this->gpioErrors->readError = false;
        }

        readToThis = readValue;
        return *this;
    }


    BlackGPIOEvent&  BlackGPIOEvent::operator<<(digitalValue value)
    {
        if( !(this->pinDirection == output) )
        {
            this->gpioErrors->writeError = true;
            this->gpioErrors->forcingError = true;
            return *this;
        }



        this->gpioErrors->forcingError = false;

        if( this->workMode == SecureMode )
        {
            if( ! this->isReady())
            {
                this->gpioErrors->writeError = true;
                return *this;
            }
        }



        std::ofstream valueFile;
        valueFile.open(this->valuePath.c_str(), std::ios::out);
        if(valueFile.fail())
        {
            valueFile.close();
            this->gpioErrors->writeError = true;
            return *this;
        }
        else
        {
            if( value == high )
            {
                valueFile << "1";
            }
            else
            {
                valueFile << "0";
            }

            valueFile.close();
            this->gpioErrors->writeError = false;
            return *this;
        }
    }

    bool BlackGPIOEvent::setupInterrupt(edgeType ed, void (*function)(void))
    {
                this->edge = ed;
                bool didSetEdge = this->setEdge();

                if ((this->fd = open(this->valuePath.c_str(), O_RDWR)) < 0)
                    return false;

                int count;
                char c;
                ioctl (this->fd, FIONREAD, &count);
                for (int i=0; i<count; i++)
                {
                    read(this->fd, &c, 1);
                }

                //this->handlers[this->pinName].handler = function;
                this->callback = function;

                this->interruptThread = new EventHandler(this);
                this->interruptThread->setPriority(BlackThread::PriorityHIGHEST);
                this->interruptThread->run();
    }

    int BlackGPIOEvent::waitForInterrupt()
    {
        pollfd myPoll;
        myPoll.fd = this->fd;
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
                this->callback();
        }

        return 0;
    }

    bool BlackGPIOEvent::setEdge()
    {
//        char* name = this->getName().c_str();
//        char* edgeExportPath = "/sys/class/gpio/gpio" + name + "/edge";
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

        int fd = open(this->edgePath.c_str(), O_WRONLY);
        if (fd < 0)
            return false;
        write(fd, edgeString, strlen(edgeString) + 1);
        close(fd);
        return true;
    }


}
