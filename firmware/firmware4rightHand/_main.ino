#define ALWAYS_CALIBRATING CALIBRATION_LOOPS == -1

#define CALIB_OVERRIDE false
#if USING_CALIB_PIN && COMMUNICATION == COMM_SERIAL && PIN_CALIB == 0 && !CALIB_OVERRIDE
  #error "You can't set your calibration pin to 0 over usb. You can calibrate with the BOOT button when using bluetooth only. Set CalibOverride to true to override this."
#endif

// Initiate an instance for communication
ICommunication* comm;
int loops = 0;

/***
Setup
1. way to communicate: Serial or Bluetooth
2. 
*/
void setup() {
  #if COMMUNICATION == COMM_SERIAL
    comm = new SerialCommunication();
    
    
  #elif COMMUNICATION == COMM_BTSERIAL
    comm = new BTSerialCommunication();
  #endif  
  comm->start(); // Start with given name, and set it to "OPEN"

  setupInputs(); 

  #if USING_FORCE_FEEDBACK
    setupServoHaptics();  // attach the given pin to finger channels
  #endif
  
}

void loop() {
  if (comm->isOpen()){
    #if USING_CALIB_PIN
    bool calibButton = getButton(PIN_CALIB) != INVERT_CALIB;
    if (calibButton)
      loops = 0;
    #else
    bool calibButton = false;
    #endif
    
    bool calibrate = false;
    if (loops < CALIBRATION_LOOPS || ALWAYS_CALIBRATING){
      calibrate = true;
      loops++;
    }
    
    int* fingerPos = getFingerPositions(calibrate, calibButton);
    bool joyButton = getButton(PIN_JOY_BTN) != INVERT_JOY;

    #if TRIGGER_GESTURE
    bool triggerButton = triggerGesture(fingerPos);
    #else
    bool triggerButton = getButton(PIN_TRIG_BTN) != INVERT_TRIGGER;
    #endif

    bool aButton = getButton(PIN_A_BTN) != INVERT_A;
    bool bButton = getButton(PIN_B_BTN) != INVERT_B;

    #if GRAB_GESTURE
    bool grabButton = grabGesture(fingerPos);
    #else
    bool grabButton = getButton(PIN_GRAB_BTN) != INVERT_GRAB;
    #endif

    #if PINCH_GESTURE
    bool pinchButton = pinchGesture(fingerPos);
    #else
    bool pinchButton = getButton(PIN_PNCH_BTN) != INVERT_PINCH;
    #endif

    bool menuButton = getButton(PIN_MENU_BTN) != INVERT_MENU;
    
    
    comm->output(encode(fingerPos, getJoyX(), getJoyY(), joyButton, triggerButton, aButton, bButton, grabButton, pinchButton, calibButton, menuButton));
    

    #if USING_FORCE_FEEDBACK
      char received[100];      
      if(comm->readData(received)){
        // Serial.println("received: "+ String(received)); //received: A662B684C790D600E272
        int hapticLimits[5];
        //This check is a temporary hack to fix an issue with haptics on v0.5 of the driver, will make it more snobby code later
        if(String(received).length() >= 10) {
           decodeData(received, hapticLimits);
          //  Serial.println("limits from UMD: "+ String(hapticLimits[1]) + String(hapticLimits[2]) + String(hapticLimits[3]) + String(hapticLimits[4])); // limits from UMD: 684790600272
           writeServoHaptics(hapticLimits); 
          }
        }
      
      // else{
      //   if (str[loops%5].length()>0){
      //   str[loops%5].toCharArray(received,str[loops%5].length());
      //   int hapticLimits[5];
      //   comm->output(received);
      //   // Serial.println("before received: "+ String(received));
      // }
        
      // }
    #endif
    delay(LOOP_TIME);
  }
}
