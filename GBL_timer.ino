#include <Servo.h>

// Sensor & Actuators

// ANGLE ADJUSTOR
Servo servo1;
int AngleServoPin = 9;  //Angle adjustment servo

// TRIGGER SERVO
Servo servo2;
int TriggerServo = 10;  // Trigger servo

// PHOTOTRANSISTOR
int Led = 8;   //Led for phototransistor
int lightLED = 6;

float threshold = 0.25;    // voltage where phototransistor shows car is in the way
const int interval = 5;  // accuracy of timer in milliseconds
double max_dist;
double error = 100;
int angle_limit = 70;
float velocity;


int Trigger;   // Trigger command
float v1, timerCount;
boolean waitFor2ndTrigger;

// Dynamic Model Settings
// 1. Mutable Parameters
double platform_height = 50 * 0.0254; // [in] First number is distance in inches, second value is conversion factor
double bucket_dist = 184/12 * 0.3048;      //  [ft] First number is distance in feet, second value is conversion factor
int scenarioCase = 2;                // Direct Launch -> 1; Single Bounce Launch -> 2
double loss_coeff = .3;              // Percentage of energy lost due to friction (need to calibrate)
int correction = 10;

// 2. Constants
double bucket_height = 15 * 0.0254; // [m] First number is distance in inches, second value is conversion factor
double bucket_width = 13 * 0.0254;    // [m] First number is distance in inches, second value is conversion factor
int plate_width = 13 * .0254;
double g = 9.81;             // [m/s^2]
double m = .0459;            // Mass of Golf Ball[kg]   
double k = 9 * 175.126835;   // Spring Constant [N/m]; First number is spring constant in lbs/in, second value is conversion factor
double d = 2 * 0.0254;     // Spring Compression Distance [m]; First number is distance in inches, second value is conversion factor
double l = 6 * 0.0254;     // Length of Canister [m]; First number is distance in inches, second value is conversion factor
double pi = 3.1415;
double d_golfball = 1.680 * .0254;  // Diameter of Golf Ball [m]
double canister_height = 6 * .0254;
double adj_dist = bucket_dist + plate_width;
double adj_height = platform_height + canister_height;


double mid_dist = (bucket_width + 2 * bucket_dist)/2;

int count = 0;
double sum = 0;
double avg = 0; 
double startTime;
double endTime;


//______________________________________________________________________________________________________________________________________________
void setup()                                 // Built-in initialization block
{
  Serial.begin(9600);                        // Set data rate to 9600 bps
  servo1.attach(AngleServoPin);              // Initialize Angle Servo
  servo2.attach(TriggerServo);               // Initialize Trigger Servo
  pinMode(Led, OUTPUT);                      // Initialize LED
  pinMode(lightLED, OUTPUT);
  
  //servo1.write(0);
  servo2.write(0);
}
//______________________________________________________________________________________________________________________________________________
void loop() {
  digitalWrite(Led, HIGH);   // Turn Led on
  digitalWrite(lightLED, HIGH);
  int angle = dynamicModel(adj_dist, adj_height, scenarioCase);
  Serial.print("Launch Angle: ");
  Serial.print(angle);
  Serial.println(" degrees");
  servo1.write(angle + correction);

  Serial.println("Do you want to launch? (type 1 for Yes or 0 for No");     //Prompt User for Trigger command
  while (Serial.available() == 0) {    
  }
  Trigger = Serial.parseInt(); //Read launch command         
  if (Trigger == 1) {
    servo2.write(90);          //Trigger launch Mechanism
  }
  else {
    servo2.write(0);
  }

  // LED Code
  timerCount = 0;
  waitFor2ndTrigger = true;
  v1 = volts(A1);                                // measure voltage from phototransister in A1
  Serial.println("Waiting for trigger...");      // wait for  phototransistor to be dimmed
  while (v1 > threshold) { 
   delay(interval);                   // otherwise it's too fast to notice button press ((when one is used)
   v1 = volts(A1);                    // measure voltage from phototransister in A1
    }
  Serial.print("Started...");                    // Display "Start"
  
  while (waitFor2ndTrigger) {              // count time until phototransistor receives light again
    delay(interval);                       // Delay for defined time
    timerCount = timerCount+interval;      // Add time to counter
    v1 = volts(A1);                        // check A1 phototransistor
    waitFor2ndTrigger = (v1 < threshold);  // is "false" when phototransistor receives light again
    }
  
  Serial.print("Velocity: ");
  Serial.print(velocity);
  Serial.println(" m/s");

  // end timer count and display results
  //Serial.println("Stopped");             // Display " sec." & newline
  //Serial.print("Final Time = ");          // Display "Final Time = "
  //Serial.print(timerCount/1000);          // Display timerCount in #.### format instead of milliseconds
  //Serial.println(" seconds");             // Display " sec." & newline
  //Serial.print("Speed = ");               // Display "Speed = "
  //Serial.print(1.680/(timerCount/1000));     
  //Serial.println(" in/sec");              // Display "m/sec" & newline
  //Serial.println("");                     // print blank line*/
  delay(3000);                            // wait 3 seconds and reset
}
                                           
float volts(int adPin)                       // Measures volts at adPin
{                                            // Returns floating point voltage
 return float(analogRead(adPin)) * 5.0 / 1024.0;
}    

float quadratic (double a, double b, double c){
  double x;
  double discriminant = sq(b) - 4 * a * c;
  if (discriminant < 0){
    x = 0;
  }
  else if (discriminant = 0){
    x = -b/(2 * a);
  }
  else{
    double x1 = (-b + sqrt(discriminant))/(2*a);
    double x2 = (-b - sqrt(discriminant))/(2*a);
    //Serial.print("x1: ");
    //Serial.print(x1);
    //Serial.print("; x2: ");
   // Serial.println(x2);
    if ((x1 > 0) and (x2 <= 0)){
      x = x1;
    }
    else if ((x1 <= 0) and (x2 > 0)){
      x = x2;
    }
    else{
      if ((x1 >= x2)){
        x = x1;
      }
      else{
        x = x2;
      }
    }
  }
  return x;
}
float dynamicModel (double bucketDist, double platformHeight, int scenario){
  //Serial.println(mid_dist);
  for (int theta = 0; theta <= angle_limit; theta++){
    double thetar = theta * pi/180;
    double PE1 = .5 * k * sq(d) + m*g*(1-d)*sin(thetar);
    double PE2 = m*g*l*sin(thetar);
    double vi = sqrt(2 * (PE1 - PE2)/m);
    double viy = vi * sin(thetar);
    double vix = vi * cos(thetar);
    /*Serial.print("Theta: ");
    Serial.println(theta);//*/
    double t = quadratic(-.5 * g, viy, platformHeight);
    double dist_trav = vix * t; 
    //Serial.print("Initial Velocity: ");
    //Serial.print(vi);
    //Serial.println(" m/s");
    //Serial.println(t);
    //Serial.println(scenario);
    //Serial.print("Sum: ");
    //Serial.println(sum);
    if (scenario == 1){
        double entrance_time = bucketDist/vix;
        double y_entry = platformHeight + viy*entrance_time - .5*g*sq(entrance_time);
        /*Serial.print(" ; Distance Traveled: ");
        Serial.print(dist_trav);
        Serial.print(" ; Needed Distance: ");
        Serial.print(bucketDist);
        Serial.print(" - ");
        Serial.println(bucketDist + bucket_width);//*/
        if ((dist_trav <= (bucketDist + bucket_width)) and ((dist_trav >= bucketDist)) and (y_entry > bucket_height)){
          //Serial.println(error);
          if (error > abs(mid_dist - dist_trav)){
            error = abs(mid_dist - dist_trav);
            avg = theta;
            velocity = vi;
          }
        }
        else{
          /*if (dist_trav > max_dist){
            max_dist = dist_trav;
            avg = theta;*/
          }
        }
    else if (scenario == 2){
      if (dist_trav < bucketDist){
        double vf = sqrt(2 * g * platformHeight + sq(vi));
        double vfy = sqrt(sq(vf) - sq(vix));
        double ref_angle = atan(vfy/vix);
        double KE2 = (1 - loss_coeff) * (.5 * m * sq(vf));
        double vref = sqrt(2 * KE2/m);            // Velocity following the bounce
        //Serial.println(vref);
        double vref_x = vref * cos(ref_angle);
        double vref_y = vref * sin(ref_angle);
        double t2 = quadratic(-.5*g, vref_y, 0);
        double bounce_dist = t2 * vref_x;
        double total_dist = bounce_dist + dist_trav;
        double entrance_time = bucketDist/vref_x;
        double y_entry = platformHeight + vref_y*entrance_time - .5*g*sq(entrance_time);
        /*Serial.print("Vx: ");
        Serial.print(vix);
        Serial.print(" ;Vrefx: ");
        Serial.print(vref_x);*/
        
        /*Serial.print(" ; Distance Traveled: ");
        Serial.print(dist_trav);
        Serial.print(" + ");
        Serial.print(bounce_dist);
        Serial.print(" = ");
        Serial.print(total_dist);
        Serial.print(" ; Needed Distance: ");
        Serial.print(bucketDist);
        Serial.print(" - ");
        Serial.println(bucketDist + bucket_width);//*/
        if ((total_dist <= (bucketDist + bucket_width)) and (total_dist >= bucketDist) and (y_entry > bucket_height)){
          avg = theta;
          velocity = vi;
        }
        
      }
      
    
   }
   
    }
   
   
   
   return avg;
}
