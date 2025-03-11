#include "Digimon.h"

void Digimon::printSerial(){

  Serial.println(getDigimonIndex());
  Serial.println(getState());
  Serial.println(getAge());
  Serial.println(getWeight());
  Serial.println(getFeedCounter());
  Serial.println(getCareMistakes());
  Serial.println(getTrainingCounter());
  Serial.println(getPoopTimer());
  Serial.println(getHungerTimer());
  Serial.println(getHungerMistakeTimer());
  Serial.println(getStrengthTimer());
  Serial.println(getStrengthMistakeTimer());
  Serial.println(getAgeTimer());
  Serial.println(getEvolutionTimer());
  Serial.println(getReturnToSleepTimer());
}


void Digimon::loop(unsigned long delta){

    updateTimers(delta);
}


void Digimon::updateTimers(unsigned long delta){

    if (getNumberOfPoops() < 4) {
        poopTimer += delta;
        if(poopTimer > properties->poopTimeSec*1000){
            poopTimer %= properties->poopTimeSec*1000;
            numberOfPoops++;
            if (numberOfPoops == 4) {
                setIsInjured(true);
                addInjury(1);
            }
            if (getWeight() > properties->minWeight) {
                loseWeight(1);
            }
        }
    }

    if(appetite > 0) {
        hungerTimer += delta;
        if(hungerTimer > properties->hungerIntervalSec*1000){
            hungerTimer %= properties->hungerIntervalSec*1000;
            reduceAppetite(1);
            if (appetite < 0) {
                setHungerHeartsCount(0);
            } else if (appetite > 4 && appetite < 14) {
                setHungerHeartsCount(4);
            } else if (appetite == 14) {
                // Overfeed essentially acts like a 5th heart
                setHungerHeartsCount(4);
                setAppetite(4);
                setOverfeedCheck(false);
            } else {
                setHungerHeartsCount(appetite);
            }
        }
    }

    if (appetite == 0 && hungerCallCheck == false) {
        // Trigger call sound
        // ...
        hungerMistakeTimer += delta;
        if (hungerTimer > 600000) {
            setHungerCallCheck(true);
        }
    } else if (appetite == 0 && hungerCallCheck == true) {
        // Trigger call sound
        // ...
        hungerMistakeTimer += delta;
        if (hungerTimer > 600000) {
            addCareMistake(1);
        }
    }

    if(strength > 0) {
        strengthTimer += delta;
        if (strengthTimer > properties->strengthIntervalSec*1000){
            strengthTimer %= properties->strengthIntervalSec*1000;
            reduceStrength(1);
            if (strength < 0) {
                setStrengthHeartsCount(0);
            } else if (strength > 4) {
                setStrengthHeartsCount(4);
            } else {
                setStrengthHeartsCount(strength);
            }
        }
    }

    if (strength == 0 && strengthCallCheck == false) {
        // Trigger call sound
        // ...
        strengthMistakeTimer += delta;
        if (strengthTimer > 600000) {
            setStrengthCallCheck(true);
        }
    } else if (strength == 0 && strengthCallCheck == true) {
        // Trigger call sound
        // ...
        strengthMistakeTimer += delta;
        if (strengthMistakeTimer > 600000) {
            addCareMistake(1);
        }
    }

    ageTimer += delta;
    uint32_t day = 1000*60*60*24;
    if(ageTimer >= day){
        ageTimer %= day;
        age++;
    }

    evolutionTimer += delta;
    if (evolutionTimer >= properties->evolutionTimeSec * 1000 && !evolved) {
        evolutionTimer = 0;

        // Check evolution conditions from NORMALEVOLUTIONDATA
        for (int i = 0; i < N_EVOLUTIONS; i++) {
            NormalEvolutionData evoData;
            memcpy_P(&evoData, &NORMALEVOLUTIONDATA[digimonIndex][i], sizeof(NormalEvolutionData));

            if (evoData.digimonIndex == 0) {
                continue; // Skip if no evolution data
            }

            bool conditionsMet = true;

            // Check conditions (example: care mistakes, training, etc.)
            if (careMistakes > evoData.careMistakes) {
                continue;
            }
            if (trainingCounter < evoData.trainingAmount) {
                continue;
            }
            // Add other conditions as needed

            // If all conditions are met, evolve
            setDigimonIndex(evoData.digimonIndex);
            evolved = true;
            break; // Evolution done, exit loop
        }
    }

    if (canReturnToSleepCheck == true && getState() == 0) {
        returnToSleepTimer += delta;
        // Return to sleep 5 minutes after disturbance
        if (returnToSleepTimer >= 300000){
            setState(1);
            setCanReturnToSleepCheck(false);
        }
    }

}