#include <PinChangeInt.h>

#define DEBUG_RECEIVER

#define AILERON  0
#define ELEVATOR 1
#define THROTTLE 2
#define RUDDER   3

#define AILERON_IN  4
#define ELEVATOR_IN 5
#define THROTTLE_IN 6
#define RUDDER_IN   7

uint8_t rc_pins[4] = {AILERON_IN, ELEVATOR_IN, THROTTLE_IN, RUDDER_IN};
uint8_t rc_flags[4] = {1, 2, 4, 8};
uint16_t rc_values[4] = {0, 0, 0, 0};

volatile uint8_t rc_shared_flags;
volatile uint16_t rc_shared_values[4];
volatile uint32_t rc_shared_ts[4];

void rc_channel_change(uint8_t id) {
  if (digitalRead(rc_pins[id]) == HIGH) {
    rc_shared_ts[id] = micros();
  }
  else {
    rc_shared_values[id] = (uint16_t)(micros() - rc_shared_ts[id]);
    rc_shared_flags |= rc_flags[id];
  }
}

void rc_aileron_change()  { rc_channel_change(AILERON);  }
void rc_elevator_change() { rc_channel_change(ELEVATOR); }
void rc_throttle_change() { rc_channel_change(THROTTLE); }
void rc_rudder_change()   { rc_channel_change(RUDDER);   }

void rc_setup_interrupts() {
  PCintPort::attachInterrupt(rc_pins[AILERON],  &rc_aileron_change, CHANGE);
  PCintPort::attachInterrupt(rc_pins[ELEVATOR], &rc_elevator_change, CHANGE);
  PCintPort::attachInterrupt(rc_pins[THROTTLE], &rc_throttle_change, CHANGE);
  PCintPort::attachInterrupt(rc_pins[RUDDER],   &rc_rudder_change, CHANGE);
}

void rc_process_channels() {
  static uint8_t flags;
  
  if (rc_shared_flags) {
    noInterrupts();
    flags = rc_shared_flags;
    
    if (flags & rc_flags[0]) rc_values[0] = rc_shared_values[0];
    if (flags & rc_flags[1]) rc_values[1] = rc_shared_values[1];
    if (flags & rc_flags[2]) rc_values[2] = rc_shared_values[2];
    if (flags & rc_flags[3]) rc_values[3] = rc_shared_values[3];
    
    rc_shared_flags = 0;
    interrupts(); 
  }

  flags = 0;
}

#ifdef DEBUG_RECEIVER
void rc_print_channels() {
  static char str[64];
 
  sprintf(str, "AILE: %d, ELEV: %d, THRO: %d, RUDD: %d\n",
    rc_values[0], rc_values[1], rc_values[2], rc_values[3]
  );
 
  Serial.print(str); 
}
#endif

void setup() {
  #ifdef DEBUG_RECEIVER
  Serial.begin(9600);
  #endif
  
  rc_setup_interrupts();
}

void loop() {
  rc_process_channels();
  
  #ifdef DEBUG_RECEIVER
  if (millis() % 100 == 0) {
    rc_print_channels();
  }
  #endif
}
