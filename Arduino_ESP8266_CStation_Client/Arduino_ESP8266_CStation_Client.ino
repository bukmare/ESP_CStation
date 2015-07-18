
#define BAUD_RATE 38400

#define CSTATION_DEBUG

#ifdef CSTATION_DEBUG
  #define DEBUG_WRITE(x) { Serial.println(x); }
#else
  #define DEBUG_WRITE(x) {}
#endif

enum lcdmode {
  lm_info,
  lm_message
};

unsigned long int millis_sum_delay = 0;
byte errors_count = 0;
lcdmode lmode = lm_info;

void setup()
{
  Serial.begin(BAUD_RATE);
  initESP();
  initLCD();
  delay(100);
  initSensors();
  delay(100);
  DEBUG_WRITE("Start\r\n\r\n");
  delay(100);
  StartConnection(true);
}

void loop()
{
  executeCommands();
  millis_sum_delay += 100;
  delay (100);
  sensorsSending();
}

void executeCommands() 
{
  char *message = readTCPMessage( 1000, NULL );
  if (message) {
    char* param;
    if ((param = getMessageParam(message, "SERV_RST=1", true))) 
    {
      closeConnection(5);
      StartConnection(true);
      delay(1000);
    } else if ((param = getMessageParam(message, "SERV_CONF=1", true))) {
      closeConnection(5);
      StartConfiguringMode();
      delay(1000);
    } else if ((param = getMessageParam(message, "TONE=", true))) {
      DEBUG_WRITE(param);
      if (param[0]=='0') {
        DEBUG_WRITE("Stopping tone");
        noTone(7);
      } else {
        DEBUG_WRITE("Starting tone");
        tone(7, 500);
      }
    }
  }
}


