#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Debounce.h>
#include <EEPROM.h>
#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(12, 13, 0x01);     //tx,rx, slaveaddr
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
byte bt1 = 2;
byte bt2 = 3;
Debounce btd1(bt1, 150);
Debounce btd2(bt2, 150);
volatile int bts1 = 0;
volatile int bts2 = 0;
int menu = 0;
float eepval = 0;
int eepaddr = 0; //eepromaddress
float whprev;

void btn1_ISR();
void btn2_ISR();
void tegangan();
void frekuensi();
void arusreal();
void arusapp();
void powerreal();
void powerapp();
void pf();
void energi();
void resetenergi();
void (*resetmcu)(void) = 0;

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  //pzem.setAddress(0x01);
  pinMode(bt1, INPUT_PULLUP);
  pinMode(bt2, INPUT_PULLUP);
  attachInterrupt(0, btn1_ISR, RISING);
  attachInterrupt(1, btn2_ISR, RISING);
  lcd.setCursor(5, 0);
  lcd.print("Ways's");
  lcd.setCursor(3, 1);
  lcd.print("WATTMETER");
  delay(1500);
}

void btn1_ISR()
{
  bts1 = btd1.read();
  if (bts1 == LOW)
  {
    menu++;
    if (menu > 7)
    {
      menu = 0;
    }
  }
}

void btn2_ISR()
{
  bts2 = btd2.read();
  if (bts2 == LOW)
  {
    resetenergi();
  }
}

void tegangan()
{
  float teg = pzem.voltage();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tegangan (V)");
  lcd.setCursor(0, 1);
  lcd.print(teg);
}

void frekuensi()
{
  float freq = pzem.frequency();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Frekuensi (Hz)");
  lcd.setCursor(0, 1);
  lcd.print(freq);
}

void arusreal()
{
  float cur1 = pzem.current() * 1000;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Arus (mA)");
  lcd.setCursor(0, 1);
  lcd.print(cur1);
}

void arusapp()
{
  float pf = pzem.pf();
  float cur2 = pzem.current();
  float iapp = (cur2 / pf) * 1000;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Arus Apprnt (mA)");
  lcd.setCursor(0, 1);
  lcd.print(iapp);
}

void powerreal()
{
  float watt = pzem.power();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power (Watt)");
  lcd.setCursor(0, 1);
  lcd.print(watt);
}

void powerapp()
{
  float pf = pzem.pf();
  float pw = pzem.power();
  float va = pw / pf;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power (VA)");
  lcd.setCursor(0, 1);
  lcd.print(va);
}

void pf()
{
  float pfac = pzem.pf();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power Factor");
  lcd.setCursor(0, 1);
  lcd.print(pfac);
}

void energi()
{
  float whcurr = pzem.energy() * 1000;
  EEPROM.get(eepaddr, eepval);
  float wh = whcurr - eepval;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Energi (Wh)");
  lcd.setCursor(0, 1);
  lcd.print(wh,0);
}

void resetenergi()
{
  whprev = pzem.energy() * 1000;
  EEPROM.put(eepaddr, whprev);
  delay(500);
}

void loop()
{
  switch (menu)
  {
  case 0:
    powerreal();
    break;
  case 1:
    powerapp();
    break;
  case 2:
    arusreal();
    break;
  case 3:
    arusapp();
    break;
  case 4:
    pf();
    break;
  case 5:
    frekuensi();
    break;
  case 6:
    tegangan();
    break;
  case 7:
    energi();
    break;
  default:
    tegangan();
    break;
  }
}
