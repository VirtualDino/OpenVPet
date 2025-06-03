#include "TrainingScreen.h"
#include "../../GameLogic/ScreenStateMachine.h"
#include "../ImageData/TrainingMeter.h"
#include "../ImageData/LargeIcons.h"

namespace V20 {

namespace {


  const unsigned long CYCLE_INTERVAL_ROOKIE = 250;
  const unsigned long CYCLE_INTERVAL_ADULT = 150;
  const unsigned long CYCLE_INTERVAL_PERFECT = 100;
  const unsigned long CYCLE_INTERVAL_ULTIMATE = 50;
  const unsigned long CYCLE_INTERVAL_SUPER_ULTIMATE = 50;
}

extern ScreenStateMachine stateMachine;
extern uint8_t digimonScreenId;

void TrainingScreen::draw(VPetLCD *lcd) {
  uint16_t pixelColor = 0xFFFF; // Assuming a default color value
  if (transitionToSprite) {
    if (millis() - lastUpdateTime < 5000) { // Length of the training animation
      displayTrainingAnimation(lcd, SPRITE_DIGIMON_ATTACK_1);
    } else if (millis() - lastUpdateTime < 8000) { //buffer for victory animation
      displayResultAnimation(lcd);
    }
  } else {
    displayMeter(lcd);
  }
}

void TrainingScreen::displayMeter(VPetLCD *lcd) {
  int x = 0;
  int y = 0;
  int meterIndex = currentMeterIndex;
  int textX = 8;

  // Draw 'POWER' using drawLetterOnLCD
  const char* power = "POWER";
  for (int i = 0; power[i] != '\0'; ++i) {
    lcd->drawLetterOnLCD(power[i], textX, y, pixelColor);
    textX += CAPITAL_LETTER_WIDTH[power[i] - 'A'] + 1; // Add 1 for spacing
  }

  // Draw the training meter sprite below 'POWER'
  y += SPRITES_UPPERCASE_ALPHABET_HEIGHT + 1; // Move y position below the text
  lcd->drawMeter(TRAINING_METER[meterIndex], 31, SPRITES_METER_RESOLUTION, 5, y, false, pixelColor);
}

void TrainingScreen::loop(VPetLCD *lcd, unsigned long delta) {
  unsigned long cycleInterval = CYCLE_INTERVAL_ROOKIE; // Default to ROOKIE interval
  int stage = digimon.getProperties()->stage;

  // Assign cycle interval based on stage
  switch (stage) {
    case STAGE_ULTIMATE:
    case STAGE_SUPER_ULTIMATE:
      cycleInterval = CYCLE_INTERVAL_SUPER_ULTIMATE;
      break;
    case STAGE_ADULT:
      cycleInterval = CYCLE_INTERVAL_ADULT;
      break;
    case STAGE_PERFECT:
      cycleInterval = CYCLE_INTERVAL_PERFECT;
      break;
    // Default case already set to ROOKIE
  }

  if (cycling && millis() >= lastUpdateTime && millis() - lastUpdateTime >= cycleInterval) {
    nextSelection();
    lastUpdateTime = millis();
  }

  // Transition to Training animation
  if (transitionToSprite) {
    if (millis() - lastUpdateTime < 5000) { // Length of the training animation
      displayTrainingAnimation(lcd, SPRITE_DIGIMON_ATTACK_1);
    } else if (millis() - lastUpdateTime < 8000) { // 3 seconds for victory animation
      displayResultAnimation(lcd);
    } else {
      // Exit the training screen and return to DigimonWatchingScreen
      onExit();
      animationEndAction();
    }
  }
}

void TrainingScreen::handleConfirmSignal(VPetLCD *lcd) {
  if (cycling) {
    cycling = false;
    trainingScore = getCurrentSelection();

    // Update Digimon's stats
    digimon.addTrainingSession(1);
    digimon.addEffort(1);
    if (digimon.getWeight() > digimon.getMinWeight()) {
      digimon.loseWeight(1);
    }
    if (trainingScore == 14) {
      digimon.addStrength(1);
      digimon.setStrengthCallCheck(false);
    }

    // Update effort heart count based on the new effort value
    switch (digimon.getEffort()) {
        case 4:
            digimon.setEffortHeartCount(1);
            break;
        case 8:
            digimon.setEffortHeartCount(2);
            break;
        case 12:
            digimon.setEffortHeartCount(3);
            break;
        case 16:
            digimon.setEffortHeartCount(4);
            break;
    }

    // Set a timer to transition to the Digimon sprite
    lastUpdateTime = millis();
    transitionToSprite = true;

    // Update the effort screen to reflect the new heart count
    // effortScreen.setHearts(digimon.getEffortHeartCount());

    // Call the notifyMainCallback if it's set
    if (notifyMainCallback) {
        notifyMainCallback();
    }
  }
}

void TrainingScreen::nextSelection() {
  currentMeterIndex = (currentMeterIndex + 1) % 15;
}

int TrainingScreen::getCurrentSelection() const {
  return meter[currentMeterIndex];
}

void TrainingScreen::onEnter() {
  cycling = true;
  currentMeterIndex = 0; // Start cycling at 0
  transitionToSprite = false;
  lastUpdateTime = millis() + 100; // Add a buffer before cycling starts
}

void TrainingScreen::onExit() {
  cycling = false;
  currentMeterIndex = 0; // Clear the selected meter index
}

void TrainingScreen::displayTrainingAnimation(VPetLCD *lcd, int index) {
  const unsigned short* sprite = spriteManager->getDigimonSprite(digimon.getDigimonIndex(), index);
  const uint16_t* wallIcon = spriteManager->getLargeIcon(ICON_WALL);
  const uint16_t* wallIconBroken = spriteManager->getLargeIcon(ICON_WALL_BROKEN);
  int spriteX = 28; // Initial position of the Digimon sprite
  int symbolX = spriteX - SPRITES_SYMBOL_RESOLUTION - 2; // Initial position of the attack symbol
  int symbolY = screenY + 4; // 4 pixels down

  // Calculate the frame based on time
  unsigned long currentTime = millis();
  int frame = (currentTime - lastUpdateTime) / 500; // 500 ms per frame

  // Move the symbol to the left each frame
  if (frame < 4) {
    symbolX -= frame * SPRITES_SYMBOL_RESOLUTION;
    lcd->drawLargeIcon(wallIcon, 0, screenY, false, pixelColor); // Include wallIcon in frames 1-4
  } else if (frame < 8) {
    // Increase the speed of the impact frames
    int impactFrame = ((currentTime - lastUpdateTime) / 125) % 2; // 125 ms per impact frame
    lcd->drawLargeElement(impactFrame == 0 ? ELEMENT_IMPACT_0 : ELEMENT_IMPACT_1, 4, screenY, false, pixelColor);
    // Do not display the Digimon sprite or attacksymbol during impact frames
    spriteX = -SPRITES_SYMBOL_RESOLUTION; // Hide Digimon sprite
    symbolX = -SPRITES_SYMBOL_RESOLUTION; // Hide symbol
  } else if (frame < 10) { // Add 2 more frames to show only the result
    // Display wallIconBroken or wallIcon based on trainingScore
    const uint16_t* iconToShow = (trainingScore == 14) ? wallIconBroken : wallIcon;
    lcd->drawLargeIcon(iconToShow, 0, screenY, false, pixelColor);
    symbolX = -SPRITES_SYMBOL_RESOLUTION; // Hide symbol
  } else {
    // Hide the symbol after the last frame
    symbolX = -SPRITES_SYMBOL_RESOLUTION;
  }

  // Draw the Digimon sprite if it's visible
  if (spriteX >= 0) {
    lcd->draw16BitArray(sprite, spriteX, screenY, false, pixelColor);
  }

  // Display the symbol if it's visible
  if (symbolX >= 0) {
    lcd->drawSymbol(digimon.getProperties()->attackIcon, symbolX, symbolY, false, pixelColor);
  }
}

void TrainingScreen::displayResultAnimation(VPetLCD *lcd) {
  const unsigned short* sprite;
  int spriteX = 16; // Initial position of the Digimon sprite
  int spriteY = screenY; // Same vertical position as the sprite

  // Declare wallIcon and wallIconBroken
  const uint16_t* wallIcon = spriteManager->getLargeIcon(ICON_WALL);
  const uint16_t* wallIconBroken = spriteManager->getLargeIcon(ICON_WALL_BROKEN);

  // Calculate the frame based on time
  unsigned long currentTime = millis();
  int frame = ((currentTime - lastUpdateTime) / 500) % 6; // 500 ms per frame, 6 frames total

  // Determine the animation sprites based on trainingScore
  int baseSprite = (trainingScore == 14) ? SPRITE_DIGIMON_WALK_0 : SPRITE_DIGIMON_ANGRY_1;
  int alternateSprite = (trainingScore == 14) ? SPRITE_DIGIMON_HAPPY : SPRITE_DIGIMON_SURPRISED;

  // Select the sprite based on the frame
  sprite = spriteManager->getDigimonSprite(digimon.getDigimonIndex(), (frame % 2 == 0) ? baseSprite : alternateSprite);

  // Draw the Digimon sprite
  lcd->draw16BitArray(sprite, spriteX, spriteY, false, pixelColor);
}

} 