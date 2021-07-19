#include <EEPROM.h>

void writeToEEPROM(String Username, String Password){ 
  int SSIDLen = Username.length();
  int PassLen = Password.length();

  EEPROM.write(0, SSIDLen);
  EEPROM.write(SSIDLen + 1 , PassLen);
  
  for (int i = 1; i <= SSIDLen; i++){
    EEPROM.write(i,Username[i - 1]);
  }
  for (int i = SSIDLen + 2; i <= SSIDLen + PassLen + 1 ; i++){
    EEPROM.write(i,Password[i - (SSIDLen + 2)]);
  }
   
  EEPROM.commit();
}


String getAPNAME(){
  int SSIDLen = EEPROM.read(0);
  
  String APNAME = "";
  for (int i = 1; i <= SSIDLen ; i++){
    APNAME += (char) EEPROM.read(i);
  }
  return APNAME;
}


String getPass(){
  int SSIDLen = EEPROM.read(0);
  int PassLen = EEPROM.read(SSIDLen + 1);

  String Pass = "";
  for (int i = SSIDLen + 2; i <= SSIDLen + 1 + PassLen ; i++){
    Pass += (char) EEPROM.read(i);
  }
  return Pass;
}
