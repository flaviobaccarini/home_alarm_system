#ifndef STATES_H
#define STATES_H
enum class State {
  OFFLINE, // initial state
  WAITING, // waiting state
  PIR_ACTIVATED, // PIR sensor is active
  BUZZER_ACTIVATED, // BUZZER is active
  INCORRECT // password is incorrect
};
#endif