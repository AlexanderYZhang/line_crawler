#include <Adafruit_NeoPixel.h>
#include <Adafruit_CircuitPlayground.h>
#include <avr/power.h>
#define PIN 10
#define NUMPIXELS 60
#define BUTTON 6

struct enemy {
  int health;
  int pos;
  long color;
};

int position = 0;
int health = 2;
int level = 1;
int beam_length = 2;
enemy* enemies;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  createLevel1();
  pixels.begin();
  digitalWrite(BUTTON, HIGH);
}

void createLevel1() {
  
  enemy arr[] = {
    {2,15, pixels.Color(150,150,150)},
    {2,30, pixels.Color(150,150,100)}
  };
  enemies = arr;

  for (int i = 0; i < sizeof(enemies); i++) {
    pixels.setPixelColor(enemies[i].pos, enemies[i].color);
  }
  pixels.show();
}

void attack() {
   for (int i = 1; i <= beam_length; i++) {
     if (pixels.getPixelColor(position + i) != 0) {
        for (int j = 0; j < sizeof(enemies); j++) {
          if (enemies[j].pos == (position + i) && enemies[j].health != 0) {
            enemies[j].health--;
            if (enemies[j].health == 0) {
              enemies[j].color = pixels.Color(0,0,0);
            }
            pixels.setPixelColor(position+i, pixels.Color(255,0,0));
          }
        }
     } else {
      pixels.setPixelColor(position + i, pixels.Color(150,150,0));
     }
   }
 
}
void remove_attack() {
  boolean remove = true;
  for (int i = 1; i <= beam_length; i++) {
    for (int j = 0; j < sizeof(enemies); j++) {
      if (enemies[j].pos == (position + i)) {
        remove = false;
        pixels.setPixelColor(position + i, enemies[j].color);
      }
    }
    if (remove) {
      pixels.setPixelColor(position + i, 0,0,0);
    }
    remove = true;
  }
}

void check_collision(int topbottom) {
  boolean collide = false;
  for (int i = 0; i < sizeof(enemies); i++) {
    if (enemies[i].pos == position) {
      pixels.setPixelColor(position, pixels.Color(255,0,0));
      collide = true;
      health--;
      break;
    }
  }
  pixels.setPixelColor(position+topbottom, 0,0,0);
  if (!collide) {
    pixels.setPixelColor(position, pixels.Color(0,150,0));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(120);
  int buttonState = digitalRead(BUTTON);
  int topbottom = 0;
  if (CircuitPlayground.leftButton() && position != 0) {
    position--;
    topbottom = 1;
  } else if(CircuitPlayground.rightButton() && position != 60) {
    position++;
    topbottom = -1;
  }
  check_collision(topbottom);
  if (buttonState == LOW) {
    attack();
  } else {
    remove_attack();
  }
  pixels.show();
}
