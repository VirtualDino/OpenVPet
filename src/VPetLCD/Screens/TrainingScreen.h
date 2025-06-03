#include "../VPetLCD.h"

namespace V20{
  class TrainingScreen : public VPetLCD::Screen{
    private:
      int currentMeterIndex = 0;
      const int meter[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
      unsigned long lastUpdateTime = 0;
      bool cycling = true;
      Digimon& digimon; // Reference to the Digimon object
      AbstractSpriteManager* spriteManager;
      bool transitionToSprite = false;
      std::function<void(void)> animationEndAction;
      int trainingScore;
      std::function<void()> notifyMainCallback;
    public:
      TrainingScreen(Digimon& digimonRef, AbstractSpriteManager* spriteMgr) : digimon(digimonRef), spriteManager(spriteMgr) {} // Constructor to initialize the reference
      void draw(VPetLCD *lcd) override;
      void loop(VPetLCD *lcd, unsigned long delta);
      void handleConfirmSignal(VPetLCD *lcd);
      void nextSelection();
      int getCurrentSelection() const;
      void onEnter();
      void onExit();
      void displayTrainingAnimation(VPetLCD *lcd, int index);
      void displayMeter(VPetLCD *lcd);
      void displayResultAnimation(VPetLCD *lcd);
      void setAnimationEndAction(std::function<void(void)> action) { animationEndAction = action; }
      void setNotifyMainCallback(std::function<void()> callback) {
          notifyMainCallback = callback;
      }
  };
} 