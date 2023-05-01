/*
  ADE9153A Custom Chip Arduino Driver Code
*/

#include <SPI.h>
#include "ADE9153A.h"
#include "ADE9153AAPI.h"
#define SPI_SPEED 1000000     //SPI Speed
#define CS 10
ADE9153AClass ade9153A;

struct EnergyRegs energyVals;  //Energy register values are read and stored in EnergyRegs structure
struct PowerRegs powerVals;    //Metrology data can be accessed from these structures
struct RMSRegs rmsVals;  
struct PQRegs pqVals;
struct AcalRegs acalVals;
struct Temperature tempVal;

void readandwrite(void);

unsigned long lastReport = 0;
const long reportInterval = 2000;
const long blinkInterval = 500;


void setup() {
  uint16_t my_data = 0xbeef;
  uint16_t my_data_new ;
  uint32_t my_product=0;
  
  Serial.begin(115200);
  pinMode(CS, OUTPUT);

  bool commscheck = ade9153A.SPI_Init(SPI_SPEED,CS); //Initialize SPI

  if (!commscheck) {
    Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");
    while (!commscheck) {     //Hold until arduino is reset
      delay(1000);
    }
  }

  Serial.println(String(ade9153A.SPI_Read_32(REG_VERSION_PRODUCT), HEX)); // Version of IC
  // ade9153A.SPI_Write_32(REG_AIGAIN, -268435456); //AIGAIN to -1 to account for IAP-IAN swap
  delay(500); 
}

void loop() {
  unsigned long currentReport = millis();

  if ((currentReport - lastReport) >= reportInterval){
    lastReport = currentReport;
    readandwrite();
  }


}



void readandwrite()
{ 
 /* Read and Print WATT Register using ADE9153A Read Library */
  ade9153A.ReadPowerRegs(&powerVals);    //Template to read Power registers from ADE9000 and store data in Arduino MCU
  ade9153A.ReadRMSRegs(&rmsVals);
  // ade9153A.ReadPQRegs(&pqVals);
  // ade9153A.ReadTemperature(&tempVal);
  
  Serial.print("RMS Current:\t");        
  Serial.print(rmsVals.CurrentRMSValue/1000); 
  Serial.println(" A");
  
  Serial.print("RMS Voltage:\t");        
  Serial.print(rmsVals.VoltageRMSValue/1000);
  Serial.println(" V");
  
  Serial.print("Active Power:\t");        
  Serial.print(powerVals.ActivePowerValue/1000);
  Serial.println(" W");
  
  Serial.print("Reactive Power:\t");        
  Serial.print(powerVals.FundReactivePowerValue/1000);
  Serial.println(" VAR");
  
  Serial.print("Apparent Power:\t");        
  Serial.print(powerVals.ApparentPowerValue/1000);
  Serial.println(" VA");
  
  // Serial.print("Power Factor:\t");        
  // Serial.println(pqVals.PowerFactorValue);
  
  // Serial.print("Frequency:\t");        
  // Serial.print(pqVals.FrequencyValue);
  // Serial.println(" Hz");
  
  // Serial.print("Temperature:\t");        
  // Serial.print(tempVal.TemperatureVal);
  // Serial.println(" degC");

  Serial.println("");
  Serial.println("");
}
