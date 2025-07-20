#include "helpers.h"


bool checkBattery(){
  //3.2v w/ 2x voltage divider and 2.5V reference
  return analogRead(BAT_PIN) > 654;
}
