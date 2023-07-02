#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BLOCK_SIZE 2

#define BUTTON_JUMP_PIN 2
#define BUTTON_RESET_PIN 3

#define PLAYER_X 10
#define PLAYER_Y 40
#define PLAYER_SIZE 15
#define PLAYER_X_SIZE 5
#define FLOOR_HEIGHT 10

#define OBSTACLE_WIDTH 5
#define OBSTACLE_HEIGHT 6
#define OBSTACLE_SPEED 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int playerY = PLAYER_Y;
bool jumping = false;

int obstacleX = SCREEN_WIDTH;
int obstacleY = PLAYER_Y + PLAYER_SIZE - OBSTACLE_HEIGHT;

bool gameOver;
int score;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  delay(2000);

  pinMode(BUTTON_JUMP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RESET_PIN, INPUT_PULLUP);
  
  resetGame();
}

void drawPlayer(int x, int y) {
  // Draw the head
  display.drawPixel(x, y, WHITE);
  display.drawPixel(x, y + 1, WHITE);
  display.drawPixel(x - 1, y + 1, WHITE);
  display.drawPixel(x + 1, y + 1, WHITE);
  display.drawPixel(x - 1, y + 2, WHITE);
  display.drawPixel(x + 1, y + 2, WHITE);
  display.drawPixel(x - 1, y + 3, WHITE);
  display.drawPixel(x + 1, y + 3, WHITE);
  
  // Draw the body
  display.drawLine(x, y + 4, x, y + 9, WHITE);
  
  // Draw the left arm
  display.drawLine(x, y + 5, x - 4, y + 6, WHITE);
  display.drawLine(x - 4, y + 6, x - 5, y + 4, WHITE);
  
  // Draw the right arm
  display.drawLine(x, y + 5, x + 4, y + 6, WHITE);
  display.drawLine(x + 4, y + 6, x + 5, y + 4, WHITE);
  
  // Draw the left leg
  display.drawLine(x, y + 9, x - 3, y + 14, WHITE);
  display.drawLine(x - 3, y + 14, x - 1, y + 14, WHITE);
  
  // Draw the right leg
  display.drawLine(x, y + 9, x + 3, y + 14, WHITE);
  display.drawLine(x + 3, y + 14, x + 1, y + 14, WHITE);
}

void resetGame() {
  playerY = PLAYER_Y;
  jumping = false;
  gameOver = false;
  score = 0;

  obstacleX = SCREEN_WIDTH;
  obstacleY = PLAYER_Y + PLAYER_SIZE - OBSTACLE_HEIGHT;
}

void loop() {
  display.clearDisplay();

  if (gameOver) {
    // Draw "Game Over!" message
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(30, 30);
    display.println("Game Over!");
  }  else {
    // Draw the player
    drawPlayer(PLAYER_X, playerY);

    // Draw the obstacle
    display.fillRect(obstacleX, obstacleY, OBSTACLE_WIDTH, OBSTACLE_HEIGHT, WHITE);

    // Draw the floor
    display.drawLine(0, SCREEN_HEIGHT - FLOOR_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - FLOOR_HEIGHT, WHITE);

    // Check for collision with the obstacle
    if (PLAYER_X + PLAYER_X_SIZE > obstacleX &&
        PLAYER_X < obstacleX + OBSTACLE_WIDTH &&
        playerY + PLAYER_SIZE > obstacleY) {
      gameOver = true;
    }

    // Draw the score
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(SCREEN_WIDTH - 20, 0);
    display.println(score);
  }

  display.display();

  if (!gameOver) {
    // Move the obstacle
    obstacleX -= OBSTACLE_SPEED;
    if (obstacleX < -OBSTACLE_WIDTH) {
      obstacleX = SCREEN_WIDTH;
      score++;
    }
  
    // Handle jumping
    if (digitalRead(BUTTON_JUMP_PIN) == LOW && !jumping) {
      jumping = true;
    }
    if (jumping) {
      playerY -= 3;
      if (playerY <= PLAYER_Y - 30) {
        jumping = false;
      }
    } else if (playerY < PLAYER_Y) {
      playerY += 2;
    }
    if (playerY > PLAYER_Y) {
      playerY = PLAYER_Y;
    }
  }

  // Handle game reset
  if (digitalRead(BUTTON_RESET_PIN) == LOW) {
    resetGame();
  }

  delay(20);
}
