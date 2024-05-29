
#include <Arduino.h>
#include <Servo.h>

// put function declarations here:
// int myFunction(int, int);

const int trigPin1 = 6;
const int echoPin1 = 7;
float duration1, distance1;         // Duration used to calculate distance
Servo myservo1, myservo2, myservo3; // create servo object to control a servo
int count = 0; // count the number of times the top is detected. This prevents triggering due to noise.
int angle = 0; // servo angle
int v1 = 90 + 90; // intended speed
int v2 = 90 - 90; // intended speed
int dir1 = v1; // initial direction for servo1
int dir2 = v2; // initial direction for servo2
int DD = 100; // delay duration


// put functions here:
int P_speed_controlled(float distance, float Goal_Distance)
{
    float error = distance-Goal_Distance;
    float Kp = 1;
    float P = Kp * error;

    // map P to speed between 0 and 90
    int speed = map(P, 0, 10, 0, 90);

    return speed;
}

void setup()
{
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  // attach servos to pins
  myservo1.attach(5);
  myservo2.attach(4);
  myservo3.attach(3);

  // initialize serial communication
  Serial.begin(9600);
}

void loop()
{
  // echo sensor measuring and printing distance
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 * 0.0343) / 2;
  Serial.print("Distance1: ");
  Serial.println(distance1);

  if (count <=5 && distance1 > 25) // decelerate when approaching the tree top around 25cm
  {
    // compute the speed
    int speed = P_speed_controlled(distance1, 10);
  Serial.print("Speed: ");
  Serial.println(speed);

  v1 = 90 + speed;
  v2 = 90 - speed;

  dir1 = v1; // update direction with new speed
  dir2 = v2; // update direction with new speed

  }

  delay(DD);



  // check if the top is detected
  count = (distance1 < 20) ? count + 1 : count; // increment when less than 20cm away from the tree top

  // Serial.println(count); // for debugging

  // check if the top is detected 5 times, and that the servo has not switched direction yet
  if (count > 5 && dir1 == v1)
  {
    delay(5000); // sleep for 5 seconds before switching direction

    // switch direction, travel with new speed
    Serial.println("Negative direction");
    v1 = 90 + 5; // much slower descent speed
    v2 = 90 - 5; // much slower descent speed
    dir1 = v2; // switch direction
    dir2 = v1; // switch direction
    DD = 3000; // delay duration to prevent speed buildup.
  }
  // actuate servos
  myservo1.write(dir1);
  myservo2.write(dir2);
  myservo3.write(9);
}
