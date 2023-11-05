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
    lv_obj_add_event_cb(ui_Screen2, ui_Main_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Dimm, changeScreen, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(ui_Screen1, changeScreen, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(ui_Screen2, changeScreen, LV_EVENT_GESTURE, NULL);

    screenTypes[0] = ui_Dimm;
    screenTypes[1] = ui_Screen1;
    screenTypes[2] = ui_Screen2;
    screenInits[0] = &ui_Dimm_screen_init;
    screenInits[1] = &ui_Screen1_screen_init;
    screenInits[2] = &ui_Screen2_screen_init;
    screenLabels[0]= ui_LabelName;
    screenLabels[1]= ui_Label2;
    screenLabels[2]= ui_Label1;

    setTextForChannel(0);

    currentScreen = ui_Dimm;
    Serial.println("Started.");
}

void TouchDisplayModule::lv_log(const char * buf)
{
    Serial.println(buf);
}

void TouchDisplayModule::changeScreen(lv_event_t * event)
{
    ui_Main_event(NULL);
    Serial.print("Change screen ");
    if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        Serial.print("left ");
        lv_indev_wait_release(lv_indev_get_act());
        TouchDisplayModule::currentScreenIndex++;
    } else if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        Serial.print("right ");
        lv_indev_wait_release(lv_indev_get_act());
        TouchDisplayModule::currentScreenIndex--;
    } else {
        Serial.println();
        return;
    }

    if(TouchDisplayModule::currentScreenIndex >= VISU_MAX_PAGE)
        TouchDisplayModule::currentScreenIndex = 0;
    else if(TouchDisplayModule::currentScreenIndex < 0)
        TouchDisplayModule::currentScreenIndex = VISU_MAX_PAGE - 1;
    TouchDisplayModule::currentScreen = screenTypes[TouchDisplayModule::currentScreenIndex];

    Serial.println(TouchDisplayModule::currentScreenIndex);

    setTextForChannel(TouchDisplayModule::currentScreenIndex);

    _ui_screen_change( &currentScreen, LV_SCR_LOAD_ANIM_FADE_IN, 500, 0, screenInits[TouchDisplayModule::currentScreenIndex]);
}

void TouchDisplayModule::setTextForChannel(int channel)
{
    switch(channel)
    {
        case 0:
            lv_label_set_text(screenLabels[channel], "Wohnlicht");
            break;
        case 1:
            lv_label_set_text(screenLabels[channel], "Arbeitslicht");
            break;
        case 2:
            lv_label_set_text(screenLabels[channel], "Deckenlicht");
            break;
    }
}

void TouchDisplayModule::ui_Main_event(lv_event_t * event)
{
    Serial.println("Main pressed.");
    TouchDisplayModule::lastPressed = millis();
    if (!TouchDisplayModule::displayOn) {
        Serial.println("Turn display on.");
        TouchDisplayModule::displayOn = true;
        digitalWrite(XIAO_BL, HIGH);
    }
}

uint32_t last_lv_handler = 0;

void TouchDisplayModule::loop()
{
    if(millis() - last_lv_handler > 5)
    {
        lv_timer_handler();  //let the GUI do its work 
        last_lv_handler = millis();
    }

    if (TouchDisplayModule::displayOn && (millis() - TouchDisplayModule::lastPressed > DISPLAY_SLEEP_DELAY)) {
        Serial.println("Turn display off.");
        TouchDisplayModule::displayOn = false;
        digitalWrite(XIAO_BL, LOW);
    }
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