#pragma once
#include <Arduino.h>

#define N_EVOLUTIONS 1 //the maximal evolution options per digimon
#define N_DIGIMON 4
#define DIGIMON_AGUMON 0
#define DIGIMON_KOROMON 1
#define DIGIMON_BOTAMON 2
#define DIGIMON_BETAMON 3

#define TYPE_VACCINE 0
#define TYPE_DATA 1
#define TYPE_VIRUS 2
#define TYPE_FREE 3


#define STAGE_BABY1 0
#define STAGE_BABY2 1
#define STAGE_ROOKIE 2
#define STAGE_ADULT 3
#define STAGE_PERFECT 4
#define STAGE_ULTIMATE 5
#define STAGE_SUPER_ULTIMATE 6

// #define POOP_FREQUENCY_BABY1 60*3 //3 minutes
// #define POOP_FREQUENCY_BABY2 60*30 //30 minutes
// #define POOP_FREQUENCY_ROOKIE 60*60 // 1hour
// #define POOP_FREQUENCY_ADULT 60*70 // 70 minutes
// #define POOP_FREQUENCY_PERFECT 60*80
// #define POOP_FREQUENCY_ULTIMATE 60*100

// NOTE: I'm unsure about the poop frequency. Below is the poop frequency for the Ver. COLOR devices, but it seems like the frequency
// varies per device? I neeed to confirm this and make  decision about the frequency we should use. Leave the above for now, I wil
// remove it soon.

#define POOP_FREQUENCY_BABY1 60*3 //3 minutes
#define POOP_FREQUENCY_BABY2 60*60 //60 minutes
#define POOP_FREQUENCY_ROOKIE 60*120 // 120 minutes
#define POOP_FREQUENCY_ADULT 60*120
#define POOP_FREQUENCY_PERFECT 60*120
#define POOP_FREQUENCY_ULTIMATE 60*120

#define EVOLUTION_TIME_BABY1 60*10 //Baby1->Baby2 takes 10 minutes
#define EVOLUTION_TIME_BABY2 60*60*6 // Baby2->rookie takes 6 hours
#define EVOLUTION_TIME_ROOKIE 60*60*24 // Rookie->adult takes 24 hours
#define EVOLUTION_TIME_ADULT 60*60*36 
#define EVOLUTION_TIME_PERFECT 60*60*48

struct DigimonProperties {
    char* digiName; //Name of the Digimon
    uint8_t stage; //baby rookie adult etc.
    uint16_t minWeight;
    uint16_t hungerIntervalSec; //the time it takes for hunger to decrease in seconds
    uint16_t strengthIntervalSec; //the time it takes for strength to decrease in seconds
    uint8_t maxDigimonPower;
    uint8_t healDoses; // Number of doses required when healing. Varies per digimon.
    uint8_t sleepTime; // Time the digimon goes to sleep. Value from 1-24.

    uint8_t sleepHour;//at what time the digimon begins sleeping (0-23)
    unsigned long poopTimeSec; //the time it takes to poop in seconds
    unsigned long evolutionTimeSec; //time it takes to evolve in seconds
    uint8_t type; // Va Da Vi
    uint8_t og_slot; // slot for battles in og mode
    uint16_t evolutionOptions; // how many possible evolutions there are in the evolution data array in progmem
};


struct NormalEvolutionData {
    uint16_t indexOfDigimonAfterEvolution;
    boolean careMistakes;
    uint8_t mustHaveCareMistakes; //if the caremistakes are 3-5, 
    uint8_t optionalCareMistakes; //then musthave is 3 and optional is 2

    boolean careBattles;
    uint8_t mustHaveBattles;
    uint8_t optionalBattles;

    boolean careOverFeed;
    uint8_t mustHaveOverfeed;
    uint8_t optionalOverfeed;

    boolean careEfford;
    uint8_t mustHaveEfford;
    uint8_t optionalEfford;

    boolean careBattledWith;
    uint16_t battledWithDigimonId;
};


const DigimonProperties DIGIMON_DATA[N_DIGIMON] PROGMEM = {
    {"Agumon",STAGE_ROOKIE,20,2880,2880,20,2 ,20,22,POOP_FREQUENCY_ROOKIE,EVOLUTION_TIME_ROOKIE,TYPE_DATA,0x03,0},
    {"Koromon",STAGE_BABY2,10,1800,1800,0 ,1 ,20,22,POOP_FREQUENCY_BABY2,EVOLUTION_TIME_BABY2,TYPE_DATA,0x03,1},
    {"Botamon",STAGE_BABY1,5 ,180 ,180 ,0 ,1 ,20,22,POOP_FREQUENCY_BABY1,EVOLUTION_TIME_BABY1 ,TYPE_DATA,0x03,1},
    {"Betamon",STAGE_ROOKIE,20,2880,2880,20,2 ,21,21,POOP_FREQUENCY_ROOKIE,EVOLUTION_TIME_ROOKIE,TYPE_DATA,0x03,0},

};

const NormalEvolutionData NORMALEVOLUTIONDATA[N_DIGIMON][N_EVOLUTIONS] PROGMEM = {
    //Agumons Digitations
    {NULL},

    //Koromons Digitations
    {{DIGIMON_AGUMON, true,0,2,false,0,0,false,0,0,false,0,0,false,0}},

    //Botamons Digitations
    {{DIGIMON_KOROMON, false,0,0,false,0,0,false,0,0,false,0,0,false,0}},

    //Betamons Digitations
    {NULL},
};



class Digimon{

    private:

        uint16_t digimonIndex=0;

        const DigimonProperties* properties;

        //variables not to save
        boolean evolved=false;

        //variables to save
        uint8_t state=0; //alive, sleep, dying, egg_state, super_dead, question_egg, dead, inbed/actually sleep
        uint16_t age=0;
        uint16_t weight=0;
        uint16_t feedCounter;
        uint16_t careMistakes=0;
        uint16_t trainingCounter;
        uint8_t numberOfPoops=0;
        uint8_t hunger=0;
        uint16_t appetite=0;
        uint8_t strength=0;
        uint8_t effort;
        uint8_t digimonPower; //dp
        uint8_t hungerHeartsCount=0;
        uint8_t strengthHeartsCount=0;
        uint8_t overfeedCount;
        uint8_t overdoseCount;
        uint8_t overdoseTracker; // Every 4 protein consumed = 1 OD. Track progress towards OD.
        boolean hungerCallCheck; // Digimon calls twice when it needs help. This variable checks if it has called once already so a care mistake can be applied.
        boolean strengthCallCheck; // Same as above.
        boolean overfeedCheck; // Check if Digimon is currently overfed.

        uint8_t injuryCount;
        boolean isInjured;
        uint8_t numberOfTrainingSessions;
        //uint16_t singleTotalBattleRecord
        //uint16_t tagTotalBattleRecord
        //uint16_t singleTotalBattleWins
        //uint16_t tagTotalBattleWins

        

        //timers
        unsigned long poopTimer;
        unsigned long hungerTimer;
        unsigned long hungerMistakeTimer;
        unsigned long strengthTimer;
        unsigned long strengthMistakeTimer;
        unsigned long ageTimer;
        unsigned long evolutionTimer;

        void updateTimers(unsigned long delta);


    public:

        Digimon(uint16_t index){digimonIndex =index;};
        void loop(unsigned long delta);
        boolean isEvolved(){return evolved;};

        //setters
        void setProperties(const DigimonProperties* _properties){properties=_properties;};
        void setDigimonIndex(uint16_t _digimonIndex){digimonIndex=_digimonIndex;};
        void setState(uint8_t _state){state=_state;}; //alive, sleep, dying, egg_state, super_dead, question_egg, dead, inbed/actually sleep
        void setAge(uint16_t _age){age=_age;};
        void setWeight(uint16_t _weight){weight=_weight;};
        void setFeedCounter( uint16_t _feedCounter){feedCounter=_feedCounter;};
        void setCareMistakes(uint16_t _careMistakes){careMistakes=_careMistakes;};
        void setTrainingCounter( uint16_t _trainingCounter){trainingCounter=_trainingCounter;};
        void setPoopTimer(unsigned long _poopTimer){poopTimer=_poopTimer;};
        void setHungerTimer(unsigned long _hungerTimer){hungerTimer=_hungerTimer;};
        void setHungerMistakeTimer(unsigned long _hungerMistakeTimer){hungerMistakeTimer=_hungerMistakeTimer;};
        void setStrengthTimer(unsigned long _strengthTimer){strengthTimer=_strengthTimer;};
        void setStrengthMistakeTimer(unsigned long _strengthMistakeTimer){strengthMistakeTimer=_strengthMistakeTimer;};
        void setAgeTimer(unsigned long _ageTimer){ageTimer=_ageTimer;};
        void setEvolutionTimer(unsigned long _evolutionTimer){evolutionTimer=_evolutionTimer;};

        void setNumberOfPoops(uint8_t _numberOfPoops){numberOfPoops=_numberOfPoops;};
        void setHunger(uint8_t _hunger){hunger=_hunger;};
        void setStrength(uint8_t _strength){strength=_strength;};
        void setEffort(uint8_t _effort){effort=_effort;};
        void setDigimonPower(uint8_t _digimonPower){digimonPower=_digimonPower;};
        void setHungerHeartsCount(uint8_t _hungerHeartsCount){hungerHeartsCount=_hungerHeartsCount;};
        void setStrengthHeartsCount(uint8_t _strengthHeartsCount){strengthHeartsCount=_strengthHeartsCount;};
        void setOverfeedCount(uint8_t _overfeedCount){overfeedCount=_overfeedCount;};
        void setAppetite(uint16_t _appetite){appetite=_appetite;};
        void setOverdoseCount(uint8_t _overdoseCount){overdoseCount=_overdoseCount;};
        void setOverdoseTracker(uint8_t _overdoseTracker){overdoseTracker=_overdoseTracker;};
        void setHungerCallCheck(boolean _hungerCallCheck){hungerCallCheck=_hungerCallCheck;};
        void setStrengthCallCheck(boolean _strengthCallCheck){strengthCallCheck=_strengthCallCheck;};
        void setOverfeedCheck(boolean _overfeedCheck){overfeedCheck=_overfeedCheck;};
        void setIsInjured(boolean _isInjured){isInjured=_isInjured;};
        void setInjuryCount(uint8_t _injuryCount){injuryCount=_injuryCount;};

        
        //getters
        
        uint16_t getDigimonIndex(){return digimonIndex;};
        const DigimonProperties* getProperties(){return properties;};
        uint8_t getState(){return state;}; //alive, sleep, dying, egg_state, super_dead, question_egg, dead, inbed/actually sleep
        uint16_t getAge(){return age;};
        uint16_t getWeight(){return weight;};
        uint16_t getFeedCounter(){return feedCounter;};
        uint16_t getCareMistakes(){return careMistakes;};
        uint16_t getTrainingCounter(){return trainingCounter;};
        unsigned long getPoopTimer(){return poopTimer;};
        unsigned long getHungerTimer(){return hungerTimer;};
        unsigned long getHungerMistakeTimer(){return hungerMistakeTimer;};
        unsigned long getStrengthTimer(){return strengthTimer;};
        unsigned long getStrengthMistakeTimer(){return strengthMistakeTimer;};
        unsigned long getAgeTimer(){return ageTimer;};
        unsigned long getEvolutionTimer(){return evolutionTimer;};
        uint8_t getNumberOfPoops(){return numberOfPoops;};
        uint8_t getHunger(){return hunger;};
        uint8_t getStrength(){return strength;};
        uint8_t getEffort(){return effort;};
        uint8_t getDigimonPower(){return digimonPower;};
        uint8_t getHungerHeartsCount(){return hungerHeartsCount;};
        uint8_t getStrengthHeartsCount(){return strengthHeartsCount;};
        uint8_t getOverfeedCount(){return overfeedCount;};
        uint16_t getAppetite(){return appetite;};
        uint8_t getOverdoseCount(){return overdoseCount;};
        uint8_t getOverdoseTracker(){return overdoseTracker;};
        boolean getOverfeedChecker(){return overfeedCheck;};
     


        void printSerial();
        void reduceHunger(int8_t amount){hunger-=amount;};
        void addHunger(int8_t amount){hunger+=amount;};
        void reduceAppetite(int8_t amount){appetite-=amount;};
        void addAppetite(int8_t amount){appetite+=amount;};
        void reduceStrength(int8_t amount){strength-=amount;};
        void addStrength(int8_t amount){strength+=amount;};
        void addWeight(int8_t w){weight+=w;};
        void loseWeight(int8_t w){weight-=w;};
        void increaseOverdoseCount(int8_t od){overdoseCount+=od;};
        void increaseOverdoseTracker(int8_t amount){overdoseTracker+=amount;};
        void addCareMistake(int8_t m){careMistakes+=m;};
        void addOverfeed(int8_t of){overfeedCount+=of;};
        // void addStrength(int8_t s){strength+=s;};
        // void loseStrength(int8_t s){strength -=s;};

        void addDigimonPower(int8_t dp){digimonPower+=dp;};
        void addInjury(int8_t inj){injuryCount+=inj;};

};