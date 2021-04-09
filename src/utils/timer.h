#ifndef _TIMER_H_
#define _TIMER_H_

long timer_current_time = 0;

struct Timer {
  unsigned long interval;
  unsigned long next_tick;
};

Timer* timer_create(unsigned long interval_in_ms) {
  struct Timer *new_timer = (Timer*)malloc(sizeof(struct Timer));

  new_timer->interval = interval_in_ms;
  new_timer->next_tick = interval_in_ms;

  return new_timer;
}

bool timer_check(Timer *timer) {
  if(timer->next_tick < timer_current_time) {
    timer->next_tick = timer_current_time + timer->interval;

    return true;
  }

  return false;
}

void timer_tick() {
  timer_current_time = millis();
}

#endif
