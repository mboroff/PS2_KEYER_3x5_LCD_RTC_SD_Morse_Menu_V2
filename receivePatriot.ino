void receivePatriot()
{
  String buf ="";
  char c;
  if (Serial3.available()) {
    delay(100); //allows all serial sent to be received together
    while(Serial3.available() ) {
      c = Serial3.read();
      buf +=c;
      Serial.print(c);
      tft.fillCircle(310, 125, 5, GREEN);
      if (c == '\n' || c == '\r') break;
    }
    int l = buf.length();

    if (buf[1] ==',') {
        tft.fillCircle(310, 125, 5, GREEN);
        if (buf[0] == '2') { // is it RX
            receiveFreq = buf.substring(2, l - 1);
            qsoFreq =  buf.substring(2, l - 1);
            tft.setTextSize(2);
            tft.fillRect(68, 148, 100, 20, BLUE);
            tft.setCursor(70, 150); 
            tft.print(receiveFreq);
            } else if (buf[0] == '3') { // is it RX
                       ritDisplay = buf.substring(2, l - 1);
                       tft.fillRect(338, 148, 100, 20, BLUE);
                       tft.setCursor(340, 150); 
                       tft.print(ritDisplay);
             } else if (buf[0] == '4') { // is it RX
                        txFreq = buf.substring(2, l - 1);
                        tft.fillRect(68, 168, 100, 20, BLUE);
                        tft.setCursor(70, 170); 
                        tft.print(txFreq);
             } else if (buf[0] == '5') { // is it band
                        bandDisplay = buf.substring(2, l - 1);
                        tft.fillRect(338, 168, 100, 20, BLUE);
                        tft.setCursor(340, 170); 
                        tft.print(bandDisplay);
              }  else if (buf[0] == '6') { // is it RX
                        stepDisplay = buf.substring(2, l - 1);
                        tft.fillRect(68, 188, 100, 20, BLUE);
                        tft.setCursor(70, 190); 
                        tft.print(stepDisplay);
              } else if (buf[0] == '7') { // is it RX
                         bandWidth = buf.substring(2, l - 1);
                         tft.fillRect(68, 208, 100, 20, BLUE);
                         tft.setCursor(70, 210); 
                         tft.print(bandWidth);
              }  else if (buf[0] == '8') { // is it RX
                          modeDisplay = buf.substring(2, l - 1);
                          tft.fillRect(338, 188, 100, 20, BLUE);
                          tft.setCursor(340, 190); 
                          tft.print(modeDisplay);
              } else if (buf[0] == '9') { // is it RX
              } else if (buf[0] == 'a') { // is it RX
              } else if (buf[0] == 'b') { // is it RX
              } else if (buf[0] == 'c') { // is it RX
              } else if (buf[0] == 'd') { // is it RX
              }
    }
  }
}
void updateRadio()
{
     tft.setTextSize(2);
     tft.fillRect(68, 148, 100, 20, BLUE);
     tft.setCursor(70, 150); 
     tft.print(receiveFreq);
     tft.fillRect(68, 168, 100, 20, BLUE);
     tft.setCursor(70, 170); 
     tft.print(txFreq);
     tft.fillRect(68, 188, 100, 20, BLUE);
     tft.setCursor(70, 190); 
     tft.print(stepDisplay);
     tft.fillRect(68, 208, 100, 20, BLUE);
     tft.setCursor(70, 210); 
     tft.print(bandWidth);
     
     tft.fillRect(338, 148, 100, 20, BLUE);
     tft.setCursor(340, 150); 
     tft.print(ritDisplay);
     tft.fillRect(338, 168, 100, 20, BLUE);
     tft.setCursor(340, 170); 
     tft.print(bandDisplay);
     tft.fillRect(338, 188, 100, 20, BLUE);
     tft.setCursor(340, 190); 
     tft.print(modeDisplay);
     
}
