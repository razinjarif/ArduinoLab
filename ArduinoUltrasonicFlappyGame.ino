#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


const int trigPin = 9;
const int echoPin = 10;


int birdY = 32;
int pipeX = 127;
int pipeGapY = 30;
int score = 0;
int highScore = 0;
int gameSpeed = 3;
bool gameOver = false;


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;
  }


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25, 25);
  display.println("SENS-O-EASY MODE");
  display.display();
  delay(2000);
}


void loop() {
  if (gameOver) {
    if (score > highScore) highScore = score;
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(15, 10);
    display.println("GAME OVER!");
    display.print("Score: ");
    display.println(score);
    display.print("High: ");
    display.println(highScore);
    display.println("\nClose hand to Restart");
    display.display();


    if (readDistance() < 6) {
      gameOver = false;
      score = 0;
      pipeX = 127;
      delay(500);
    }
    return;
  }


  int dist = readDistance();
  int targetY = map(dist, 5, 35, 0, SCREEN_HEIGHT - 8);


  birdY = birdY + (targetY - birdY) * 0.4;
  birdY = constrain(birdY, 0, SCREEN_HEIGHT - 8);


  pipeX -= gameSpeed;


  if (pipeX < 0) {
    pipeX = 127;
    pipeGapY = random(5, 30);
    score++;
  }


  if (pipeX < 18 && pipeX > 8) {
    if (birdY < pipeGapY || birdY > pipeGapY + 28) {
      gameOver = true;
    }
  }


  display.clearDisplay();
  display.fillRect(12, birdY, 8, 8, WHITE);


  display.fillRect(pipeX, 0, 10, pipeGapY, WHITE);
  display.fillRect(pipeX, pipeGapY + 28, 10, SCREEN_HEIGHT, WHITE);


  display.setCursor(0, 0);
  display.print("S:");
  display.print(score);
  display.display();
  delay(35);
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
