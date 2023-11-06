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

    //ui_init();

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Dimm_screen_init();
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);

    lv_obj_add_event_cb(ui_Dimm, changeScreen, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(ui_Screen1, changeScreen, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(ui_Screen2, changeScreen, LV_EVENT_GESTURE, NULL);

    screenTypes[0] = ui_Screen2;
    screenTypes[1] = ui_Dimm;
    screenTypes[2] = ui_Screen1;
    screenTypes[3] = ui_Screen2;
    screenInits[0] = &ui_Screen2_screen_init;
    screenInits[1] = &ui_Dimm_screen_init;
    screenInits[2] = &ui_Screen1_screen_init;
    screenInits[3] = &ui_Screen2_screen_init;
    screenLabels[0]= ui_Label1;
    screenLabels[1]= ui_LabelName;
    screenLabels[2]= ui_Label2;
    screenLabels[3]= ui_Label1;

    setTextForChannel(0);

    bool flag = false;
    for(int i = 0; i < VISU_MAX_PAGE; i++)
    {
        if(checkPageActive(i))
        {
            flag = true;
            loadPage(i);
            currentScreen = screenTypes[ParamPAGE_typeIndex(i)-1];
            TouchDisplayModule::currentScreenIndex = i;
            break;
        }
    }

    if(!flag)
    {
        logErrorP("No start Page found");
        ui_Screen3_screen_init();
        lv_disp_load_scr(ui_Screen3);
        TouchDisplayModule::currentScreenIndex = 255;
    }
    else
    {
        screenInits[TouchDisplayModule::currentScreenIndex]();
        lv_disp_load_scr(currentScreen);
    }
}

void TouchDisplayModule::lv_log(const char * buf)
{
    logDebug("lvgl", buf);
}

void TouchDisplayModule::changeScreen(lv_event_t * event)
{
    TouchDisplayModule::lastPressed = millis();
    if (!TouchDisplayModule::displayOn) {
        logDebug("Display", "Turn display on.");
        TouchDisplayModule::displayOn = true;
        digitalWrite(XIAO_BL, HIGH);
    }

    int oldIndex = TouchDisplayModule::currentScreenIndex;
    if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        do {
            TouchDisplayModule::currentScreenIndex++;
            if(TouchDisplayModule::currentScreenIndex >= VISU_MAX_PAGE)
                TouchDisplayModule::currentScreenIndex = 0;
        } while(!checkPageActive(TouchDisplayModule::currentScreenIndex));
    } else if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        do {
            TouchDisplayModule::currentScreenIndex--;
            if(TouchDisplayModule::currentScreenIndex < 0)
                TouchDisplayModule::currentScreenIndex = VISU_MAX_PAGE - 1;
        } while(!checkPageActive(TouchDisplayModule::currentScreenIndex));
    } else {
        return;
    }

    if(oldIndex == TouchDisplayModule::currentScreenIndex)
        return;
    
    loadPage(TouchDisplayModule::currentScreenIndex);
}

void TouchDisplayModule::loadPage(int channel)
{
    logDebug("Display", "Loading Page %i", channel);
    TouchDisplayModule::currentScreen = screenTypes[ParamPAGE_typeIndex(TouchDisplayModule::currentScreenIndex)-1];
    setTextForChannel(TouchDisplayModule::currentScreenIndex);
    _ui_screen_change(&currentScreen, LV_SCR_LOAD_ANIM_FADE_IN, 500, 0, screenInits[TouchDisplayModule::currentScreenIndex]);
}

bool TouchDisplayModule::checkPageActive(int channel)
{
    return ParamPAGE_typeIndex(channel) != 0;
}

void TouchDisplayModule::setTextForChannel(int channel)
{
    uint8_t *display = ParamPAGE_displayIndex(channel);
    lv_label_set_text(screenLabels[channel], (char*)display);
}

uint32_t last_lv_handler = 0;

void TouchDisplayModule::loop()
{
    if(millis() - last_lv_handler > 5)
    {
        lv_timer_handler();  //let the GUI do its work 
        last_lv_handler = millis();
    }

    if (TouchDisplayModule::currentScreenIndex != 255 && TouchDisplayModule::displayOn && (millis() - TouchDisplayModule::lastPressed > DISPLAY_SLEEP_DELAY)) {
        logDebugP("Turn display off.");
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