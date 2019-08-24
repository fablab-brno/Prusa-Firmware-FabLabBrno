#include <string.h>
extern const char* lcd_display_message_fullscreen_P();
extern void lcd_set_custom_characters_progress();
extern void lcd_set_custom_characters_degree();
extern void lcd_show_fullscreen_message_and_wait_P(const char *msg);
extern char *strchr_pointer;
extern int Fabman_mode;
extern bool stoppedInfo;

void lcd_FM_login();
void serial_FM_login();
void serial_FM_logoff();
void lcd_FM_offline_screen();
char FM_filename;

#include "cardreader.h"
char *starpos = NULL;
char *username = NULL;
bool fileExist = 0;

/*
   Fabman mode:

   0)Menu mode
   1)Login screen
   2)Offline screen
   3)User not alowed
*/

void lcd_FM_login_screen() {
  SERIAL_PROTOCOLLN("Stage 3 = reached lcd_FM_login_screen()");
  lcd_update_enable(false);

  lcd_set_custom_characters_progress();
  lcd_puts_P(PSTR(ESC_2J ESC_H(2, 1) "Prusa i3 Fabman" ESC_H(3, 2) "Swipe to login"));
  while (Fabman_mode) {
    delay_keep_alive(100);
    proc_commands();
  }
  lcd_set_custom_characters_degree();
  lcd_update_enable(true);
  lcd_update(2);
}

void lcd_FM_login() {
  SERIAL_PROTOCOLLN("Stage 1 = reached lcd_FM_login()");
  while (Fabman_mode == 1) {
    SERIAL_ECHO("Fabman mode = ");
    SERIAL_ECHOLN(Fabman_mode);
    SERIAL_PROTOCOLLN("Stage 2 = Calling lcd_FM_login_screen()");
    lcd_FM_login_screen();

  }
}

void lcd_FM_offline() {
  SERIAL_PROTOCOLLN("Stage 1 = reached lcd_FM_offline_screen()");
  while (Fabman_mode == 2) {
    SERIAL_ECHO("Fabman mode = ");
    SERIAL_ECHOLN(Fabman_mode);
    SERIAL_PROTOCOLLN("Stage 2 = Calling lcd_FM_offline_screen()");
    lcd_FM_offline_screen();
  }
}


void FM_print_username() {
  starpos = (strchr(strchr_pointer + 5, '*'));
  if (starpos != NULL)
    *(starpos) = '\0';
  username = (strchr_pointer + 5);
  SERIAL_ECHO("UNM: ");
  SERIAL_ECHOLN(username);
}

void filament_used_in_last_print() {
  SERIAL_ECHOLN("stoppedInfo is: " + (stoppedInfo));
  if (stoppedInfo == 1) {
    SERIAL_ECHOLN("stopState:false");
    stoppedInfo = 0;
  } else {
    SERIAL_ECHOLN("stopState:true");
  }
  SERIAL_ECHO("FNM:");
  SERIAL_ECHOLN(card.longFilename);
  SERIAL_ECHO("TFU: ");
  SERIAL_ECHOLN(total_filament_used);
}

#define MAX_FILE_SIZE 1024
#define JSON_BUFFER_SIZE 200

void getConfigFromJSON() {
  card.ls();
  if (fileExist != 0) {
    SERIAL_PROTOCOLLN("File found, trying to read it");
    // Converted name of "config.json" file in root of SD card
    card.openFile("CONFI~1.JSO", true);
    // get first character
    uint8_t c = card.get();
    // print whole file
    while (!card.eof()) {
      // filter newlines to have message in one line
      if (c != '\n') {
        SERIAL_ECHO(c);
      }
      c = card.get();
    }
    // end of message
    SERIAL_ECHOLN("");
  } else {
    SERIAL_PROTOCOLLN("config file doesn't exist, keep going");
  }
}

void lcd_FM_not_allowed() {
  SERIAL_PROTOCOLLN("Stage 1 = reached lcd_FM_not_allowed()");
  while (Fabman_mode == 3) {
    SERIAL_ECHO("Fabman mode = ");
    SERIAL_ECHOLN(Fabman_mode);
    SERIAL_PROTOCOLLN("Stage 2 = Calling lcd_FM_not_allowed_screen()");
  }
}


void serial_FM_login() {
  SERIAL_PROTOCOLLN("Serial FM login");
  Fabman_mode = 0;
  SERIAL_ECHO("Fabman mode = ");
  SERIAL_ECHOLN(Fabman_mode);
}

void serial_FM_logoff() {
  SERIAL_PROTOCOLLN("Serial FM logoff");
  Fabman_mode = 1;
  SERIAL_ECHO("Fabman mode = ");
  SERIAL_ECHOLN(Fabman_mode);
}

void serial_FM_offline() {
  SERIAL_PROTOCOLLN("Serial FM offline");
  Fabman_mode = 2;
  SERIAL_ECHO("Fabman mode = ");
  SERIAL_ECHOLN(Fabman_mode);
}

void serial_FM_not_allowed() {
  SERIAL_PROTOCOLLN("Serial FM user not allowed");
  Fabman_mode = 3;
  SERIAL_ECHO("Fabman mode = ");
  SERIAL_ECHOLN(Fabman_mode);
}

void check_FM_login() {
  /*SERIAL_ECHO("Fabman mode = ");
    SERIAL_ECHOLN(Fabman_mode);*/
  if (Fabman_mode == 1) {
    lcd_FM_login();
  }
  /*else if (Fabman_mode == 2) {
    lcd_FM_offline();
    }*/
  else if (Fabman_mode == 3) {
    lcd_FM_not_allowed();
  }
}
