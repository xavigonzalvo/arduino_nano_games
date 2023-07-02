#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BLOCK_SIZE 2

#define BUTTON_JUMP_PIN 2

#define PLAYER_X 10
#define PLAYER_Y 50
#define PLAYER_SIZE 5
#define FLOOR_HEIGHT 10

#define OBSTACLE_WIDTH 5
#define OBSTACLE_HEIGHT 5
#define OBSTACLE_SPEED 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int playerY = PLAYER_Y;
bool jumping = false;

int obstacleX = SCREEN_WIDTH;
int obstacleY = PLAYER_Y + PLAYER_SIZE - OBSTACLE_HEIGHT;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  delay(2000);

  pinMode(BUTTON_JUMP_PIN, INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop() {
  display.clearDisplay();

  // Draw the player
  display.fillRect(PLAYER_X, playerY, PLAYER_SIZE, PLAYER_SIZE, WHITE);

  // Draw the obstacle
  display.fillRect(obstacleX, obstacleY, OBSTACLE_WIDTH, OBSTACLE_HEIGHT, WHITE);

  // Draw the floor
  display.drawLine(0, SCREEN_HEIGHT - FLOOR_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - FLOOR_HEIGHT, WHITE);
  
  display.display();

  // Move the obstacle
  obstacleX -= OBSTACLE_SPEED;
  if (obstacleX < -OBSTACLE_WIDTH) {
    obstacleX = SCREEN_WIDTH;
  }

  // Check for collision with the obstacle
  if (PLAYER_X + PLAYER_SIZE > obstacleX && PLAYER_X < obstacleX + OBSTACLE_WIDTH && playerY + PLAYER_SIZE > obstacleY) {
    Serial.println("Game Over");
    while (1);
  }

  // Handle jumping
  if (digitalRead(BUTTON_JUMP_PIN) == LOW && !jumping) {
    jumping = true;
  }
  if (jumping) {
    playerY -= 2;
    if (playerY <= PLAYER_Y - 20) {
      jumping = false;
    }
  } else if (playerY < PLAYER_Y) {
    playerY += 2;
  }

  delay(20);
}
