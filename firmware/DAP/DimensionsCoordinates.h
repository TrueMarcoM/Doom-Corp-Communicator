//Coordinates, dimensions, margins, etc
#define SCREEN_WIDTH 240  //for now going to hardcode the width because of the tft.init() being invovled somewhere else, etc.
#define SCREEN_HEIGHT 135
#define SCREEN_X 0
#define SCREEN_Y 0

#define BORDER_WIDTH 4
#define SPACER_WIDTH 4


//Display coords/dimensions (the actual area where the content is located)
#define DISPLAY_WIDTH (SCREEN_WIDTH - BORDER_WIDTH - BORDER_WIDTH)
#define DISPLAY_HEIGHT (SCREEN_HEIGHT - BORDER_WIDTH - BORDER_WIDTH)
#define DISPLAY_X (SCREEN_X + BORDER_WIDTH)
#define DISPLAY_Y (SCREEN_Y + BORDER_WIDTH)


#define CENTER_OF_SCREEN_X 120  //240 / 2
#define CENTER_OF_SCREEN_Y 67   //135 / 2



//Menu Bar
//Menu elements width and position declarations
#define MENU_BAR_WIDTH DISPLAY_WIDTH
#define MENU_BAR_HEIGHT (8 + BORDER_WIDTH + BORDER_WIDTH)  //A bar that contains text. Text characters generally have a height of 8. Bar must be taller.
#define MENU_BAR_X DISPLAY_X
#define MENU_BAR_Y DISPLAY_Y


#define MENU_BAR_BORDER_WIDTH 4
#define MENU_BAR_INTERIOR_WIDTH (MENU_BAR_WIDTH - MENU_BAR_BORDER_WIDTH - MENU_BAR_BORDER_WIDTH)
#define MENU_BAR_INTERIOR_HEIGHT (MENU_BAR_HEIGHT - MENU_BAR_BORDER_WIDTH - MENU_BAR_BORDER_WIDTH)
#define MENU_BAR_INTERIOR_X (MENU_BAR_X + MENU_BAR_BORDER_WIDTH)
#define MENU_BAR_INTERIOR_Y (MENU_BAR_Y + MENU_BAR_BORDER_WIDTH)

#define RADIUS 10
#define STRAIGHT_START_X (MENU_BAR_X + RADIUS)
#define STRAIGHT_END_X (MENU_BAR_X + MENU_BAR_WIDTH - RADIUS)
#define STRAIGHT_WIDTH (MENU_BAR_WIDTH - 2 * RADIUS)

#define TIME_WIDTH (tft.textWidth("12:00 PM"))
#define TIME_HEIGHT 8
#define TIME_X (MENU_BAR_INTERIOR_X + 4)
#define TIME_Y MENU_BAR_INTERIOR_Y

#define ACTIVE_APP_WIDTH (tft.textWidth("Remote"))
#define ACTIVE_APP_HEIGHT 8
#define ACTIVE_APP_X (TIME_X + TIME_WIDTH + SPACER_WIDTH)
#define ACTIVE_APP_Y DISPLAY_Y

#define BATTERY_BAR_WIDTH 32                              //30 pixels wide interior
#define BATTERY_BAR_HEIGHT 7                              //5 pixels high interior
#define BATTERY_BAR_X (DISPLAY_WIDTH - BATTERY_BAR_WIDTH - 4)  //width of the screen - the width of the bar - the space from the right border
// #define BATTERY_BAR_X (STRAIGHT_END_X - BATTERY_BAR_WIDTH)
#define BATTERY_BAR_Y (MENU_BAR_Y + MENU_BAR_BORDER_WIDTH)  //width of the screen - the width of the bar - the space from the right border

/*
     The batteryBarX + x should equal straightEndX */

#define BATTERY_BAR_INTERIOR_WIDTH 30  //full
#define BATTERY_BAR_INTERIOR_HEIGHT 5  //full
#define BATTERY_BAR_INTERIOR_X (BATTERY_BAR_X + 1)
#define BATTERY_BAR_INTERIOR_Y (BATTERY_BAR_Y + 1)



// #define PERCENT_VALUE_WIDTH (tft.textWidth("100")) //18, 6 pixels wide for each character
#define BATTERY_PERCENT_WIDTH (tft.textWidth("100%"))
#define BATTERY_PERCENT_HEIGHT 8
#define BATTERY_PERCENT_X (BATTERY_BAR_X - SPACER_WIDTH - BATTERY_PERCENT_WIDTH)
#define BATTERY_PERCENT_Y (MENU_BAR_Y + MENU_BAR_BORDER_WIDTH)