#ifndef SWAG_SCANNER_ICONTROLLERGUI_H
#define SWAG_SCANNER_ICONTROLLERGUI_H

#include "IController.h"
#include <memory>
#include <string>
#include <iostream>


class SwagGUI;

class IFormsPayload;

Q_DECLARE_METATYPE(std::string)

namespace controller {
    /**
     * Represents a controller for GUIs.
     */
    class IControllerGUI : public IController {
    Q_OBJECT

    public:

        explicit IControllerGUI(std::shared_ptr<SwagGUI> gui);

        virtual ~IControllerGUI() {}


        /**
         * This method connects gui to this controller. Must run this method whenever
         * swapping controllers for GUI. Call this method in the manager class.
         *
         */
//        void setup_gui();

        /**
         * Update the controller with the given payload.
         *
         * @param payload payload from GUI.
         */
        virtual void update(const IFormsPayload &payload) = 0;


    signals:

        /**
         * Write message to the GUI console.
         *
         * @param info message you want to write.
         */
        void update_console(const std::string &info);


    protected:
        std::shared_ptr<SwagGUI> gui;

    };

}

#endif //SWAG_SCANNER_ICONTROLLERGUI_H
