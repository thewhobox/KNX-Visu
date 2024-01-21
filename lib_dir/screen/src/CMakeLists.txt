SET(SOURCES screens/ui_Dimm.c
    screens/ui_Color.c
    screens/ui_Switch.c
    screens/ui_Message.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c)

add_library(ui ${SOURCES})
