#include "OpenKNX.h"
#include <lvgl.h>

#define EXT1_PIN 28
#define EXT2_PIN 29

#define DISPLAY_SLEEP_DELAY 10000

class TouchDisplayModule : public OpenKNX::Module
{
	public:
		void loop() override;
		void setup() override;
		void processAfterStartupDelay() override;
		void processInputKo(GroupObject &ko) override;

		const std::string name() override;
		const std::string version() override;
		// void writeFlash() override;
		// void readFlash(const uint8_t* data, const uint16_t size) override;
		// uint16_t flashSize() override;
	
	private:
		enum DoorState {
			UNDEFINED,
			OPEN,
			CLOSED,
			AUTO
		};

		static void lv_log(const char * buf);
		static void changeScreen(lv_event_t * event);
		static void ui_Main_event(lv_event_t * event);
		static void display_pressed();

		inline static int screenTypes[] = {0,1,2};

		static lv_obj_t *currentScreen;
		inline static int currentScreenIndex = 0;

		inline static DoorState doorState;
		inline static bool displayOn;
		inline static unsigned long lastPressed;
};