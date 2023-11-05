#include <Arduino.h>
#include "OpenKNX.h"
#include "TouchDisplayModule.h"

void setup()
{
    const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, new TouchDisplayModule());
    openknx.setup();

    // call direct for testing without KNX connected
    openknx.modules.list[0]->setup();
}

void loop()
{
	openknx.loop();

    // call direct for testing without KNX connected
    openknx.modules.list[0]->loop();
}