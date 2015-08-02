#ifndef BlackGPIOEventEVENT_H
#define BlackGPIOEventEVENT_H

#include "../BlackGPIO/BlackGPIO.h"
#include "../BlackThread/BlackThread.h"
#include "eventhandler.h"

namespace BlackLib
{
    class EventHandler;
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
    class BlackGPIOEvent : virtual private BlackCoreGPIO
    {
    private:
        errorGPIO       *gpioErrors;                    /*!< @brief is used to hold the errors of BlackGPIOEvent class */
        gpioName        pinName;                        /*!< @brief is used to hold the selected GPIO pin name */
        direction       pinDirection;                   /*!< @brief is used to hold the selected GPIO pin direction */
        workingMode     workMode;                       /*!< @brief is used to hold the selected working mode */
        edgeType        edge;                           /*!< @brief is used to hold the pins edge */
        void            (*callback)();                  /*!< @brief is used to hold the callback function when triggered */
        EventHandler    *interruptThread;               /*!< @brief is used to handle interrupts */
        int             fd;                             /*!< @brief is used to hold the edge file location */
        std::string     valuePath;                      /*!< @brief is used to hold the value file path */
        std::string     edgePath;                       /*!< @brief is used to hold the edge file path */


        /*! @brief Checks the export state of GPIO pin.
        *
        * This function reads specified file from path, where defined at BlackGPIOEvent::valuePath variable.
        * If this file could open successfully, this means pin is exported successfully.
        * @return False if file opening fails, else true.
        */
        bool            isExported();

        /*! @brief Checks direction of GPIO pin.
        *
        * This function reads specified file from path, where defined at BlackGPIOEvent::directionPath variable.
        * @return True if direction file can open successfully and its value is equal to defined direction when
        * class initialization, else false.
        */
        bool            isDirectionSet();

        /*! @brief Checks ready state of GPIO pin.
        *
        * This function calls isExported() and isDirectionSet() functions and then evaluates return
        * values of these functions.
        * @return True if both functions return true, else false.
        * @sa isExported()
        * @sa isDirectionSet()
        */
        bool            isReady();

        std::string     getEdgeFilePath();

        bool            setEdge();

        int             waitForInterrupt();


    public:

        /*!
        * This enum is used to define GPIO debugging flags.
        */
        enum flags      {   exportFileErr       = 0,    /*!< enumeration for @a errorCoreGPIO::exportFileError status */
                            exportErr           = 1,    /*!< enumeration for @a errorGPIO::exportError status */
                            directionFileErr    = 2,    /*!< enumeration for @a errorCoreGPIO::directionFileError status */
                            directionErr        = 3,    /*!< enumeration for @a errorGPIO::directionError status */
                            readErr             = 4,    /*!< enumeration for @a errorGPIO::readError status */
                            writeErr            = 5,    /*!< enumeration for @a errorGPIO::writeError status */
                            forcingErr          = 6,    /*!< enumeration for @a errorGPIO::forcingError status */
                        };

        /*! @brief Constructor of BlackGPIOEvent class.
        *
        * This function initializes BlackCoreGPIO class with entered parameters and errorGPIO struct.
        * Then it sets value file path variable.
        * @param [in] pn        gpio pin name(enum)
        * @param [in] pd        gpio pin direction(enum)
        * @param [in] wm        working mode(enum), default value is SecureMode
        *
        * @par Example
        *  @code{.cpp}
        *   // Pin:30 - Direction:Out - Working Mode:SecureMode
        *   BlackLib::BlackGPIOEvent  myGpio(BlackLib::GPIO_30, BlackLib::output);
        *
        *   // Pin:60 - Direction:Out - Working Mode:FastMode
        *   BlackLib::BlackGPIOEvent  myGpio2(BlackLib::GPIO_60, BlackLib::output, BlackLib::FastMode);
        *
        *   // Pin:40 - Direction:In - Working Mode:SecureMode
        *   BlackLib::BlackGPIOEvent *myGpioPtr = new BlackLib::BlackGPIOEvent(BlackLib::GPIO_40, BlackLib::input);
        *
        *   myGpio.getValue();
        *   myGpio2.getValue();
        *   myGpioPtr->getValue();
        *
        * @endcode
        *
        * @sa gpioName
        * @sa direction
        * @sa workingMode
        */
                        BlackGPIOEvent(gpioName pn, direction pd, workingMode wm = SecureMode);
                        BlackGPIOEvent(gpioName pn, direction pd, edgeType ed, void (*funcion)(), workingMode wm = SecureMode);

        /*! @brief Destructor of BlackGPIOEvent class.
        *
        * This function deletes errorGPIO struct pointer.
        */
        virtual         ~BlackGPIOEvent();

        /*! @brief Reads value of gpio pin as string type.
        *
        * If working mode is selected SecureMode, this function checks pin ready state by calling isReady() function.
        * If pin is not ready, function returns with BlackLib::GPIO_PIN_NOT_READY_STRING value. If working mode is
        * selected FastMode, ready state checking will skip. Then it reads specified file from path, where defined at
        * BlackGPIOEvent::valuePath variable. This file holds gpio pin value.
        * @return @a string type GPIO pin value. If file opening fails, it returns BlackLib::FILE_COULD_NOT_OPEN_STRING
        * or if pin isn't ready, it returns BlackLib::GPIO_PIN_NOT_READY_STRING.
        *
        * @par Example
        * @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   std::string val = myGpio.getValue();
        *   std::cout << "Gpio30 Value: " << val;
        * @endcode
        * @code{.cpp}
        *   // Possible Output:
        *   // Gpio30 Value: 1
        * @endcode
        */
        std::string     getValue();

        /*! @brief Reads value of gpio pin as int type.
        *
        * If working mode is selected SecureMode, this function checks pin ready state by calling isReady() function.
        * If pin is not ready, function returns with BlackLib::GPIO_PIN_NOT_READY_INT value. If working mode is
        * selected FastMode, ready state checking will skip. Then it reads specified file from path, where defined at
        * BlackGPIOEvent::valuePath variable. This file holds gpio pin value.
        * @return @a int type GPIO pin value. If file opening fails, it returns BlackLib::FILE_COULD_NOT_OPEN_INT
        * or if pin isn't ready, it returns BlackLib::GPIO_PIN_NOT_READY_INT.
        *
        * @par Example
        * @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   int val = myGpio.getNumericValue();
        *   std::cout << "Gpio30 Value: " << val;
        * @endcode
        * @code{.cpp}
        *   // Possible Output:
        *   // Gpio30 Value: 1
        * @endcode
        */
        int             getNumericValue();

        /*! @brief Exports name of gpio pin.
        *
        *  @return BlackGPIOEvent::pinName variable.
        *
        *  @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   std::cout << "My gpio name: GPIO_" << static_cast<int>(myGpio.getName()) << std::endl;
        *   std::cout << "My gpio name: GPIO_" << (int)myGpio.getName() << std::endl;
        *   std::cout << "My gpio name: GPIO_" << myGpio.getName() << std::endl;
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // My gpio name: GPIO_30
        *   // My gpio name: GPIO_30
        *   // My gpio name: GPIO_30
        *  @endcode
        */
        gpioName        getName();

        /*! @brief Exports direction of gpio pin.
        *
        *  @return BlackGPIOEvent::pinDirection variable.
        *
        *  @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   std::cout << "Values: input=1, output=2, bothDirection=3" << std::endl << std::endl;
        *
        *   std::cout << "My gpio direction: " << static_cast<int>(myGpio.getDirection()) << std::endl;
        *   std::cout << "My gpio direction: " << (int)myGpio.getDirection() << std::endl;
        *   std::cout << "My gpio direction: " << myGpio.getDirection() << std::endl;
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // Values: input=1, output=2, bothDirection=3
        *   //
        *   // My gpio direction: 2
        *   // My gpio direction: 2
        *   // My gpio direction: 2
        *  @endcode
        */
        direction       getDirection();

        /*! @brief Sets value of GPIO pin.
        *
        * If pin direction is not output, function returns with false value. If working mode is selected SecureMode,
        * this function checks pin ready state by calling isReady() function. If pin is not ready, function returns
        * with false value. If working mode is selected FastMode, ready state checking will skip. Then the input
        * parameter is converted to 1 or 0 and this value is saved to value file.
        * @param [in] v new pin value(enum)
        * @return True if setting new value is successful, else false.
        *
        * @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *   BlackLib::BlackGPIOEvent myGpio2(BlackLib::GPIO_60, BlackLib::input, BlackLib::SecureMode);
        *
        *   if( myGpio.setValue(BlackLib::high) )
        *   {
        *       std::cout << "Gpio 30 set high successfully." << std::endl;
        *   }
        *   else
        *   {
        *       std::cout << "Gpio 30 couldn't set." << std::endl;
        *   }
        *
        *   if( myGpio2.setValue(BlackLib::low) )
        *   {
        *       std::cout << "Gpio 60 set low successfully (This is not possible)." << std::endl;
        *   }
        *   else
        *   {
        *       std::cout << std::endl << "Gpio 60 couldn't set. Because this pin's direction is input."
        *                 << std::endl << "You can't write anything to input type pin." << std::endl;
        *   }
        *
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // Gpio 30 set high successfully.
        *   //
        *   // Gpio 60 couldn't set. Because this pin's direction is input.
        *   // You can't write anything to input type pin.
        *  @endcode
        *
        * @sa digitalValue
        */
        bool            setValue(digitalValue v);

        /*! @brief Checks value of GPIO pin.
        *
        * This function calls getNumericValue() function and evaluates return value.
        * @return True if return value equals 1, else false.
        *
        * @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   myGpio.setValue(BlackLib::high);
        *   std::cout << std::boolalpha << myGpio.isHigh() << std::endl;
        *
        *   if( myGpio.isHigh() )
        *   {
        *       std::cout << "Do something with me, i am high." << std::endl;
        *   }
        *
        *   myGpio.setValue(BlackLib::low);
        *   std::cout << ( myGpio.isHigh() ? "yeap" : "nope") << std::endl;
        *
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // true
        *   // Do something with me, i am high.
        *   // nope
        *  @endcode
        *
        * @sa getNumericValue()
        */
        bool            isHigh();

        /*! @brief Toggles value of GPIO pin.
        *
        * If pin direction is output, this function sets pin value to 1 or 0, by value of current state.
        *
        * @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   myGpio.setValue(BlackLib::high);
        *   std::cout << "Before toggle: " << std::boolalpha << myGpio.isHigh() << std::endl;
        *
        *   myGpio.toggleValue();
        *   std::cout << "After toggle: " << std::boolalpha << myGpio.isHigh() << std::endl;
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // Before toggle: true
        *   // After toggle: false
        *  @endcode
        */
        void            toggleValue();

        /*! @brief Changes working mode.
        *
        * This function sets new working mode value to BlackGPIOEvent::workingMode variable.
        *
        * @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   myGpio.setWorkingMode(BlackLib::FastMode);
        *   std::cout << "Now you can be more faster than the previous one, but MAYBE." << std::endl;
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // Now you can be more faster than the previous one, but MAYBE.
        *  @endcode
        */
        void            setWorkingMode(workingMode newWM);

        /*! @brief Exports working mode value.
        *
        *  @return BlackLib::workingMode variable.
        *
        *  @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   std::cout << "Your current working mode is "
        *             << ( (myGpio.getWorkingMode() == BlackLib::SecureMode) ? "Secure Mode" : "Fast Mode" ) << std::endl;
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // Your current working mode is Secure Mode
        *  @endcode
        */
        workingMode     getWorkingMode();


        /*! @brief Is used for general debugging.
        *
        * @return True if any error occured, else false.
        *
        * @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   myGpio.getNumericValue();
        *
        *   if( myGpio.fail() )
        *   {
        *       std::cout << "ERROR OCCURED" << std::endl;
        *   }
        *   else
        *   {
        *       std::cout << "EVERYTHING IS OK" << std::endl;
        *   }
        * @endcode
        * @code{.cpp}
        *   // Possible Output:
        *   // EVERYTHING IS OK
        * @endcode
        *
        * @sa errorGPIO
        */
        bool            fail();

        /*! @brief Is used for specific debugging.
        *
        * You can use this function, after call BlackGPIOEvent member functions in your code. The
        * input parameter is used for finding out status of selected error.
        * @param [in] f specific error type (enum)
        * @return Value of @a selected error.
        *
        * @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   if( myGpio.fail(BlackLib::BlackGPIOEvent::directionFileErr) or
        *       myGpio.fail(BlackLib::BlackGPIOEvent::exportFileErr) )
        *   {
        *        std::cout << "GPIO30: BlackGPIOEvent INITIALIZATION FAILED" << std::endl;
        *   }
        *
        *   myGpio.setValue(BlackLib::high);
        *   if( myGpio.fail(BlackLib::BlackGPIOEvent::directionErr) or
        *       myGpio.fail(BlackLib::BlackGPIOEvent::exportErr) )
        *   {
        *        std::cout << "GPIO30: You are in secure mode and isReady() function failed." << std::endl;
        *   }
        *
        *   if( myGpio.fail(BlackLib::BlackGPIOEvent::forcingErr) )
        *   {
        *        std::cout << "GPIO30: You are tried to set some value to input type pin."
        *                  << " This operation is not valid." << std::endl;
        *   }
        *
        *   if( myGpio.fail(BlackLib::BlackGPIOEvent::writeErr) )
        *   {
        *        std::cout << "GPIO30: You could not write anything." << std::endl;
        *   }
        *
        *
        *
        *   BlackLib::BlackGPIOEvent myGpio2(BlackLib::GPIO_60, BlackLib::input, BlackLib::FastMode);
        *
        *   if( myGpio2.fail(BlackLib::BlackGPIOEvent::directionFileErr) or
        *       myGpio2.fail(BlackLib::BlackGPIOEvent::exportFileErr) )
        *   {
        *        std::cout << "GPIO60: BlackGPIOEvent INITIALIZATION FAILED" << std::endl;
        *   }
        *
        *   myGpio2.setValue(BlackLib::high);
        *   if( myGpio2.fail(BlackLib::BlackGPIOEvent::directionErr) or
        *       myGpio2.fail(BlackLib::BlackGPIOEvent::exportErr) )
        *   {
        *        std::cout << "GPIO60: This is not meaningful. Because you are in fast mode and these flags" << std::endl
        *                  << "could not change in fast mode." << std::endl;
        *   }
        *
        *   if( myGpio2.fail(BlackLib::BlackGPIOEvent::forcingErr) )
        *   {
        *       std::cout << "GPIO60: You are tried to set some value to input type pin."
        *                 << " This operation is not valid." << std::endl;
        *   }
        *
        *   if( myGpio2.fail(BlackLib::BlackGPIOEvent::writeErr) )
        *   {
        *        std::cout << "GPIO60: You could not write anything." << std::endl;
        *   }
        *
        *   myGpio2.getValue();
        *   if( ! myGpio2.fail(BlackLib::BlackGPIOEvent::readErr) )
        *   {
        *        std::cout << "GPIO60: You are read some value from GPIO60 successfully." << std::endl;
        *   }
        *
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // GPIO60: You are tried to set some value to input type pin. This operation is not valid.
        *   // GPIO60: You could not write anything.
        *   // GPIO60: You are read some value from GPIO60 successfully.
        *  @endcode
        *
        * @sa errorGPIO
        */
        bool            fail(BlackGPIOEvent::flags f);

        /*! @brief Reads value of gpio pin as string type with ">>" operator.
        *
        * If working mode is selected SecureMode, this function checks pin ready state by calling isReady() function.
        * If working mode is selected FastMode, ready state checking will skip. Then it reads specified file from path,
        * where defined at BlackGPIOEvent::valuePath variable, with ">>" operator. This file holds gpio pin value.
        * @param [in] &readToThis read value and save this value to this variable. If file opening fails, this
        * functions sets BlackLib::FILE_COULD_NOT_OPEN_STRING to variable or if pin isn't ready, it sets
        * BlackLib::GPIO_PIN_NOT_READY_STRING.
        *
        * @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   myGpio << BlackLib::high;
        *
        *   std::string val;
        *   myGpio >> val;
        *
        *   std::cout << "Current value: " << val << std::endl;
        *
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // Current value: 1
        *  @endcode
        */
        BlackGPIOEvent&      operator>>(std::string &readToThis);

        /*! @brief Reads value of gpio pin as int type with ">>" operator.
        *
        * If working mode is selected SecureMode, this function checks pin ready state by calling isReady() function.
        * If working mode is selected FastMode, ready state checking will skip. Then it reads specified file from path,
        * where defined at BlackGPIOEvent::valuePath variable, with ">>" operator. This file holds gpio pin value.
        * @param [in] &readToThis read value and save this value to this variable. If file opening fails, this
        * functions sets BlackLib::FILE_COULD_NOT_OPEN_INT to variable or if pin isn't ready, it sets
        * BlackLib::GPIO_PIN_NOT_READY_INT.
        *
        * @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   myGpio << BlackLib::high;
        *
        *   int val;
        *   myGpio >> val;
        *
        *   std::cout << "Current value: " << val << std::endl;
        *
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // Current value: 1
        *  @endcode
        */
        BlackGPIOEvent&      operator>>(int &readToThis);

        /*! @brief Sets value of GPIO pin with "<<" operator.
        *
        *  If pin direction is not output, function does nothing. If working mode is selected SecureMode, this function
        *  checks pin ready state by calling isReady() function. If pin is not ready, function does nothing. If working
        *  mode is selected FastMode, ready state checking will skip. Then the input parameter is saved to value file.
        *  @param [in] &value new pin value(enum)
        *
        *  @par Example
        *  @code{.cpp}
        *   BlackLib::BlackGPIOEvent myGpio(BlackLib::GPIO_30, BlackLib::output, BlackLib::SecureMode);
        *
        *   myGpio << BlackLib::high ;
        *   std::cout << std::boolalpha << myGpio.isHigh() << std::endl;
        *
        *   myGpio << BlackLib::low ;
        *   std::cout << std::boolalpha << myGpio.isHigh() << std::endl;
        *
        *  @endcode
        *  @code{.cpp}
        *   // Possible Output:
        *   // true
        *   // false
        *  @endcode
        *
        *  @sa digitalValue
        */
        BlackGPIOEvent&      operator<<(digitalValue value);

        bool setupInterrupt(edgeType edge, void (*function)(void));

        int             interruptHandler();

    };
}

#endif // BlackGPIOEventEVENT_H
