#include <EEPROM.h> //Used to save setpoint when power-off
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to the Arduino digital pin 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Arduino Pins
#define SSR_PIN 21

// Variables
uint8_t state = 0;
float real_temp; // We will store here the real temp
float prev_temp;
float Setpoint;                         // In degrees C
float SetpointDiff = 10;                // In degrees C
float elapsedTime, now_time, prev_time; // Variables for time control
float refresh_rate = 1000;              // Ramp up and cool down refresh rate
float pid_refresh_rate = 60000;         // PID refresh rate
float now_pid_error, prev_pid_error;

// --- ROBUST LOGGING VARIABLES ---
float loggedTemps[31]; // Array for minutes 0 through 30 (31 points total)
int loggedPWMs[31];    // Array to track the PWM value at each minute
float loggedP[31];
float loggedI[31];
float loggedD[31];
int sampleCount = 0;
bool isLogging = false;
bool initialPID = true;

unsigned long startTime = 0;
unsigned long lastPrintTime = 0; // For the 1-second serial print

int currentPWM = 0; // Tracks the current heater output (0-255)

///////////////////PID constants///////////////////////
float kp = 6.977124;
float ki = 0.0064603;
float kd = 1302.39648;
float PID_p, PID_i, PID_d, PID_total;
///////////////////////////////////////////////////////

void setup(void)
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);

  // Start up the library
  sensors.begin();

  pinMode(SSR_PIN, OUTPUT);
  digitalWrite(SSR_PIN, HIGH); // When HIGH, the SSR is Off

  Serial.println("--- PID TEST ---");
  Serial.println("Enter temperature setpoint (25-150) to begin:");

  while (Serial.available() == 0)
  {
  };

  Setpoint = Serial.parseFloat();

  // Clear the initial character sent so it doesn't immediately trigger the 30-min log
  while (Serial.available() > 0)
  {
    Serial.read();
  }

  Setpoint = constrain(Setpoint, 25, 150);

  Serial.println("Waiting for temperature to reset...");
  sensors.requestTemperatures();
  real_temp = (sensors.getTempCByIndex(0));
  delay(5000);

  Serial.print("\n>>> Setpoint = ");
  Serial.print(Setpoint); // This prints the variable value
  Serial.println(" received! Starting 30-minute logging (Minute 0 logged now).");
  isLogging = true;
  sampleCount = 0;
  startTime = millis(); // Record exact start time for drift-free calculation
}

void loop(void)
{
  // 1. Always request and read temperatures at the start of the loop
  sensors.requestTemperatures();
  real_temp = (sensors.getTempCByIndex(0));

  // 3. Handle the 30-minute recording sequence
  if (isLogging)
  {
    unsigned long currentMillis = millis();

    // Calculate the exact time the NEXT sample should be taken (sampleCount * 60,000 ms).
    // This method entirely prevents cumulative time drift.
    unsigned long targetTime = (unsigned long)sampleCount * 60000UL;

    if (currentMillis - startTime >= targetTime)
    {
      // Record the data into the arrays
      loggedTemps[sampleCount] = real_temp;
      loggedPWMs[sampleCount] = currentPWM;
      loggedP[sampleCount] = PID_p;
      loggedI[sampleCount] = PID_i;
      loggedD[sampleCount] = PID_d;

      // Print progress
      Serial.print(">>> Logged minute ");
      Serial.print(sampleCount);
      Serial.print(": Temp = ");
      Serial.print(real_temp);
      Serial.print(" C | PWM = ");
      Serial.print(currentPWM);
      Serial.print(" | P = ");
      Serial.print(PID_p);
      Serial.print(" | I = ");
      Serial.print(PID_i);
      Serial.print(" | D = ");
      Serial.print(PID_d);
      Serial.print(" | Total = ");
      Serial.println(PID_total);

      // 4. After 30 minutes, print CSV
      if (sampleCount == 30)
      {
        isLogging = false;

        Serial.println("\n--- 30 MINUTE TEST COMPLETE ---");
        Serial.println("Minute,Temperature(C),PWM,P,I,D");

        // Print all 31 data points in vertical CSV format
        for (int i = 0; i <= 30; i++)
        {
          Serial.print(i);
          Serial.print(",");
          Serial.print(loggedTemps[i]);
          Serial.print(",");
          Serial.print(loggedPWMs[i]);
          Serial.print(",");
          Serial.print(loggedP[i]);
          Serial.print(",");
          Serial.print(loggedI[i]);
          Serial.print(",");
          Serial.println(loggedD[i]);
        }

        Serial.println("-------------------------------");
        Serial.println("Log finished. Data is ready for copy/paste.");
      }

      // Increment sample count for the next minute
      sampleCount++;
    }
  }

  // 5. Non-blocking 1-second standard serial print
  if ((millis() - lastPrintTime >= 1000))
  {
    lastPrintTime = millis();
    Serial.print(": Temp = ");
    Serial.print(real_temp);
    Serial.print(" C | PWM = ");
    Serial.print(currentPWM);
    Serial.print(" | P = ");
    Serial.print(PID_p);
    Serial.print(" | I = ");
    Serial.print(PID_i);
    Serial.print(" | D = ");
    Serial.print(PID_d);
    Serial.print(" | Total = ");
    Serial.println(PID_total);
  }

  // 6. State Machine for Heating Control
  if (state == 0)
  {
    ramp_up();
  }
  else if (state == 1)
  {
    PID_control();
  }
  else
  {
    cool_down();
  }
}

// Function for ramping up the temperature
void ramp_up(void)
{
  // Rising temperature to (Setpoint - SetpointDiff)
  elapsedTime = millis() - prev_time;
  if (elapsedTime > refresh_rate)
  {
    if (real_temp < (Setpoint - SetpointDiff))
    {
      digitalWrite(SSR_PIN, LOW); // Turn On SSR
      currentPWM = 255;           // Log as fully ON
    }
    else
    {
      digitalWrite(SSR_PIN, HIGH); // Turn Off SSR
      currentPWM = 0;              // Log as fully OFF
      state = 1;                   // Already hot so we go to PID control
    }

    prev_time = millis();
  }
} // End of ramp_up loop

// Main PID compute and execute function
void PID_control(void)
{
  elapsedTime = millis() - prev_time;
  if (elapsedTime > pid_refresh_rate || initialPID)
  {
    if (initialPID)
    {
      Serial.println("First PID run");
      prev_temp = real_temp;
      real_temp = (sensors.getTempCByIndex(0));
      Serial.print("Prev Temp: ");
      Serial.print(prev_temp);
      Serial.print(" | Real Temp: ");
      Serial.print(real_temp);
    }
    initialPID = false;
    // 1. We calculate the error
    Serial.println("Calculating PID...");
    now_pid_error = Setpoint - real_temp;

    // 2. We calculate PID values
    PID_p = kp * now_pid_error;
    Serial.print("PID Error: ");
    Serial.print(now_pid_error);
    Serial.print("P: ");
    Serial.println(PID_p);
    PID_d = -kd * ((real_temp - prev_temp) / (pid_refresh_rate / 1000.0));

    // 2.2 Decide if we apply I or not. Only when error is very small
    PID_i = PID_i + (ki * now_pid_error);
    PID_i = constrain(PID_i, 0, 255); // anti-windup

    // 3. Calculate and constrain total PID value safely
    PID_total = PID_p + PID_i + PID_d;
    if (PID_total < 0)
      PID_total = 0;
    else if (PID_total < 4)
      PID_total = 4;
    if (PID_total > 255)
      PID_total = 255;

    // 4. Track current PWM for the logger
    currentPWM = (int)PID_total;

    // 5. Write PWM signal to the SSR (Active LOW: 255 is OFF, 0 is fully ON)
    analogWrite(SSR_PIN, 255 - currentPWM);

    // 6. Save values for next loop
    prev_time = millis();           // Store time for next loop
    prev_pid_error = now_pid_error; // Store error for next loop
    prev_temp = real_temp;
  }
} // End PID_control loop

// Function for turning off everything and monitoring the cooling down process
void cool_down(void)
{
  digitalWrite(SSR_PIN, HIGH); // SSR is OFF with HIGH pulse!
  currentPWM = 0;              // Log as fully OFF

  elapsedTime = millis() - prev_time;
  if (elapsedTime > refresh_rate)
  {
    prev_time = millis();
  }
} // End cool_down loop