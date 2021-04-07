#include "freertos/FreeRTOS.h"
#include "freertos/task.h" 
#include "project_config.h"
#include "rLog.h"

void app_main() 
{
  rlog_v("DEMO", "Test message, level: %s", "VERBOSE");
  rlog_d("DEMO", "Test message, level: %s", "DEBUG");
  rlog_i("DEMO", "Test message, level: %s", "INFORMATION");
  rlog_w("DEMO", "Test message, level: %s", "WARNING");
  rlog_e("DEMO", "Test message, level: %s", "ERROR");
}
