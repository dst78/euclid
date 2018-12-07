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


#define SETTING_PARAMETER_NAME_MCHN "Mchn" // MIDI channel for the instrument
#define SETTING_PARAMETER_NAME_SLEN "Slen" // sequence length
#define SETTING_PARAMETER_NAME_SPUL "Spul" // active sequence pulses - this is in the Euclidian Rythm sense
#define SETTING_PARAMETER_NAME_SROT "Srot" // sequence offset - this is in the Euclidian Rythm sense
#define SETTING_PARAMETER_NAME_SDIR "Sdir" // sequence play direction - forward, backward, alternating
#define SETTING_PARAMETER_NAME_SNLN "Snln" // sequence note length - 16th, 8th, 4th
#define SETTING_PARAMETER_NAME_SGTE "Sgte" // sequence gate length
#define SETTING_PARAMETER_NAME_SVEL "Svel" // sequence note velocity
#define SETTING_PARAMETER_NAME_SNTE "Snte" // sequence MIDI note
#define SETTING_PARAMETER_NAME_SRDA "Srda" // sequence randomization amount
#define SETTING_PARAMETER_NAME_SRDC "Srdc" // sequence randomization chance

#define SETTING_PARAMETER_VALUE_SDIR_MIN 0
#define SETTING_PARAMETER_VALUE_SDIR_MAX 2

#define SETTING_PARAMETER_VALUE_SNLN_SIXTEENTH 16
#define SETTING_PARAMETER_VALUE_SNLN_EIGHTHS    8
#define SETTING_PARAMETER_VALUE_SNLN_QUARTERS   4

#define SETTING_PARAMETER_VALUE_SGTE_MIN  1
#define SETTING_PARAMETER_VALUE_SGTE_MAX 99

#define SETTING_PARAMETER_VALUE_SVEL_MIN   1
#define SETTING_PARAMETER_VALUE_SVEL_MAX 127

#define SETTING_PARAMETER_VALUE_SNTE_MIN 24 // C1
#define SETTING_PARAMETER_VALUE_SNTE_MAX 96 // C7
