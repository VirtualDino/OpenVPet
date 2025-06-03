/////////////////////////////////////////////////////////////////
/*
  main.cpp - main programm to test the VPetLCD Class and the
  screen classes. JUST FOR TESTING PURPOSES This is the *.ino file
  from the arduino ide
  Created by Berat Özdemir, January 16 , 2021.
*/
/////////////////////////////////////////////////////////////////

#include "VPetLCD/VPetLCD.h"
#include "VPetLCD/VPetLCDMenuBar32p.h"
#include "VPetLCD/Screens/AgeWeightScreen.h"
#include "VPetLCD/Screens/DigimonNameScreen.h"
#include "VPetLCD/Screens/HeartsScreen.h"
#include "VPetLCD/Screens/ProgressBarScreen.h"
#include "VPetLCD/Screens/PercentageScreen.h"
#include "VPetLCD/Screens/SelectionScreen.h"
#include "VPetLCD/Screens/ClockScreen.h"
#include "VPetLCD/Screens/DigimonWatchingScreen.h"
#include "VPetLCD/Screens/AnimationScreens/EatingAnimationScreen.h"
#include "VPetLCD/Screens/TrainingScreen.h"


#include "GameLogic/ScreenStateMachine.h"

#include "GameLogic/Digimon.h"


#include "config.h"

uint16_t digiIndex =DIGIMON_BOTAMON;
Digimon digimon(digiIndex);

//ESP32 Specific stuff
#include "VPetLCD/DisplayAdapter/TFT_eSPI_Displayadapter.h"
#include "VPetLCD/ESP32SpriteManager.h"
#include "GameLogic/ESP32DigimonDataLoader.h"

#include <TFT_eSPI.h>
#include "Button2.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "time.h"


#define ADC_EN 14 //ADC_EN is the ADC detection enable port
#define ADC_PIN 34
#define BUTTON_1 35
#define BUTTON_2 0

Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

int hours = 23;
int minutes = 59;
int seconds = 0;

boolean buttonPressed = false;

int displayHeight = 240;
int displayWidth = 135;


//TFT_eSPI-Only stuff
TFT_eSPI tft = TFT_eSPI(displayWidth, displayHeight); // Create object "tft"
TFT_eSprite img = TFT_eSprite(&tft);                  // Create Sprite object "img" with pointer to "tft" object
TFT_eSPI_DisplayAdapter displayAdapter(&img, displayHeight, displayWidth);         //create a DisplayAdapter for VPetLCD class
//--------

//ESP32 Only stuff
ESP32SpriteManager spriteManager;
ESP32DigimonDataLoader dataLoader;
//

//Creating all instances for the UI
VPetLCD screen(&displayAdapter, &spriteManager, 40, 16);
VPetLCDMenuBar32p menuBar(9, 5, displayHeight);

V20::DigimonWatchingScreen digimonScreen(&spriteManager, digimon.getDigimonIndex(), digimon, 0, 35, 0, 0);
V20::DigimonNameScreen digiNameScreen(&spriteManager, dataLoader.getDigimonProperties(digiIndex)->digiName, digimon.getDigimonIndex(), 24);
// V20::AgeWeightScreen ageWeightScreen(5, 21);
V20::AgeWeightScreen ageWeightScreen(digimon.getAge(), digimon.getWeight());
// V20::HeartsScreen hungryScreen("Hungry", 2, 4);
V20::HeartsScreen hungryScreen("Hungry", digimon.getHungerHeartsCount(), 4);
// V20::HeartsScreen strengthScreen("Strength", 3, 4);
V20::HeartsScreen strengthScreen("Strength", digimon.getStrengthHeartsCount(), 4);
// V20::HeartsScreen effortScreen("Effort", 4, 4);
V20::HeartsScreen effortScreen("Effort", digimon.getEffortHeartCount(), 4);
V20::ProgressBarScreen dpScreen("DP", 29, 40);
V20::PercentageScreen sPercentageScreen("WIN", 'S', 100);
V20::PercentageScreen tPercentageScreen("WIN", 'T', 93);
V20::SelectionScreen foodSelection(true);
V20::SelectionScreen fightSelection(true);
V20::ClockScreen clockScreen(true);
V20::EatingAnimationScreen eatingAnimationScreen(&spriteManager, digimon.getDigimonIndex());
V20::SelectionScreen testSelection(true);
V20::SelectionScreen settingsSelection(true);
V20::TrainingScreen trainingScreen(digimon, &spriteManager);

//15 screens and 3 signals (one for each button)
uint8_t numberOfScreens = 16;
uint8_t numberOfSignals = 3;

uint8_t confirmSignal = 0;
uint8_t nextSignal = 1;
uint8_t backSignal = 2;

//Creating the ScreenStateMachine, which handles transitions between screens
//and the actions of the buttons; the buttons are just sending signals to the statemachine
ScreenStateMachine stateMachine(numberOfScreens, numberOfSignals);

uint8_t digimonScreenId = stateMachine.addScreen(&digimonScreen);
uint8_t digiNameScreenId = stateMachine.addScreen(&digiNameScreen);
uint8_t ageWeightScreenId = stateMachine.addScreen(&ageWeightScreen);
uint8_t hungryScreenId = stateMachine.addScreen(&hungryScreen);
uint8_t strengthScreenId = stateMachine.addScreen(&strengthScreen);
uint8_t effortScreenId = stateMachine.addScreen(&effortScreen);
uint8_t dpScreenId = stateMachine.addScreen(&dpScreen);
uint8_t sPercentageScreenId = stateMachine.addScreen(&sPercentageScreen);
uint8_t tPercentageScreenId = stateMachine.addScreen(&tPercentageScreen);
uint8_t foodSelectionId = stateMachine.addScreen(&foodSelection);
uint8_t fightSelectionId = stateMachine.addScreen(&fightSelection);
uint8_t clockScreenId = stateMachine.addScreen(&clockScreen);
uint8_t eatingAnimationScreenId = stateMachine.addScreen(&eatingAnimationScreen);
uint8_t testSelectionId = stateMachine.addScreen(&testSelection);
uint8_t settingsSelectionId = stateMachine.addScreen(&settingsSelection);
uint8_t trainingScreenId = stateMachine.addScreen(&trainingScreen);

uint8_t poop=0;

uint8_t previousScreenId;

unsigned long lastInteractionTime = 0; // Track the last time a button was pressed
bool screenOn = true; // Track the screen state

int brightnessLevel = 3; // Start at a default brightness level
const int maxBrightnessLevel = 5;

// Forward declaration of the turnOnScreen function
void turnOnScreen();

void syncTimeWithFeedback();
void toggleBrightness();

void initWiFiAndSyncTimezone() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");

  // Explicitly set timezone to UTC
  setenv("TZ", "UTC0", 1);
  tzset();

  configTime(0, 0, ntpServer); // Sync time in UTC explicitly
  Serial.println("Time synchronized via NTP in UTC");
}

void stateMachineInit() {
  const DigimonProperties *properties = dataLoader.getDigimonProperties(digimon.getDigimonIndex());
  digimon.setProperties(properties);

  //return to food selection screen after showing eating animation
  eatingAnimationScreen.setAnimationEndAction([]() {
    stateMachine.setCurrentScreen(foodSelectionId);
  });

  // in order to be able to go back to the digimon watching screen
  // we will add a transition from every screen to the digimon watching screen
  //triggered by the backsignal (backbutton)
  for (int i = 1; i <= numberOfScreens + 1;i++) {
    stateMachine.addTransition(i, digimonScreenId, backSignal);
  }

  //The Scale Menu transitions
  stateMachine.addTransition(digiNameScreenId, ageWeightScreenId, nextSignal);
  stateMachine.addTransition(ageWeightScreenId, hungryScreenId, nextSignal);
  stateMachine.addTransition(hungryScreenId, strengthScreenId, nextSignal);
  stateMachine.addTransition(strengthScreenId, effortScreenId, nextSignal);
  stateMachine.addTransition(effortScreenId, dpScreenId, nextSignal);
  stateMachine.addTransition(dpScreenId, sPercentageScreenId, nextSignal);
  stateMachine.addTransition(sPercentageScreenId, tPercentageScreenId, nextSignal);
  stateMachine.addTransition(tPercentageScreenId, digiNameScreenId, nextSignal);

  //Transitions between clock screen and digimon watching screen
  stateMachine.addTransition(digimonScreenId, clockScreenId, backSignal);
  stateMachine.addTransition(clockScreenId, digimonScreenId, backSignal);

  //Conditional transtitions from digimonScreen to the others (menuselection)
  //this must be set, because unset transitions wont trigger transitionActions
  stateMachine.addTransition(digimonScreenId, digimonScreenId, nextSignal);

  //if nextSignal is sent (nextbutton pressed), the menuselection will be
  //incremented and the selection will be set
  stateMachine.addTransitionAction(digimonScreenId, nextSignal, []() {
      menuBar.nextSelection();
    });

  //Here are the conditional transitions handled.
  stateMachine.addTransition(digimonScreenId, digimonScreenId, confirmSignal);
  stateMachine.addTransitionAction(digimonScreenId, confirmSignal, []() {
    uint8_t maxdp = digimon.getProperties()->maxDigimonPower;
    switch (menuBar.getSelection()) {
    case 0:
      digiNameScreen.setDigimonSpriteIndex(digimon.getDigimonIndex());
      // hungryScreen.setHearts(4-4*digimon.getHunger()/10);
      hungryScreen.setHearts(digimon.getHungerHeartsCount());
      strengthScreen.setHearts(digimon.getStrengthHeartsCount());
      if(maxdp > 0){
        dpScreen.setFillPercentage((digimon.getDigimonPower()*100)/maxdp);
      } else {
        dpScreen.setFillPercentage(0);
      }
      ageWeightScreen.setAge(digimon.getAge());
      ageWeightScreen.setWeight(digimon.getWeight());
      stateMachine.setCurrentScreen(digiNameScreenId);
      break;
    case 1:
      foodSelection.setSelection(0);
      stateMachine.setCurrentScreen(foodSelectionId);
      break;
    case 2:
      if (digimon.getProperties()->stage != STAGE_BABY1 && digimon.getProperties()->stage != STAGE_BABY2) {
        stateMachine.setCurrentScreen(trainingScreenId);
        trainingScreen.onEnter();
      } else {
        Serial.println("Cannot navigate to TrainingScreen: Digimon stage is too low.");
      }
      break;
    case 3:
      fightSelection.setSelection(0);
      stateMachine.setCurrentScreen(fightSelectionId);
      break;
    case 4:
      digimonScreen.flushPoop();
      digimon.setNumberOfPoops(0);
      break;
    case 7: // Testing menu
      testSelection.setSelection(0);
      stateMachine.setCurrentScreen(testSelectionId);
      break;
    case 8: // Settings menu
      settingsSelection.setSelection(0);
      stateMachine.setCurrentScreen(settingsSelectionId);
      break;
    }
  });


  //adding functionality of buttons in food screen:
  stateMachine.addTransition(foodSelectionId, foodSelectionId, nextSignal);
  stateMachine.addTransitionAction(foodSelectionId, nextSignal, []() {
    foodSelection.nextSelection();
    });

  //adding functionality of buttons in food screen:
  stateMachine.addTransition(foodSelectionId, foodSelectionId, confirmSignal);
  stateMachine.addTransitionAction(foodSelectionId, confirmSignal, []() {
    uint8_t selection = foodSelection.getSelection();
    switch (selection) {
    case 0: // Meat
      if (digimon.getState() == 1) {
        digimon.setState(0);
        Serial.println("State is now 0");
        digimon.addSleepDisturbance(1);
      }
      if (digimon.getAppetite() < 14) {
        digimon.addWeight(1);
        digimon.addAppetite(1);
        digimon.setHungerCallCheck(false);

        if (digimon.getAppetite() < 0) {
          digimon.setHungerHeartsCount(0);
        } else if (digimon.getAppetite() > 4) {
          digimon.setHungerHeartsCount(4);
        } else if (digimon.getAppetite() == 14) {
          digimon.setOverfeedCheck(true);
          digimon.addOverfeed(1);
        } else {
          digimon.setHungerHeartsCount(digimon.getAppetite());
        }

        if ((hours >= digimon.getProperties()->sleepHour) || (hours >= 0 && hours < 8)) {
          digimon.setCanReturnToSleepCheck(true);
        }

      eatingAnimationScreen.setSprites(SYMBOL_MEAT, SYMBOL_HALF_MEAT,SYMBOL_EMPTY_MEAT);

      // Set animation end action to return to food selection
      eatingAnimationScreen.setAnimationEndAction([]() {
        stateMachine.setCurrentScreen(foodSelectionId);
      });

      previousScreenId = foodSelectionId;
      eatingAnimationScreen.startAnimation();
      stateMachine.setCurrentScreen(eatingAnimationScreenId);
      } else {
        // Play refusal animation
      }
      break;

    case 1: // Pill
      if (digimon.getState() == 1) {
        digimon.setState(0);
        digimon.addSleepDisturbance(1);
      }
      digimon.addWeight(2);
      digimon.addStrength(1);
      digimon.increaseOverdoseTracker(1);
      digimon.setStrengthCallCheck(false);

      // Increase heart count
      if (digimon.getStrength() < 0) {
        digimon.setStrengthHeartsCount(0);
      } else if (digimon.getStrength() > 4) {
        digimon.setStrengthHeartsCount(4);
      } else {
        digimon.setStrengthHeartsCount(digimon.getStrength());
      }

      // Increase DP for every 4 protein eaten
      if (digimon.getOverdoseTracker() == 4) {
        digimon.addDigimonPower(1);
      }


      // Increase overdose count for every 4 protein eated, for a maximum of 7
      // Also resets OD tracker to zero.
      if (digimon.getOverdoseTracker() == 4 && digimon.getOverdoseCount() < 7) {
        digimon.increaseOverdoseCount(1);
        digimon.setOverdoseTracker(0);
      } else if (digimon.getOverdoseTracker() == 4 && digimon.getOverdoseCount() == 7) {
        digimon.setOverdoseTracker(0);
      }

      if ((hours >= digimon.getProperties()->sleepHour) || (hours >= 0 && hours < 8)) {
        digimon.setCanReturnToSleepCheck(true);
      }

      eatingAnimationScreen.setSprites(SYMBOL_PILL, SYMBOL_HALF_PILL, SYMBOL_EMPTY);
      
      // Set animation end action to return to food selection
      eatingAnimationScreen.setAnimationEndAction([]() {
        stateMachine.setCurrentScreen(foodSelectionId);
      });

      previousScreenId = foodSelectionId;
      eatingAnimationScreen.startAnimation();
      stateMachine.setCurrentScreen(eatingAnimationScreenId);
      break;
    }
    });

    //go back to food selection if pressed confirm again
    stateMachine.addTransition(eatingAnimationScreenId, foodSelectionId, confirmSignal);
    //abort animation
    stateMachine.addTransitionAction(eatingAnimationScreenId, confirmSignal, [](){
      eatingAnimationScreen.abortAnimation();
    });

  //adding functionality of buttons in fight screen:
  stateMachine.addTransition(fightSelectionId, fightSelectionId, nextSignal);
  stateMachine.addTransitionAction(fightSelectionId, nextSignal, []() {
    fightSelection.nextSelection();

    });

  // Adding functionality of buttons in testing screen:
  stateMachine.addTransition(testSelectionId, testSelectionId, nextSignal);
  stateMachine.addTransitionAction(testSelectionId, nextSignal, []() {
    testSelection.nextSelection();
  });

  stateMachine.addTransition(testSelectionId, testSelectionId, confirmSignal);
  stateMachine.addTransitionAction(testSelectionId, confirmSignal, []() {
    uint8_t selection = testSelection.getSelection();
    switch (selection) {
      case 0: // Sleep
        digimon.setState(1);
        eatingAnimationScreen.setSprites(SYMBOL_HEARTEMPTY, SYMBOL_EMPTY, SYMBOL_EMPTY);
        
        // Set animation end action to return to testing menu
        eatingAnimationScreen.setAnimationEndAction([]() {
          stateMachine.setCurrentScreen(testSelectionId);
        });

        previousScreenId = testSelectionId;
        eatingAnimationScreen.startAnimation();
        stateMachine.setCurrentScreen(eatingAnimationScreenId);
        break;

      case 1: // Wake
        digimon.setState(0);
        eatingAnimationScreen.setSprites(SYMBOL_HEART, SYMBOL_EMPTY, SYMBOL_EMPTY);
        
        // Set animation end action to return to testing menu
        eatingAnimationScreen.setAnimationEndAction([]() {
          stateMachine.setCurrentScreen(testSelectionId);
        });

        previousScreenId = testSelectionId;
        eatingAnimationScreen.startAnimation();
        stateMachine.setCurrentScreen(eatingAnimationScreenId);
        break;

      case 2: // Evo (increase digimon index)
        if (digimon.getDigimonIndex() < N_DIGIMON - 1) {
          digimon.setDigimonIndex(digimon.getDigimonIndex() + 1);
          const DigimonProperties *newProperties = dataLoader.getDigimonProperties(digimon.getDigimonIndex());
          digimon.setProperties(newProperties);
          digimonScreen.setDigimonSpritesIndex(digimon.getDigimonIndex());
          digiNameScreen.setDigimonSpriteIndex(digimon.getDigimonIndex());
          digiNameScreen.setDigimonName(newProperties->digiName);
          eatingAnimationScreen.setDigimonSpriteIndex(digimon.getDigimonIndex());
        }
        eatingAnimationScreen.setSprites(SYMBOL_HEART, SYMBOL_EMPTY, SYMBOL_EMPTY);
        
        // Set animation end action to return to testing menu
        eatingAnimationScreen.setAnimationEndAction([]() {
          stateMachine.setCurrentScreen(testSelectionId);
        });

        previousScreenId = testSelectionId;
        eatingAnimationScreen.startAnimation();
        stateMachine.setCurrentScreen(eatingAnimationScreenId);
        break;

      case 3: // Devo (decrease digimon index)
        if (digimon.getDigimonIndex() > 0) {
          digimon.setDigimonIndex(digimon.getDigimonIndex() - 1);
          const DigimonProperties *newProperties = dataLoader.getDigimonProperties(digimon.getDigimonIndex());
          digimon.setProperties(newProperties);
          digimonScreen.setDigimonSpritesIndex(digimon.getDigimonIndex());
          digiNameScreen.setDigimonSpriteIndex(digimon.getDigimonIndex());
          digiNameScreen.setDigimonName(newProperties->digiName);
          eatingAnimationScreen.setDigimonSpriteIndex(digimon.getDigimonIndex());
        }
        eatingAnimationScreen.setSprites(SYMBOL_POOP, SYMBOL_EMPTY, SYMBOL_EMPTY);
        
        // Set animation end action to return to testing menu
        eatingAnimationScreen.setAnimationEndAction([]() {
          stateMachine.setCurrentScreen(testSelectionId);
        });

        previousScreenId = testSelectionId;
        eatingAnimationScreen.startAnimation();
        stateMachine.setCurrentScreen(eatingAnimationScreenId);
        break;
    }
  });

  // Go back to testing selection if confirm pressed again during animation
  stateMachine.addTransition(eatingAnimationScreenId, testSelectionId, confirmSignal);
  stateMachine.addTransitionAction(eatingAnimationScreenId, confirmSignal, [](){
    eatingAnimationScreen.abortAnimation();
  });

  stateMachine.addTransition(eatingAnimationScreenId, eatingAnimationScreenId, confirmSignal);
  stateMachine.addTransitionAction(eatingAnimationScreenId, confirmSignal, [](){
    eatingAnimationScreen.abortAnimation();
    stateMachine.setCurrentScreen(previousScreenId);
  });

  // New case for settings menu
  stateMachine.addTransition(settingsSelectionId, settingsSelectionId, confirmSignal);
  stateMachine.addTransition(settingsSelectionId, settingsSelectionId, nextSignal);
  stateMachine.addTransitionAction(settingsSelectionId, nextSignal, []() {
    settingsSelection.nextSelection();
  });

  
  stateMachine.addTransition(trainingScreenId, trainingScreenId, confirmSignal);
  stateMachine.addTransitionAction(trainingScreenId, confirmSignal, []() {
    trainingScreen.handleConfirmSignal(&screen);
  });


  trainingScreen.setAnimationEndAction([]() {
    stateMachine.setCurrentScreen(digimonScreenId);
  });

  trainingScreen.setNotifyMainCallback([]() {
    effortScreen.setHearts(digimon.getEffortHeartCount());
  });
}

void syncTimeWithFeedback() {
  // Display connecting message
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, displayHeight / 2 - 20);
  tft.print("Connecting WiFi...");

  WiFi.begin(ssid, password);
  int maxRetries = 20; // 10 seconds timeout
  int retries = 0;

  while (WiFi.status() != WL_CONNECTED && retries < maxRetries) {
    delay(500);
    if (btn2.isPressed()) {
      Serial.println("WiFi connection skipped by user.");
      break;
    }
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, displayHeight / 2 - 20);
    tft.print("Syncing time...");
    setenv("TZ", "UTC0", 1);
    tzset();
    configTime(0, 0, ntpServer);
    delay(2000); // Short delay to show message clearly
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, displayHeight / 2 - 20);
    tft.print("WiFi failed!");
    delay(1500);
  }

  // Return to settings menu
  stateMachine.setCurrentScreen(settingsSelectionId);
}

void button_init()
{
  btn1.setLongClickHandler([](Button2& b) {
    if (!screenOn) {
      tft.writecommand(TFT_DISPON); // Turn the screen back on
      tft.fillScreen(0x86CE); // Refresh the screen content
      analogWrite(4, map(brightnessLevel, 2, maxBrightnessLevel, 51, 255)); // Restore user brightness
      screenOn = true;
      lastInteractionTime = millis(); // Reset the inactivity timer
    } else {
      stateMachine.sendSignal(backSignal);
      buttonPressed = true;
      lastInteractionTime = millis(); // Reset the inactivity timer
    }
  });

  btn1.setPressedHandler([](Button2& b) {
    if (!screenOn) {
      tft.writecommand(TFT_DISPON); // Turn the screen back on
      tft.fillScreen(0x86CE); // Refresh the screen content
      analogWrite(4, map(brightnessLevel, 2, maxBrightnessLevel, 51, 255)); // Restore user brightness
      screenOn = true;
      lastInteractionTime = millis(); // Reset the inactivity timer
    } else {
      stateMachine.sendSignal(nextSignal);
      buttonPressed = true;
      lastInteractionTime = millis(); // Reset the inactivity timer
    }
  });

  btn2.setPressedHandler([](Button2& b) {
    if (!screenOn) {
      tft.writecommand(TFT_DISPON); // Turn the screen back on
      tft.fillScreen(0x86CE); // Refresh the screen content
      analogWrite(4, map(brightnessLevel, 2, maxBrightnessLevel, 51, 255)); // Restore user brightness
      screenOn = true;
      lastInteractionTime = millis(); // Reset the inactivity timer
    } else {
      stateMachine.sendSignal(confirmSignal);
      buttonPressed = true;
      lastInteractionTime = millis(); // Reset the inactivity timer
    }
  });
}

void toggleBrightness() {
  brightnessLevel++;
  if (brightnessLevel > maxBrightnessLevel) {
    brightnessLevel = 2; // Start from 2 to avoid turning off the screen
  }

  // Adjust the actual brightness of the backlight here
  // For example, using PWM to control brightness
  analogWrite(4, map(brightnessLevel, 2, maxBrightnessLevel, 51, 255)); // Map from 51 to avoid turning off
}

void setupScreens()
{
  menuBar.setIconOnIndex(0,0);
  menuBar.setIconOnIndex(1,1);
  menuBar.setIconOnIndex(2,2);
  menuBar.setIconOnIndex(3,3);
  menuBar.setIconOnIndex(4,4);
  menuBar.setIconOnIndex(5,5);
  menuBar.setIconOnIndex(6,6);
  menuBar.setIconOnIndex(7,7); // Testing menu
  menuBar.setIconOnIndex(8, 8); // Settings menu (wrench icon)


  screen.setMenuBar(&menuBar);
  screen.setLCDPos(0, 32);
  screen.setLcdScale(6);

  //Positioning of the screens
  int screensOffsetX = 4;


  //set offset of the screens
  ageWeightScreen.setPos(screensOffsetX, 0);
  effortScreen.setPos(screensOffsetX, 0);
  strengthScreen.setPos(screensOffsetX, 0);
  hungryScreen.setPos(screensOffsetX, 0);
  dpScreen.setPos(screensOffsetX, 0);
  sPercentageScreen.setPos(screensOffsetX, 0);
  tPercentageScreen.setPos(screensOffsetX, 0);
  clockScreen.setPos(screensOffsetX, 0);
  eatingAnimationScreen.setPos(screensOffsetX, 0);
  


  //adding the food selection options
  foodSelection.addOption("Meat", SYMBOL_MEAT);
  foodSelection.addOption("PILL", SYMBOL_PILL);

  //adding the battle options
  fightSelection.setShowIcons(false);
  fightSelection.addOption("SINGLE");
  fightSelection.addOption("TAG");

  clockScreen.setHours(hours);
  clockScreen.setMinutes(minutes);
  clockScreen.setSeconds(seconds);

  // Adding the testing selection options
  testSelection.addOption("SLP", SYMBOL_HEARTEMPTY);
  testSelection.addOption("WAKE", SYMBOL_HEART);
  testSelection.addOption("Evo", SYMBOL_HEART);
  testSelection.addOption("Devo", SYMBOL_POOP);

  // Adding the settings selection options
  settingsSelection.addOption("Time", SYMBOL_HEART);
  settingsSelection.addOption("BRIGHT", SYMBOL_HEART);

  stateMachine.addTransitionAction(settingsSelectionId, confirmSignal, []() {
    uint8_t selection = settingsSelection.getSelection();
    switch (selection) {
      case 0: // Time
        syncTimeWithFeedback();
        break;
      case 1: // Brightness
        toggleBrightness();
        break;
    }
  });
}

// =========================================================================
void setup(void)
{
  Serial.begin(115200);
  Serial.println("Start");

  pinMode(ADC_EN, OUTPUT);
  digitalWrite(ADC_EN, HIGH);

  randomSeed(analogRead(1));
  /*
  digimon.setDigimonIndex(10);
    digimon.setState(20);
    digimon.setAge(30);
    digimon.setWeight(40);
    digimon.setFeedCounter(50);
    digimon.setCareMistakes(60);
    digimon.setTrainingCounter(70);
    digimon.setTimeUntilEvolution(80);
    digimon.setPoopTimer(90);
    digimon.setAgeTimer(100);
    digimon.setEvolutionTimer(110);
    Serial.println(digimon.getDigimonIndex());
  Serial.println(digimon.getState());
  Serial.println(digimon.getAge());
  Serial.println(digimon.getWeight());
  Serial.println(digimon.getFeedCounter());
  Serial.println(digimon.getCareMistakes());
  Serial.println(digimon.getTrainingCounter());
  Serial.println(digimon.getTimeUntilEvolution());
  Serial.println(digimon.getPoopTimer());
  Serial.println(digimon.getAgeTimer());
  Serial.println(digimon.getEvolutionTimer());
  savegame.saveDigimon(&digimon);
  */

 //savegame.loadDigimon(&digimon);

  // TFT initialization
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(0x86CE); // Ensure the screen is filled with a color at startup

  // Display loading messages
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, displayHeight / 2 - 40);
  tft.print("Connecting WiFi...");
  tft.setCursor(10, displayHeight / 2 - 20);
  tft.print("Syncing time...");

  // Initialize buttons and screens
  Serial.println("button_init");
  button_init();
  Serial.println("setupScreens");
  setupScreens();

  stateMachineInit();
  Serial.println("stateMachineInit");

  // Connect WiFi and sync time
  initWiFiAndSyncTimezone();

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH); // Turn on the backlight initially

  digimon.setTurnOnScreenCallback(turnOnScreen);
}
// =========================================================================

unsigned long ticker = 0;
unsigned long tickerResetValue = 1000;
unsigned long lastDelta = 0;
float getFragmentation() ;
boolean debug=false;

void loop()
{
  unsigned long currentTime = millis();

  // Check if 30 seconds have passed since the last interaction
  if (screenOn && (currentTime - lastInteractionTime >= 30000)) {
    stateMachine.setCurrentScreen(digimonScreenId); // Set to Digimon Watching Screen
    tft.writecommand(TFT_DISPOFF); // Turn off the display
    analogWrite(4, 0); // Set brightness to lowest setting for sleep mode
    screenOn = false;
  }

  // Always run button loops to detect button presses
  btn1.loop();
  btn2.loop();

  // Always run digimon logic to update timers and states
  unsigned long t1 = millis();
  digimon.loop(lastDelta);

  if (digimon.isEvolved()) {
    const DigimonProperties *newProperties = dataLoader.getDigimonProperties(digimon.getDigimonIndex());
    digimon.setProperties(newProperties);

    // Update screens after evolution
    digimonScreen.setDigimonSpritesIndex(digimon.getDigimonIndex());
    digiNameScreen.setDigimonSpriteIndex(digimon.getDigimonIndex());
    digiNameScreen.setDigimonName(newProperties->digiName);
    eatingAnimationScreen.setDigimonSpriteIndex(digimon.getDigimonIndex());

    // Reset evolved flag to allow further evolutions
    digimon.setEvolved(false);
    Serial.println("Evolved flag reset");
  }

  // Update the screen's internal state regardless of whether the display is on
  digimonScreen.setNumberOfPoop(digimon.getNumberOfPoops());
  digimonScreen.setDigimonState(digimon.getState());
  digimonScreen.setDigimonSpritesIndex(digimon.getDigimonIndex());

  // Updating the screens which need the loop
  digimonScreen.loop(lastDelta);
  clockScreen.loop(lastDelta);
  digiNameScreen.loop(lastDelta);

  // Switch to next frame only when the screen is active
  if (stateMachine.getCurrentScreen() == &eatingAnimationScreen)
    eatingAnimationScreen.loop(lastDelta);

  // Update clock every second
  static unsigned long lastClockUpdate = 0;
  static int lastCheckedHour = -1; // Track last hour checked to avoid repeated state changes

  if (millis() - lastClockUpdate >= 1000) {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      int adjustedHour = (timeinfo.tm_hour + timezoneOffsetHours) % 24;
      clockScreen.setHours(adjustedHour);
      clockScreen.setMinutes(timeinfo.tm_min);
      clockScreen.setSeconds(timeinfo.tm_sec);

      // Check if it's time to sleep
      if (adjustedHour == digimon.getProperties()->sleepHour && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0 && lastCheckedHour != adjustedHour) {
        digimon.setState(1); // Set state to sleep
        lastCheckedHour = adjustedHour;
      }

      // Check if it's time to wake up (8:00 am)
      if (adjustedHour == 8 && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0 && lastCheckedHour != adjustedHour) {
        digimon.setState(0); // Set state to awake
        lastCheckedHour = adjustedHour;
      }

      // Reset lastCheckedHour after the minute passes to allow next day's check
      if (timeinfo.tm_min != 0 || timeinfo.tm_sec != 0) {
        lastCheckedHour = -1;
      }
    }
    lastClockUpdate = millis();
  }

  // Only render the screen if it's on
  if (screenOn) {
    digitalWrite(4, HIGH); // Ensure the backlight is on
    screen.renderScreen(stateMachine.getCurrentScreen());
  }

  buttonPressed = false;

  if (debug == true) {
    // Here should be debug stuff but it's only fps lol
    tft.setTextColor(TFT_BLACK);
    tft.fillRect(0, 0, 100, 20, 0xFFFF);
    tft.drawString(String((1000.0) / lastDelta) + " FPS", 0, 0);
    tft.drawString("Fragmentation: " + String(getFragmentation()), 0, 10);
  }

  unsigned long t2 = millis();
  lastDelta = t2 - t1;

  if (stateMachine.getCurrentScreen() == &trainingScreen) {
    trainingScreen.loop(&screen, lastDelta);
  }
}




//measure the HeapFragmentation
float getFragmentation() {
  
  return 100 - heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) * 100.0 / heap_caps_get_free_size(MALLOC_CAP_8BIT);
}

void turnOnScreen() {
  if (!screenOn) {
    tft.writecommand(TFT_DISPON); // Turn the screen back on
    analogWrite(4, map(brightnessLevel, 2, maxBrightnessLevel, 51, 255)); // Restore user brightness
    screenOn = true;
    lastInteractionTime = millis(); // Reset the inactivity timer
  }
}