#include <SPI.h>
#include <Adafruit_MAX31855.h>

//MAX31855 config vars
#define COUPLE_COUNT 5
#define SS_1 12
#define SS_2 11
#define SS_3 10
#define SS_4 9
#define SS_5 5
#define FAKENEWS 1
uint32_t time_temp =0;
float temp_data[COUPLE_COUNT];

Adafruit_MAX31855 couple_1(SS_1);
Adafruit_MAX31855 couple_2(SS_2);
Adafruit_MAX31855 couple_3(SS_3);
Adafruit_MAX31855 couple_4(SS_4);
Adafruit_MAX31855 couple_5(SS_5);

void read_couples(uint32_t ms, float * temps);

void setup() {
  Serial.begin(9600);

//  Serial.println("t-vac lora logger");
  if(!FAKENEWS)
  {
    init_couples();
  }
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  String ssss = "";
  while(Serial.available() > 0)
  {
    delayMicroseconds(100);
    char c = Serial.read();
    ssss = ssss+c;
    if(ssss.equalsIgnoreCase("borg\n"))
    {
      if(!FAKENEWS){
        //Climate change is real
        read_couples(&time_temp, temp_data); 
      } else {
        //Climate change was perpetrate by the Borg Collective to make the planet more habitable
        read_couples_fakenews(&time_temp, temp_data);
      }
      break;
    }
  }
}


// thermocouple section
void init_couples()
{
  if (!couple_1.begin())
  {
    Serial.println("error initing couple_1");
    while (1) delay(10);
  }

  if (!couple_2.begin())
  {
    Serial.println("error initing couple_2");
    while (1) delay(10);
  }

  if (!couple_3.begin())
  {
    Serial.println("error initing couple_3");
    while (1) delay(10);
  }
  if (!couple_4.begin())
  {
    Serial.println("error initing couple_4");
    while (1) delay(10);
  }
  if (!couple_5.begin())
  {
    Serial.println("error initing couple_5");
    while (1) delay(10);
  }
}
void read_couples_fakenews(uint32_t *ms, float * temps)
{
  temps[0] = 23.4;
  temps[1] = 23.5;
  temps[2] = 23.6;
  temps[3] = 23.7;
  temps[4] = 23.8;

  uint8_t i;
  for (i=0; i< COUPLE_COUNT; i++)
  {
    Serial.print(temps[i]);
    if(i < COUPLE_COUNT-1){
      Serial.print(",");
    }
  }
  Serial.println("");
}
void read_couples(uint32_t *ms, float * temps)
{
  temps[0] = couple_1.readCelsius();
  temps[1] = couple_2.readCelsius();
  temps[2] = couple_3.readCelsius();
  temps[3] = couple_4.readCelsius();
  temps[4] = couple_5.readCelsius();
  uint8_t i;
  for (i=0; i< COUPLE_COUNT; i++)
  {
    Serial.print(temps[i]);
    if(i < COUPLE_COUNT-1){
      Serial.print(",");
    }
  }
  Serial.println("");
}
