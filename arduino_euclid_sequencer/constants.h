/**
 * holds constants which are used across scripts
 * 
 * constants which signify pin assignments etc are defined within the script 
 * that cares and won't be accessed across scripts
 */
#define VERSION "0.1.1"

// related to the LCD display
#define DISPLAY_EDITMODE_MENU 'm'
#define DISPLAY_EDITMODE_GRID 'g'

#define DISPLAY_MENUPOS_CLOCK 2
#define DISPLAY_MENUPOS_BPM 4
#define DISPLAY_MENUPOS_SWING 7
#define DISPLAY_MENUPOS_PARAMETER_NAME 12
#define DISPLAY_MENUPOS_PARAMETER_VALUE 17

#define DISPLAY_SEQINDICATOR_KICK         'K'
#define DISPLAY_SEQINDICATOR_SNARE        'S'
#define DISPLAY_SEQINDICATOR_HIHAT_CLOSED 'h'
#define DISPLAY_SEQINDICATOR_HIHAT_OPEN   'H'
#define DISPLAY_SEQINDICATOR_RIMSHOT      'R'
#define DISPLAY_SEQINDICATOR_CLAVE        'C'
#define DISPLAY_SEQINDICATOR_TOM_HI       'T'
#define DISPLAY_SEQINDICATOR_TOM_LO       't'

#define SETTING_CLOCKSOURCE_CV       'C'
#define SETTING_CLOCKSOURCE_INTERNAL 'I'
