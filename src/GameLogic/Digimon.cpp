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
  Serial.println(getStrengthTimer());
  Serial.println(getAgeTimer());
  Serial.println(getEvolutionTimer());
}


void Digimon::loop(unsigned long delta){

    updateTimers(delta);
}


void Digimon::updateTimers(unsigned long delta){
    poopTimer += delta;
    if(poopTimer > properties->poopTimeSec*1000){
        poopTimer %= properties->poopTimeSec*1000;
        numberOfPoops++;
        loseWeight(1);
    }

    if(appetite > 0) {
        hungerTimer += delta;
        if(hungerTimer > properties->hungerIntervalSec*1000){
            hungerTimer %= properties->hungerIntervalSec*1000;
            reduceAppetite(1);
            if (appetite < 0) {
                setHungerHeartsCount(0);
            } else if (appetite > 4) {
                setHungerHeartsCount(4);
            } else {
                setHungerHeartsCount(appetite);
            }
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

    ageTimer += delta;
    uint32_t day = 1000*60*60*24;
    if(ageTimer >= day){
        ageTimer %= day;
        age++;
    }

    evolutionTimer += delta;
    if(evolutionTimer >= properties->evolutionTimeSec*1000){
        evolutionTimer =0;
        evolved = true;
    }

}