/*Установить библиотеки "OneWire" и "DallasTemperature"*/
#include <SPI.h>
#include <Wire.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/wdt.h>

uint32_t timer = millis();

volatile unsigned int flow_1_frequency; // Measures flow sensor pulses
volatile unsigned int flow_2_frequency; // Measures flow sensor pulses
volatile unsigned int flow_3_frequency; // Measures flow sensor pulses
volatile unsigned int flow_4_frequency; // Measures flow sensor pulses
double L_minute_1 = -999999; // Calculated litres/hour
double L_minute_2 = -999999; // Calculated litres/hour
double L_minute_3 = -999999; // Calculated litres/hour
double L_minute_4 = -999999; // Calculated litres/hour
#define FLOW_1_PIN 18
#define FLOW_2_PIN 19
#define FLOW_3_PIN 20
#define FLOW_4_PIN 21

#define TEMP_1_PIN 33
#define TEMP_2_PIN 32
#define TEMP_3_PIN 31
#define TEMP_4_PIN 30
#define TEMP_5_PIN 29
#define TEMP_6_PIN 28
#define TEMP_7_PIN 26


OneWire temp_1_wire(TEMP_1_PIN);
OneWire temp_2_wire(TEMP_2_PIN);
OneWire temp_3_wire(TEMP_3_PIN);
OneWire temp_4_wire(TEMP_4_PIN);
OneWire temp_5_wire(TEMP_5_PIN);
OneWire temp_6_wire(TEMP_6_PIN);
OneWire temp_7_wire(TEMP_7_PIN);

DallasTemperature temp_1(&temp_1_wire);
DallasTemperature temp_2(&temp_2_wire);
DallasTemperature temp_3(&temp_3_wire);
DallasTemperature temp_4(&temp_4_wire);
DallasTemperature temp_5(&temp_5_wire);
DallasTemperature temp_6(&temp_6_wire);
DallasTemperature temp_7(&temp_7_wire);

DeviceAddress temp_1_address;
DeviceAddress temp_2_address;
DeviceAddress temp_3_address;
DeviceAddress temp_4_address;
DeviceAddress temp_5_address;
DeviceAddress temp_6_address;
DeviceAddress temp_7_address;

byte MacAddress[] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x10 };
IPAddress LocalIPAddress(192, 168, 3, 10);
unsigned int LocalPort = 6000;
IPAddress DestinationAddress(255, 255, 255, 255);
unsigned int DestinationPort = 6000;
char IncomingBuffer[UDP_TX_PACKET_MAX_SIZE];
EthernetUDP Udp;

void flow_1() // Interrupt function
{
   flow_1_frequency++;
}
void flow_2() // Interrupt function
{
   flow_2_frequency++;
}
void flow_3() // Interrupt function
{
   flow_3_frequency++;
}
void flow_4() // Interrupt function
{
   flow_4_frequency++;
}

void setup() {
  Ethernet.begin(MacAddress, LocalIPAddress);
  Udp.begin(LocalPort);
  delay(10);

  Serial.begin(115200);

  temp_1.begin();
  temp_1.getAddress(temp_1_address, 0);
  temp_1.setResolution(temp_1_address, 12);

  temp_1.begin();
  temp_2.getAddress(temp_2_address, 0);
  temp_2.setResolution(temp_2_address, 12);

  temp_3.begin();
  temp_3.getAddress(temp_3_address, 0);
  temp_3.setResolution(temp_3_address, 12);

  temp_4.begin();
  temp_4.getAddress(temp_4_address, 0);
  temp_4.setResolution(temp_4_address, 12);

  temp_5.begin();
  temp_5.getAddress(temp_5_address, 0);
  temp_5.setResolution(temp_5_address, 12);

  temp_6.begin();
  temp_6.getAddress(temp_6_address, 0);
  temp_6.setResolution(temp_6_address, 12);

  temp_7.begin();
  temp_7.getAddress(temp_7_address, 0);
  temp_7.setResolution(temp_7_address, 12);

  pinMode(FLOW_1_PIN, INPUT);
  attachInterrupt(5, flow_1, RISING); // Setup Interrupt
  pinMode(FLOW_2_PIN, INPUT);
  attachInterrupt(4, flow_2, RISING); // Setup Interrupt
  pinMode(FLOW_3_PIN, INPUT);
  attachInterrupt(3, flow_3, RISING); // Setup Interrupt
  pinMode(FLOW_4_PIN, INPUT);
  attachInterrupt(2, flow_4, RISING); // Setup Interrupt
  sei(); // Enable interrupts

  wdt_enable(WDTO_8S);
}

void loop() {
    if (timer > millis()) timer = millis(); // if millis() or timer wraps around, we'll just reset it
    if (millis() - timer >= 2000) {  // approximately every 2 seconds or so, print out the current stats
    timer = millis(); // reset the timer

    // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
    L_minute_1 = (flow_1_frequency / 7.5); // (Pulse frequency) / 7.5Q = flowrate in L/minute
    L_minute_2 = (flow_2_frequency / 7.5); // (Pulse frequency) / 7.5Q = flowrate in L/minute
    L_minute_3 = (flow_3_frequency / 7.5); // (Pulse frequency) / 7.5Q = flowrate in L/minute
    L_minute_4 = (flow_4_frequency / 7.5); // (Pulse frequency) / 7.5Q = flowrate in L/minute
    Udp.beginPacket(DestinationAddress, DestinationPort);
    Udp.print("WATER_FLOW_1=");
    Udp.println(L_minute_1);
    Udp.endPacket();
    Udp.beginPacket(DestinationAddress, DestinationPort);
    Udp.print("WATER_FLOW_2=");
    Udp.println(L_minute_2);
    Udp.endPacket();
    Udp.beginPacket(DestinationAddress, DestinationPort);
    Udp.print("WATER_FLOW_3=");
    Udp.println(L_minute_3);
    Udp.endPacket();
    Udp.beginPacket(DestinationAddress, DestinationPort);
    Udp.print("WATER_FLOW_4=");
    Udp.println(L_minute_4);
    Udp.endPacket();

    Serial.print("WATER_FLOW_1=");
    Serial.println(L_minute_1);
    Serial.print("WATER_FLOW_2=");
    Serial.println(L_minute_2);
    Serial.print("WATER_FLOW_3=");
    Serial.println(L_minute_3);
    Serial.print("WATER_FLOW_4=");
    Serial.println(L_minute_4); 
    
    float ekv_1_temp = temp_1.getTempC(temp_1_address);
    float ekv_2_temp = temp_2.getTempC(temp_2_address);
    float ekv_3_temp = temp_3.getTempC(temp_3_address);
    float ekv_4_temp = temp_4.getTempC(temp_4_address);
    float ekv_5_temp = temp_5.getTempC(temp_5_address);
    float ekv_6_temp = temp_6.getTempC(temp_6_address);
    float ekv_7_temp = temp_7.getTempC(temp_7_address);

    if (ekv_1_temp > -100){
      Udp.beginPacket(DestinationAddress, DestinationPort);
      Udp.print("TEMP_DS1=");
      Udp.println(ekv_1_temp);
      Udp.endPacket();
    }
    if (ekv_2_temp > -100){
      Udp.beginPacket(DestinationAddress, DestinationPort);
      Udp.print("TEMP_DS2=");
      Udp.println(ekv_2_temp);
      Udp.endPacket();
    }
    if (ekv_3_temp > -100){
      Udp.beginPacket(DestinationAddress, DestinationPort);
      Udp.print("TEMP_DS3=");
      Udp.println(ekv_3_temp);
      Udp.endPacket();
    }
    if (ekv_4_temp > -100){
      Udp.beginPacket(DestinationAddress, DestinationPort);
      Udp.print("TEMP_DS4=");
      Udp.println(ekv_4_temp);
      Udp.endPacket();
    }
    if (ekv_5_temp > -100){
      Udp.beginPacket(DestinationAddress, DestinationPort);
      Udp.print("TEMP_DS5=");
      Udp.println(ekv_5_temp);
      Udp.endPacket();
    }
    if (ekv_6_temp > -100){
      Udp.beginPacket(DestinationAddress, DestinationPort);
      Udp.print("TEMP_DS6=");
      Udp.println(ekv_6_temp);
      Udp.endPacket();
    }
    if (ekv_7_temp > -100){
      Udp.beginPacket(DestinationAddress, DestinationPort);
      Udp.print("TEMP_DS7=");
      Udp.println(ekv_7_temp);
      Udp.endPacket();
    }
    
    if (ekv_1_temp > -100){
      Serial.print("TEMP_DS1=");    
      Serial.println(ekv_1_temp);
    }
    if (ekv_2_temp > -100){
      Serial.print("TEMP_DS2=");    
      Serial.println(ekv_2_temp);
    }
    if (ekv_3_temp > -100){
      Serial.print("TEMP_DS3=");    
      Serial.println(ekv_3_temp);
    }
    if (ekv_4_temp > -100){
      Serial.print("TEMP_DS4=");    
      Serial.println(ekv_4_temp);
    }
    if (ekv_5_temp > -100){
      Serial.print("TEMP_DS5=");    
      Serial.println(ekv_5_temp);
    }
    if (ekv_6_temp > -100){
      Serial.print("TEMP_DS6=");    
      Serial.println(ekv_6_temp);
    }
    if (ekv_7_temp > -100){
      Serial.print("TEMP_DS7=");    
      Serial.println(ekv_7_temp);
    }
    
    temp_1.requestTemperatures();
    temp_2.requestTemperatures();
    temp_3.requestTemperatures();
    temp_4.requestTemperatures();
    temp_5.requestTemperatures();
    temp_6.requestTemperatures();
    temp_7.requestTemperatures();

    flow_1_frequency = 0; // Reset Counter
    flow_2_frequency = 0; // Reset Counter
    flow_3_frequency = 0; // Reset Counter
    flow_4_frequency = 0; // Reset Counter
    L_minute_1 = 0;
    L_minute_2 = 0;
    L_minute_3 = 0;
    L_minute_4 = 0;

    wdt_reset();
  }
}
