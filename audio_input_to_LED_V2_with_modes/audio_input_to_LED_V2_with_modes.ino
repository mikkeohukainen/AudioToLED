int potValue;

// mode switch variables
const int buttonPin = 2; // the number of the pushbutton pin
int buttonState = 0; // variable for reading the pushbutton status
int lastButtonState = 0; // previous state of the button
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50; // the debounce time in milliseconds

int mode = 0;

int red = 0;
int green = 0;
int blue = 0;

int originalRed = 0;
int originalGreen = 0;
int originalBlue = 0;

unsigned long startTime;  // for the delay of the color transition
int state = 0;  // to keep track of the color transition state
bool firstRound = true;


void setup() {

  Serial.begin(9600);

  pinMode(buttonPin, INPUT);

  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);

  startTime = millis();

  delay(3000);
}


void loop() {

  toggleModes();
  
  potValue = analogRead(A5);
  
  // read the input on analog pin 1:
  int sensorValue = analogRead(A1);
  int shiftedSensorValue = sensorValue - 511;

    if (shiftedSensorValue > 30)  // bias to compensate low amplitude of the signal
    {
      shiftedSensorValue = shiftedSensorValue * 1.2;
    }
    

  float brightnessFactor = map(shiftedSensorValue, 0, 500, 5, 255) / 255.0;

  if (shiftedSensorValue < 30 && shiftedSensorValue > 5)  // "filter out" the low readings
    {
      brightnessFactor = brightnessFactor * 0.1;
    }

  else if (shiftedSensorValue > 30)
    {
      brightnessFactor = brightnessFactor * 2;
    }
  else if (shiftedSensorValue > 50)   // emphasis on the higher readings
    {
      brightnessFactor = brightnessFactor * 2.5;
    }
  
  float voltage = sensorValue * (5.0 / 1023.0);


  if (mode == 0)
  {
    /*
    Color is set with the potentiometer
    and amplitude of the audio signal adjusts the brightness
    */
    setRGB_withPotBright_with_music(potValue, brightnessFactor);
  }
  else if (mode == 1)
  {
    /*
    Color is automatically fading through the spectrum
    and amplitude of the audio signal adjusts the brightness
    */
    setRGB_auto_Bright_with_music(brightnessFactor);
  }
  else if (mode == 2)
  {
    /*
    Color is set with the potentiometer
    brightness is static (max)
    */
    setRGB_withPot_BrightStatic(potValue);
  }
  else if (mode == 3)
  {
    /*
    Color is automatically fading through the spectrum
    brightness is static (max)
    */
    setRGB_auto_BrightStatic();
  }
  else if (mode == 4)
  {
    ledsOff();
  }
  
  analogWrite(6, red);
  analogWrite(3, green);
  analogWrite(9, blue);
}

void setRGB_withPotBright_with_music(int value, float brightness) 
{

  red = 0;
  green = 0;
  blue = 0;

  if (value < 205) {                       // Red to Yellow
    red = 255;  
    green = map(value, 0, 204, 0, 255);    
  } else if (value < 410) {                // Yellow to Green
    green = 255;
    red = map(value, 205, 409, 255, 0);   
  } else if (value < 615) {                // Green to Cyan
    green = 255;
    blue = map(value, 410, 614, 0, 255);
  } else if (value < 820) {                // Cyan to Blue
    blue = 255;
    green = map(value, 615, 819, 255, 0);  
  } else {                                 // Blue to Purple
    blue = 255;
    red = map(value, 820, 1023, 0, 255);
  }
  
  red = red * brightness;
  green = green * brightness;
  blue = blue * brightness;  
}


void setRGB_auto_Bright_with_music(float brightness)
{
  if (firstRound)
  {
    originalRed = red = 255;
    originalGreen = green = 0;
    originalBlue = blue = 0;

    firstRound = false;    
  }

  if (millis() - startTime > 10) 
  {
    switch(state) 
    {
        case 0:  // Red to Yellow
            if (originalGreen < 255) originalGreen++;
            else state++;
            break;
        case 1:  // Yellow to Green
            if (originalRed > 0) originalRed--;
            else state++;
            break;
        case 2:  // Green to Cyan
            if (originalBlue < 255) originalBlue++;
            else state++;
            break;
        case 3:  // Cyan to Blue
            if (originalGreen > 0) originalGreen--;
            else state++;
            break;
        case 4:  // Blue to Magenta
            if (originalRed < 255) originalRed++;
            else state++;
            break;
        case 5:  // Magenta to Red
            if (originalBlue > 0) originalBlue--;
            else state = 0;  // Reset to initial state
            break;
    }
    startTime = millis();
  }

  // Add the brightness factor to the color values

  red = originalRed * brightness;
  green = originalGreen * brightness;
  blue = originalBlue * brightness;  
}

void setRGB_withPot_BrightStatic(int value)
{
  red = 0;
  green = 0;
  blue = 0;

  if (value < 205) {                       // Red to Yellow
    red = 255;  
    green = map(value, 0, 204, 0, 255);    
  } else if (value < 410) {                // Yellow to Green
    green = 255;
    red = map(value, 205, 409, 255, 0);   
  } else if (value < 615) {                // Green to Cyan
    green = 255;
    blue = map(value, 410, 614, 0, 255);
  } else if (value < 820) {                // Cyan to Blue
    blue = 255;
    green = map(value, 615, 819, 255, 0);  
  } else {                                 // Blue to Purple
    blue = 255;
    red = map(value, 820, 1023, 0, 255);
  }
}

void setRGB_auto_BrightStatic()
{
  if (firstRound)
  {
    
    red = 255;
    green = 0;
    blue = 0;

    firstRound = false;    
  }

  if (millis() - startTime > 10) 
  {
    switch(state) 
    {
        case 0:  // Red to Yellow
            if (green < 255) green++;
            else state++;
            break;
        case 1:  // Yellow to Green
            if (red > 0) red--;
            else state++;
            break;
        case 2:  // Green to Cyan
            if (blue < 255) blue++;
            else state++;
            break;
        case 3:  // Cyan to Blue
            if (green > 0) green--;
            else state++;
            break;
        case 4:  // Blue to Magenta
            if (red < 255) red++;
            else state++;
            break;
        case 5:  // Magenta to Red
            if (blue > 0) blue--;
            else state = 0;  // Reset to initial state
            break;
    }
    startTime = millis();
  }
}

void ledsOff()
{
  red = 0;
  green = 0;
  blue = 0;
}

void toggleModes()
{
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if the button state changed
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        if (mode == 0) {
          mode = 1;
        } else if (mode == 1) {
          mode = 2;
        } else if (mode == 2) {
          mode = 3;
        } else if (mode == 3) {
          mode = 4;
        } else if (mode == 4) {
          mode = 0;
        }
      }
    }
  }

  // Reset the lastButtonState:
  lastButtonState = reading;

  Serial.println(mode);
}



