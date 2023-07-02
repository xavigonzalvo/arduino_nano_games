#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BLOCK_SIZE 2

#define BUTTON_LEFT_PIN 2
#define BUTTON_RIGHT_PIN 3

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

byte gameBoard[SCREEN_WIDTH / BLOCK_SIZE][SCREEN_HEIGHT / BLOCK_SIZE] = {0};

int posX = SCREEN_WIDTH / 2;
int posY = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  delay(2000);

  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop() {
  display.clearDisplay();

  // Draw the game board
  for (int i = 0; i < SCREEN_WIDTH / BLOCK_SIZE; i++) {
    for (int j = 0; j < SCREEN_HEIGHT / BLOCK_SIZE; j++) {
      if (gameBoard[i][j] == 1) {
        for (int x = 0; x < BLOCK_SIZE; x++) {
          for (int y = 0; y < BLOCK_SIZE; y++) {
            display.drawPixel(i * BLOCK_SIZE + x, j * BLOCK_SIZE + y, WHITE);
          }
        }
      }
    }
  }

  // Draw the falling block
  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      display.drawPixel(posX + i, posY + j, WHITE);
    }
  }
  display.display();

  // Check for collisions with the game board or the bottom of the screen
  if (posY >= SCREEN_HEIGHT - BLOCK_SIZE ||
      gameBoard[posX / BLOCK_SIZE][(posY + BLOCK_SIZE) / BLOCK_SIZE] == 1) {
    gameBoard[posX / BLOCK_SIZE][posY / BLOCK_SIZE] = 1;
    posX = SCREEN_WIDTH / 2;
    posY = 0;
  } else {
    posY++;
  }

  // Move the block sideways
  if (digitalRead(BUTTON_LEFT_PIN) == LOW && posX > 0 && gameBoard[(posX - 1) / BLOCK_SIZE][posY / BLOCK_SIZE] == 0) {
    posX--;
    Serial.println("Left button pressed");
  }
  if (digitalRead(BUTTON_RIGHT_PIN) == LOW && posX < SCREEN_WIDTH - BLOCK_SIZE && gameBoard[(posX + BLOCK_SIZE) / BLOCK_SIZE][posY / BLOCK_SIZE] == 0) {
    posX++;
    Serial.println("Right button pressed");
  }
  delay(100);
}
