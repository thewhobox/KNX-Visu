#include <TouchDisplayModule.h>
#include <Arduino.h>
#include <ui.h>
#include "lv_xiao_round_screen.h"

const std::string TouchDisplayModule::name()
{
    return "Touch Display";
}

const std::string TouchDisplayModule::version()
{
    return "0.0dev";
}

void TouchDisplayModule::setup()
{
    TouchDisplayModule::doorState = DoorState::UNDEFINED;
    TouchDisplayModule::displayOn = true;
    TouchDisplayModule::lastPressed = 0;

    lv_log_register_print_cb(lv_log);

    lv_init();

    lv_xiao_disp_init();
    lv_xiao_touch_init();

    ui_init();

    lv_obj_add_event_cb(ui_Dimm, ui_Main_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Screen1, ui_Main_event, LV_EVENT_CLICKED, NULL);

    currentScreen = ui_Dimm;
    lv_obj_add_event_cb(currentScreen, changeScreen, LV_EVENT_GESTURE, NULL);

    Serial.println("Started.");
}

void TouchDisplayModule::lv_log(const char * buf)
{
    Serial.println(buf);
}

void TouchDisplayModule::changeScreen(lv_event_t * event)
{
    bool direct = false;
    if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        TouchDisplayModule::currentScreenIndex++;
    } else if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        TouchDisplayModule::currentScreenIndex--;
        direct = true;
    } else {
        return;
    }

    TouchDisplayModule::currentScreenIndex = TouchDisplayModule::currentScreenIndex % 3;
    switch(screenTypes[TouchDisplayModule::currentScreenIndex])
    {
        case 0:
            TouchDisplayModule::currentScreen = ui_Dimm;
            break;
        case 1:
            TouchDisplayModule::currentScreen = ui_Screen1;
            break;
        case 2:
            TouchDisplayModule::currentScreen = ui_Screen2;
            break;
    }
    

    _ui_screen_change( &(currentScreen), (direct ? LV_SCR_LOAD_ANIM_MOVE_LEFT : LV_SCR_LOAD_ANIM_MOVE_LEFT), 500, 0, &ui_Screen1_screen_init);
}

void TouchDisplayModule::ui_Main_event(lv_event_t * event)
{
    display_pressed();
    Serial.println("Main pressed.");
}

void TouchDisplayModule::loop()
{
    lv_timer_handler();  //let the GUI do its work 

    /*if (TouchDisplayModule::displayOn && (millis() - TouchDisplayModule::lastPressed > DISPLAY_SLEEP_DELAY)) {
        Serial.println("Turn display off.");
        TouchDisplayModule::displayOn = false;
        digitalWrite(XIAO_BL, LOW);
    }*/
}

void TouchDisplayModule::processInputKo(GroupObject& iKo)
{
}

void TouchDisplayModule::processAfterStartupDelay()
{
}

// void TouchDisplayModule::writeFlash()
// {
//     for (size_t i = 0; i < flashSize(); i++)
//     {
//         //openknx.flash.writeByte(0xd0 + i);
//     }
// }

// void TouchDisplayModule::readFlash(const uint8_t* data, const uint16_t size)
// {
//     // printHEX("RESTORE:", data,  len);
// }

// uint16_t TouchDisplayModule::flashSize()
// {
//     return 10;
// }