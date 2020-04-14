#ifndef _SERIAL_DEBUG_H_
#define _SERIAL_DEBUG_H_

unsigned int8 sd_enabled = 0;

void sd_setup() {
  Serial.begin(115200);
}

void sd_print(void *msg) {
  if(sd_enabled == 0) return;

  Serial.print(msg);
}

void sd_println(void *msg) {
  if(sd_enabled == 0) return;

  Serial.println(msg);
}

#endif
