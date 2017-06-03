#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <stdint.h>

#include "constants.h"
#include "colors.h"
#include "levels.h"


char *field;

uint32_t colored[PIXELS_COUNT];

Adafruit_NeoPixel *pixels;
Adafruit_NeoPixel strips[STRIPS_COUNT] = {
  Adafruit_NeoPixel(PIXELS_COUNT_PER_STRIP, PIN1, NEO_GRB + NEO_KHZ400),
  Adafruit_NeoPixel(PIXELS_COUNT_PER_STRIP, PIN2, NEO_GRB + NEO_KHZ400),
  Adafruit_NeoPixel(PIXELS_COUNT_PER_STRIP, PIN3, NEO_GRB + NEO_KHZ400),
  Adafruit_NeoPixel(PIXELS_COUNT_PER_STRIP, PIN4, NEO_GRB + NEO_KHZ400),
  Adafruit_NeoPixel(PIXELS_COUNT_PER_STRIP, PIN5, NEO_GRB + NEO_KHZ400)};

char player_index = 0;
char player_type  = 'P';

char cur_strip = 0;
char cur_level = 1;

unsigned long speed_time = 0;
unsigned long blink_time = 0;
unsigned long tmp_lava = 0;

// const long debounce = 200;
// long time = 0;
// int previous = LOW;
// bool start = false;


void setup() {
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  pinMode(SW_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  begin_all_strips();
  level_init(cur_level);
  show_all_strips();

  pixels = &strips[0];
}


void loop() {
//  Serial.print("player_index: "); Serial.println((int)player_index);
  if (check_final()) {
    finish_movie();
    level_init(++cur_level);
  }
  check_lava();
//  end_effect();
  movement();
  if (collision()) {
    player_index = 0;
  }
  refreshLeds();
}


void finish_movie() {
  field = LEVEL_END;

  while (digitalRead(SW_PIN)) {
    end_effect();
    refreshLeds();
  }
}

bool check_final() {
  return field[player_index] == 'E';
  // return (player_index == 8);
}

void level_init(int cur_level) {
Serial.println("level_init");
  switch(cur_level) {
    case 1: field = FIELD1; break;
    case 2: field = FIELD2; break;

    default: field = FIELD1; break;
  }
  player_index = 0;
}

bool collision() {
  return (field[player_index] == 'L');
}

void check_lava() {
   if(millis() >= (tmp_lava + 1200)) {
    tmp_lava = millis();
    for (int i = 0; i < PIXELS_COUNT; i++) {
      if (field[i] == 'L')
        field[i] = 'l';
      else if(field[i] == 'l')
        field[i] = 'L';
     }
   }
}

void end_effect() {
   if(millis() >= (tmp_lava + 200)) {
    tmp_lava = millis();
    for (int i = 0; i < PIXELS_COUNT; i++) {
      if (field[i] == 'Y')
        field[i] = 'P';
      else if(field[i] == 'P')
        field[i] = 'Y';
     }
   }
}

void refreshLeds() {
  cast_field();
  upload_colors();
}


void movement() {
  int yPosition = 500;
  if((speed_time + 200) < millis() ) {
      speed_time = millis();
      yPosition = analogRead(Y_PIN);
   }

  if((blink_time + 150) < millis() ) {
    blink_time = millis();
    player_type = (player_type == 'P') ? 'p' : 'P';
  }

  if (yPosition < 400) {
    if (player_index < 79)
      player_index++;
  } else if (yPosition > 600) {
    if (player_index > 0)
      player_index--;
  }
}

bool configure_strip_usage(int& index) {
  if (index >= PIXELS_COUNT_PER_STRIP) {
    index = 0;
    if (cur_strip == 4)  {
      cur_strip = 0;
      pixels = &strips[cur_strip];
      return true;
    }
    pixels = &strips[++cur_strip];
  } else if (index < 0) {
    index = 0;
    if (cur_strip != 0) {
      index = PIXELS_COUNT_PER_STRIP - 1;
      pixels = &strips[--cur_strip];
    }
  }
  return false;
}

void cast_field() {
  for (int i = 0; i < PIXELS_COUNT; i++) {
    switch(field[i]) {
      case 'L': colored[i] = RED; break;
      case 'l': colored[i] = GREEN; break;
      case 'Y': colored[i] = YELLOW; break;
      case 'P': colored[i] = PINK; break;
      case 'M': colored[i] = MAGENTA; break;
      case 'E': colored[i] = WHITE; break;
      default: colored[i] = OFF; break;
    }
    if(player_type == 'P')
      colored[player_index] = BLUE;
    else
      colored[player_index] = PLAYER;
  }
}



void upload_colors() {
  int index = 0;
  for (int i = 0; !configure_strip_usage(index); index++) {
    uint32_t color = colored[i++];
    pixels->setPixelColor(index, color);
  }
  show_all_strips();
}

void begin_all_strips() {
  for (int i = 0; i < STRIPS_COUNT; i++) strips[i].begin();
}

void show_all_strips() {
  for (int i = 0; i < STRIPS_COUNT; i++) strips[i].show();
}

void drop_all_strips() {
  for (int i = 0; i < STRIPS_COUNT; i++) strips[i].clear();
}
