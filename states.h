#ifndef STATES_H_INCLUDED
#define STATES_H_INCLUDED

enum class State {
  OFFLINE, // initial state
  WAITING, // waiting state
  PIR_ACTIVATED, // PIR sensor is active
  BUZZER_ACTIVATED, // BUZZER is active
  INCORRECT // password is incorrect
};

#endif 