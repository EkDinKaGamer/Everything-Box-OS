#ifndef EBOS_MENU_H
#define EBOS_MENU_H
#include <Arduino.h>
#include "../display/MAX7219_Display.h"
#include "../display/TM1637_Display.h"
#include "../input/Joystick.h"
#include "../core/AppManager.h"
#include "../System/Animations.h"
#include "../System/SoundManager.h"
#include "../core/Settings.h"
#include "esp8266/Communication.h"

// Hierarchical menu manager, non-blocking control via update().
class MenuManager {
  MAX7219_Display* disp;
  TM1637_Display* num;
  Joystick* joy;
  AppManager* apps;
  Animations* anim;
  SoundManager* sound;
  Settings* settings;

  enum MenuState { MS_MAIN, MS_GAMES, MS_TOOLS, MS_CREATIVE, MS_ANIM, MS_HW, MS_SETTINGS, MS_WIFI, MS_SUB } state;

  const char menu_main[9][10] = {"GAMES","TOOLS","PIXEL","ANIM","HARDWARE","SETTINGS","WIFI","ABOUT","EXIT"};
  uint8_t mainCount = 9;
  int8_t sel = 0;
  unsigned long lastInput = 0;

  // Settings substate
  int8_t set_sel = 0; // 0 brightness,1 RGB,2 buzzer,3 boot,4 reset,5 save
  int8_t rgb_step = 0; // 0 R 1 G 2 B

  // WiFi response buffer from ESP
  char wifiResp[64]; bool wifiRespReady = false;

public:
  void begin(MAX7219_Display* d, TM1637_Display* t, Joystick* j, AppManager* a, Animations* an, SoundManager* s, Settings* st) {
    disp=d; num=t; joy=j; apps=a; anim=an; sound=s; settings=st; state=MS_MAIN; sel=0; set_sel=0;
    wifiResp[0]=0; wifiRespReady=false;
  }

  void showMain() {
    disp->clear(); disp->drawCenteredText(menu_main[sel]); disp->show();
  }

  void update() {
    // check incoming wifi responses
    if (Communication::availableResponse()) {
      int n = Communication::readResponse(wifiResp, sizeof(wifiResp));
      wifiRespReady = n>0;
    }

    NavEvent e = joy->consumeEvent();
    if (e==NAV_NONE) return;
    lastInput = millis();
    switch(state) {
      case MS_MAIN:
        handleMainEvent(e); break;
      case MS_GAMES:
        handleGames(e); break;
      case MS_TOOLS:
        handleTools(e); break;
      case MS_CREATIVE:
        handleCreative(e); break;
      case MS_ANIM:
        handleAnims(e); break;
      case MS_HW:
        handleHardware(e); break;
      case MS_SETTINGS:
        handleSettings(e); break;
      case MS_WIFI:
        handleWiFi(e); break;
      default:
        state = MS_MAIN; break;
    }
  }

  void handleMainEvent(NavEvent e) {
    if (e==NAV_UP) { if (--sel < 0) sel = mainCount-1; sound->click(); showMain(); }
    else if (e==NAV_DOWN) { if (++sel >= mainCount) sel=0; sound->click(); showMain(); }
    else if (e==NAV_BTN_A || e==NAV_PRESS || e==NAV_RIGHT) {
      sound->select();
      // map selection
      const char* item = menu_main[sel];
      if (strcmp(item,"GAMES")==0) { state=MS_GAMES; sel=0; showGamesMenu(); }
      else if (strcmp(item,"TOOLS")==0) { state=MS_TOOLS; sel=0; showToolsMenu(); }
      else if (strcmp(item,"PIXEL")==0) { state=MS_CREATIVE; apps->launchCreative(0); }
      else if (strcmp(item,"ANIM")==0) { state=MS_ANIM; anim->showLibrary(disp); }
      else if (strcmp(item,"HARDWARE")==0) { state=MS_HW; showHardwareMenu(); }
      else if (strcmp(item,"SETTINGS")==0) { state=MS_SETTINGS; set_sel=0; showSettings(); }
      else if (strcmp(item,"WIFI")==0) { state=MS_WIFI; showWiFiMenu(); }
      else if (strcmp(item,"ABOUT")==0) { disp->scrollText_P(PSTR("Everything Box OS v1.0")); }
      else if (strcmp(item,"EXIT")==0) { disp->scrollText_P(PSTR("Bye")); }
    }
  }

  void showGamesMenu() {
    disp->scrollText_P(PSTR("GAMES: SNAKE PONG ..."));
  }
  void handleGames(NavEvent e) {
    if (e==NAV_BTN_A || e==NAV_PRESS) { apps->launchGame(0); } // snake default
    else if (e==NAV_BTN_B) { apps->launchGame(1); } // pong
    else if (e==NAV_BTN_C) { disp->scrollText_P(PSTR("FLAPPY")); }
    else if (e==NAV_BTN_D) { state=MS_MAIN; showMain(); }
  }

  void showToolsMenu() {
    disp->scrollText_P(PSTR("TOOLS: CALC STOPWATCH TIMER COIN DICE RNG"));
  }
  void handleTools(NavEvent e) {
    if (e==NAV_BTN_A) { apps->launchTool(0); } // calculator
    else if (e==NAV_BTN_B) { apps->stopwatch.start(); }
    else if (e==NAV_BTN_C) { apps->timerApp.start(10); }
    else if (e==NAV_BTN_D) { state=MS_MAIN; showMain(); }
  }

  void handleCreative(NavEvent e) {
    // Pixel draw handled inside app
    if (e==NAV_BTN_D) { state=MS_MAIN; showMain(); }
  }

  void handleAnims(NavEvent e) {
    if (e==NAV_BTN_A) anim->boot(disp);
    if (e==NAV_BTN_B) disp->drawBitmap(ICON_HEART);
    if (e==NAV_BTN_D) { state=MS_MAIN; showMain(); }
  }

  void showHardwareMenu() {
    disp->scrollText_P(PSTR("HARDWARE: RGB BUZZER SERVO"));
  }
  void handleHardware(NavEvent e) {
    if (e==NAV_BTN_A) { // RGB demo
      // set via settings rgb values
      // call external controller if available
      disp->scrollText_P(PSTR("RGB"));
    } else if (e==NAV_BTN_D) { state=MS_MAIN; showMain(); }
  }

  void showSettings() {
    // show current selection
    switch(set_sel) {
      case 0: { char b[8]; sprintf(b,"BR:%d", settings->brightness); disp->drawCenteredText(b); disp->show(); } break;
      case 1: { char r[12]; sprintf(r,"RGB:%d,%d,%d",settings->rgbR,settings->rgbG,settings->rgbB); disp->scrollText(r); } break;
      case 2: { disp->scrollText_P(settings->soundOn?PSTR("SOUND ON"):PSTR("SOUND OFF")); } break;
      case 3: { disp->scrollText_P(settings->bootAnimation?PSTR("BOOT ON"):PSTR("BOOT OFF")); } break;
      case 4: disp->scrollText_P(PSTR("RESET")); break;
      case 5: disp->scrollText_P(PSTR("SAVE")); break;
    }
  }
  void handleSettings(NavEvent e) {
    if (e==NAV_UP) { if (--set_sel < 0) set_sel = 5; showSettings(); }
    else if (e==NAV_DOWN) { if (++set_sel > 5) set_sel = 0; showSettings(); }
    else if (e==NAV_BTN_A) {
      // adjust or toggle
      switch(set_sel) {
        case 0: // brightness
          settings->brightness = (settings->brightness + 1) & 0x0F;
          disp->setBrightness(settings->brightness);
          showSettings(); break;
        case 1: // cycle rgb components
          if (rgb_step==0) { settings->rgbR = (settings->rgbR + 64) % 256; }
          else if (rgb_step==1) { settings->rgbG = (settings->rgbG + 64) % 256; }
          else { settings->rgbB = (settings->rgbB + 64) % 256; }
          rgb_step = (rgb_step+1)%3; showSettings(); break;
        case 2: settings->soundOn = !settings->soundOn; sound->begin(nullptr); showSettings(); break;
        case 3: settings->bootAnimation = !settings->bootAnimation; showSettings(); break;
        case 4: // reset defaults
          settings->brightness = 8; settings->soundOn=true; settings->bootAnimation=true; settings->rgbR=255;settings->rgbG=0;settings->rgbB=0; showSettings(); break;
        case 5: // save
          settings->save(); disp->scrollText_P(PSTR("SAVED")); break;
      }
    } else if (e==NAV_BTN_D) { state=MS_MAIN; showMain(); }
  }

  void showWiFiMenu() {
    disp->scrollText_P(PSTR("WIFI: SCAN->BTN_A CONNECT->BTN_B"));
  }
  void handleWiFi(NavEvent e) {
    if (e==NAV_BTN_A) { // scan
      Communication::request("WIFI:SCAN"); disp->scrollText_P(PSTR("SCAN...")); }
    else if (e==NAV_BTN_B) { // connect to predefined SSID (for demo)
      Communication::request("WIFI:CONNECT:myssid:password"); disp->scrollText_P(PSTR("CONNECTING")); }
    else if (e==NAV_BTN_D) { state=MS_MAIN; showMain(); }
    if (wifiRespReady) {
      disp->scrollText(wifiResp);
      wifiRespReady=false; wifiResp[0]=0;
    }
  }
};

#endif
