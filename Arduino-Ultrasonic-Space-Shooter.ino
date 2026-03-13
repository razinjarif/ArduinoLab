#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int trigPin = 9;
const int echoPin = 10;

int shipY = 32;
int enemyX = 127;
int enemyY = 30;
int bulletX = 15;
int bulletY = 32;
bool bulletActive = false;
int score = 0;
int highScore = 0;
bool gameOver = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 25);
  display.println("SPACE SHOOTER");
  display.display();
  delay(2000);
}

void loop() {
  if (gameOver) {
    if (score > highScore) highScore = score;
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20, 10);
    display.println("MISSION FAILED!");
    display.setCursor(20, 30);
    display.print("Score: "); display.println(score);
    display.setCursor(20, 40);
    display.print("High: "); display.println(highScore);
    display.setCursor(10, 55);
    display.println("Close hand to Restart");
    display.display();
    
    if(readDistance() < 6) {
       gameOver = false;
       score = 0;
       enemyX = 127;
       bulletActive = false;
       delay(500);
    }
    return;
  }

  int dist = readDistance();
  int targetY = map(dist, 5, 35, 0, SCREEN_HEIGHT - 8);
  shipY = shipY + (targetY - shipY) * 0.3; 
  shipY = constrain(shipY, 0, SCREEN_HEIGHT - 8);

  enemyX -= 3;
  if (enemyX < 0) {
    enemyX = 127;
    enemyY = random(0, SCREEN_HEIGHT - 8);
  }

  if (!bulletActive) {
    bulletX = 15;
    bulletY = shipY + 3;
    bulletActive = true;
  } else {
    bulletX += 6;
    if (bulletX > 127) bulletActive = false;
  }

  if (bulletActive && bulletX >= enemyX && bulletX <= enemyX + 8 && bulletY >= enemyY && bulletY <= enemyY + 8) {
    score++;
    enemyX = 127;
    enemyY = random(0, SCREEN_HEIGHT - 8);
    bulletActive = false;
  }

  if (enemyX < 15 && enemyX > 5 && shipY + 8 > enemyY && shipY < enemyY + 8) {
    gameOver = true;
  }

  display.clearDisplay();
  
  display.drawLine(5, shipY, 5, shipY + 8, WHITE);
  display.drawLine(5, shipY, 12, shipY + 4, WHITE);
  display.drawLine(5, shipY + 8, 12, shipY + 4, WHITE);
  
  display.fillRect(enemyX, enemyY, 8, 8, WHITE);
  
  if (bulletActive) {
    display.drawPixel(bulletX, bulletY, WHITE);
    display.drawPixel(bulletX + 1, bulletY, WHITE);
  }

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("S:"); display.print(score);
  display.display();
  delay(20); 
}

int readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  int d = duration * 0.034 / 2;
  if (d <= 0) return 35; 
  return d;
}

