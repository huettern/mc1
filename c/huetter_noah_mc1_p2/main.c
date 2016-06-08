/**
 *  filename: main.c
 *  Author: Noah Huetter
 *  Email:  noah.huetter@students.fhnw.ch 
 *  Date: 2016-06-08
 */

//================================================================================
// Includes
//================================================================================
#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//================================================================================
// Defines
//===============================================================================
#define DOOR_PIN 4
#define DOOR_PORT_IN PINA

#define BUTTON_PIN 5
#define BUTTON_PORT_IN PINA

#define STATUS_PIN 0
#define STATUS_PORT PORTA

#define SIREN_PIN 1
#define SIREN_PORT PORTA

#define TIMEOUT 100

//================================================================================
// Funtion declarations
//================================================================================
void stateMachine(void);
void UpdateStatusanzeige(void);

void initIO (void);

unsigned char getDoor(void);
unsigned char getTaster(void);

//================================================================================
// Static data
//================================================================================
enum STATE
{
    SGehen, SScharf, SKommen, SAlarm, SIdle
};

enum StateDisplay
{
    Aus, Ein, Langsamblinken, Schnellblinken
};

static enum StateDisplay Statusanzeige = Aus;

//================================================================================
// Main routine
//================================================================================
int main(void)
{
    initIO();
    while(1){
        stateMachine();
        UpdateStatusanzeige();
        _delay_ms(100);
    }
}

//================================================================================
// State machine
//================================================================================
void stateMachine(void) 
{
    static int timer = 0;
    static enum STATE state = SIdle;
    
    enum STATE nextState = state;
    unsigned char inputDoor, inputTaster;

    inputDoor = getDoor();
    inputTaster = getTaster();

    switch(state)
    {
        case SIdle:
            if(inputTaster && !inputDoor) 
            {
                timer = 0;
                Statusanzeige = Langsamblinken;
                nextState = SGehen;
            }
            break;
        case SGehen:
            if(inputTaster)
            {
                Statusanzeige = Aus;
                nextState = SIdle;
            }
            else if(timer >= TIMEOUT)
            {
                Statusanzeige = Ein;
                nextState = SScharf;
            }
            else
            {
                timer++;
            }
            break;
        case SScharf:
            if(inputDoor)
            {
                timer = 0;
                Statusanzeige = Schnellblinken;
                nextState = SKommen;
            }
            break;
        case SKommen:
            if(timer >= TIMEOUT)
            {
                SIREN_PORT |= (1<<SIREN_PIN);
                nextState = SAlarm;
            }
            else if(inputTaster)
            {
                Statusanzeige = Aus;
                nextState = SIdle;
            }
            else 
            {
                timer++;
            }
            break;
        case SAlarm:
            if(inputTaster)
            {
                SIREN_PORT &= ~(1<<SIREN_PIN);
                Statusanzeige = Aus;
                nextState = SIdle;
            }
            break;
        default:
            nextState = SIdle;
            break;

    }
    state = nextState;
}


void UpdateStatusanzeige(void)
{
    static unsigned int counter = 0;

    switch (Statusanzeige)
    {
        case Schnellblinken:
            if(++counter >= 2)
            {
                counter = 0;
                STATUS_PORT ^= (1<<STATUS_PIN);
            }
            break;
        case Langsamblinken:
            if(++counter >= 5)
            {
                counter = 0;
                STATUS_PORT ^= (1<<STATUS_PIN);
            }
            break;
        case Ein:
            counter = 0;
            STATUS_PORT |= (1<<STATUS_PIN);
            break;
        case Aus:
            counter = 0;
            STATUS_PORT &= ~(1<<STATUS_PIN);
            break;
    }
}

//================================================================================
// IO Functions
//================================================================================
unsigned char getDoor(void){
    unsigned char but;
    but = ((~DOOR_PORT_IN & (1<<DOOR_PIN))>>DOOR_PIN);
    return but;
}

unsigned char getTaster(void){
    static unsigned char but_old;
    unsigned char but;
    but = ((~BUTTON_PORT_IN & (1<<BUTTON_PIN))>>BUTTON_PIN);
    if((but != but_old) && but)
    {
        but_old = but;
        return but;
    }
    else
    {
        but_old = but;
        return 0;
    }
}

//================================================================================
// IO Init Functions
//================================================================================
void initIO (void) 
{
    //PORT A Direction
    DDRA    &= ~((1<<DOOR_PIN)|(1<<BUTTON_PIN)); //Inputs
    DDRA    |=  ((1<<STATUS_PIN)|(1<<SIREN_PIN)); //Outputs
    //PORT A Pullups
    PORTA   |=  ((1<<DOOR_PIN)|(1<<BUTTON_PIN)); //set pullups
}
// EOF
