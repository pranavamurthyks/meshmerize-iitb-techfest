const int IN1 = 16; // RM Dir 1
const int IN2 = 17; // RM Dir 2
const int IN3 = 18; // LM Dir 1
const int IN4 = 19; // LM Dir 2
const int ENA = 4;  // PWM for RM
const int ENB = 5;  // PWM for LM

const int sensor_count = 8; 
const int ir_sensor_pins[8] = {14, 27, 26, 25, 33, 32, 35, 34};
int sensor_values[8];

int successive_white_iterations = 0;

float kp = 200;
float ki = 0;
float kd = 600;

float error = 0, last_error = 0;
float integral = 0;

int base_speed = 180;
int max_speed = 255;
int threshold = 500;

bool just_turned = false;

void setup()
{
  Serial.begin(115200);

  for (int i = 0; i < sensor_count; i++)
    pinMode(ir_sensor_pins[i], INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}


void loop()
{
  float position;
  int white_count = 0;
  int black_count = 0;


  for (int i = 0; i < sensor_count; i++)
  {
    sensor_values[i] = analogRead(ir_sensor_pins[i]);
    if (sensor_values[i] < threshold) white_count++;
    else black_count++;
  }

 if (black_count == sensor_count)
  {
    if (!just_turned)
    {
      just_turned = true;
      position = read_line_position();
      while (position >= 0)
      {
      turn_left();
      delay(2);
      for (int i = 0; i < sensor_count; i++)
      {
        sensor_values[i] = analogRead(ir_sensor_pins[i]);
      }
      position = read_line_position();
      }
      return;
    }
    else 
    {
      position = last_error;
      error = position;
    }
  }

  else 
  {
    position = read_line_position();
    error = position;
    last_error = error; 
  }

  integral += error;
  float derivative = error - last_error;
  float correction = kp * error + kd * derivative + ki * integral;



  // Turns
  if ((white_count == sensor_count) && !just_turned)
  {
    turn_left();
    delay(300);
    just_turned = true;
    for (int i = 0; i < sensor_count; i++)
    {
      sensor_values[i] = analogRead(ir_sensor_pins[i]);
    }
    position = read_line_position();

    while (position >= 0) // Check again
    {
      turn_left();
      delay(5);
      for (int i = 0; i < sensor_count; i++)
      {
        sensor_values[i] = analogRead(ir_sensor_pins[i]);
      }
    
      position = read_line_position();
    }
    return;
  }




  else if ((position > 1) && !just_turned)
  {
    just_turned = true;
    move_motors(150, 150);
    delay(30);

    black_count = 0;
    for (int i = 0; i < sensor_count; i++)
    {
      sensor_values[i] = analogRead(ir_sensor_pins[i]);
      if (sensor_values[i] > threshold) black_count++;
    }

    if (black_count == sensor_count)
    {
      turn_right();
      delay(200);
      for (int i = 0; i < sensor_count; i++)
      {
        sensor_values[i] = analogRead(ir_sensor_pins[i]);
      }
      position = read_line_position();

      while (position <= 0)
      {
        turn_right();
        delay(5);
        for (int i = 0; i < sensor_count; i++)
        {
          sensor_values[i] = analogRead(ir_sensor_pins[i]);
        }
    
        position = read_line_position();
      }
    }
    return;
  }
    



  else if ((position < -1) && !just_turned)
  {
    turn_left();
    delay(200);
    just_turned = true;
    for (int i = 0; i < sensor_count; i++)
    {
      sensor_values[i] = analogRead(ir_sensor_pins[i]);
    }
    position = read_line_position();
  
    while (position >= 0)
    {
      turn_left();
      delay(5);
      for (int i = 0; i < sensor_count; i++)
      {
        sensor_values[i] = analogRead(ir_sensor_pins[i]);
      }
      position = read_line_position();
    }
    return;
  }


  
  int left_speed = base_speed + correction;
  int right_speed = base_speed - correction;

  left_speed = constrain(left_speed, 0, max_speed);
  right_speed = constrain(right_speed, 0, max_speed);

  move_motors(left_speed, right_speed);
  last_error = error;
  Serial.print("Correction: "); Serial.print(correction);
  Serial.print(" | L: "); Serial.print(left_speed);
  Serial.print(" R: "); Serial.println(right_speed);

  if (position > -1.0 && position < 1.0)
  {
    just_turned = false;
  }


  delay(5);
  
  
}


void turn_right()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 255);
}

void turn_left()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 255);
  analogWrite(ENB, 0);
}

void turn_left_reverse()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 255);
  analogWrite(ENB, 150);
}

float read_line_position()
{
  float weighted_sum = 0;
  long sum = 0;
  float weights[8] = {-5.5 , -3, -1, 0, 0, 1, 3, 5.5};

  for (int i = 0; i < sensor_count; i++)
  {
    int val = (sensor_values[i] < threshold) ? 1000 : 0;
    weighted_sum += val * weights[i];
    sum += val;
  }

  if (sum == 0) return 0;
  return (float)weighted_sum / sum;
}


void move_motors(int left_speed, int right_speed)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, right_speed);
  analogWrite(ENB, left_speed);
}

void stop_motors()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
