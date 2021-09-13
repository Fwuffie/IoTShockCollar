const int transmitterPin = 4;

bool outbits[43] = {0,1,0,0,1,0,1,1,0,1,0,0,0,0,1,0};
bool rawoutbits[181] = {1,1,1,1,1,1,0,0,0};

void encodeOutBits(){
  for (int i = 0; i < 43; i++){
     rawoutbits[9  + (4*i)] = 1;
     rawoutbits[10 + (4*i)] = outbits[i];
     rawoutbits[11 + (4*i)] = outbits[i];
     rawoutbits[12 + (4*i)] = 0;
  }
}

void generateOutBits(int transmissionchannel, int collarmode, int value){
  transmissionchannel -= 1;
  int checksum = 128 + 13 + value + collarmode + (transmissionchannel*16);
  // Generate Channel Bits
  for (int i = 16; i < 20 ; i++){
    outbits[i] = bitRead(transmissionchannel, abs(i-19));
  }
  // Generate Mode Bits
  for (int i = 20; i < 24 ; i++){
    outbits[i] = bitRead(collarmode, abs(i-23));
  }
  // Generate Value Bits
  for (int i = 24; i < 32 ; i++){
    outbits[i] = bitRead(value, abs(i-31));
  }
  // Generate Checksum bits
  for (int i = 32; i <= 40 ; i++){
    outbits[i] = bitRead(checksum, abs(i-39));
  }
  // Generate final Low Bits
  for (int i = 40; i < 43 ; i++){
    outbits[i] = 0;
  }
}

void transmitSignal(int transmissionchannel, int collarmode, int intensity, int duration, int delayms){
  pinMode(transmitterPin, OUTPUT);
  generateOutBits(transmissionchannel, collarmode, intensity);
  encodeOutBits();
  
  int endtime = millis() + 1000*duration;
  do {
    for (int i = 0; i < 5; i++){
      for (int j = 0; j < 181; j++){
        digitalWrite(transmitterPin, rawoutbits[j]);
        delayMicroseconds(delayms);
      }
    }
    delay(50);
  } while (millis() < endtime);
}
