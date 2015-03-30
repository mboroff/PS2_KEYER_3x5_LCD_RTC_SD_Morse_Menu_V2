
void logQSOrcv()
{
#ifdef DEBUG 1
  Serial.println("logQSOrcv");
#endif

  if (stationSignalctr == 0) {
    sprintf(qsoStarttime, "%i:%i:%i", myHour, myMinute, mySecond);  // set start of qso
  }
  String buf = "";

  printCursors(inputFieldno);
  if (inputFieldno ==0) buf = stationId;
 // if (inputFieldno ==3) buf = mystationSignal;

  int bufLength;
  int workCharlength;             // temp length of field to update
  char workChar[10];   //         copy string to array area

  boolean endSubmenu = false;

  while (endSubmenu == false) {
    char ch, ch2;
    while (kbd.available()) {
      ch = kbd.read();
      ch2 = toupper(ch);
      if (ch == PS2_ENTER || ch == PS2_TAB) {
        // merge field with whatever you reading for
        bufLength = buf.length();  //get the length of data read
        if (inputFieldno == 0) {
          buf.toCharArray(workChar, buf.length() + 1); // copy all of what was read into char array
          strcpy(stationId, workChar);                // add buf read to stationId
          inputFieldno = 1;
          printCursors(1);
          buf = "";                                        //         reset buf
          break;
        }
        if (inputFieldno == 1) {    // get length of stationId data
          buf.toCharArray(workChar, buf.length() + 1); // copy all of what was read into char array
          strcat(stationSignal, workChar);                // add buf read to stationId
          inputFieldno = 2;
          printCursors(2);
          buf = "";                               //         reset buf
          break;
        }

        if (inputFieldno == 3) {    // get length of qsoFreq data
          buf.toCharArray(workChar, buf.length() + 1); // copy all of what was read into char array
          strcat(mystationSignal, workChar);                // add buf read to stationId
          inputFieldno = 3;
          printCursors(3);
          buf = "";                              //         reset buf
          break;
        }  // end of reading freq
      }    // end of enter or tab
      if (ch == PERCENTKEY || ch == PS2_ESC) {
        bufLength = buf.length();  //get the length of data read
        buf.toCharArray(workChar, buf.length() + 1); // copy all of what was read into char array
        if (inputFieldno == 0) {
          strcat(stationId, workChar);                // add buf read to stationId
        }
        if (inputFieldno == 1) {    // get length of stationId data
          strcat(stationSignal, workChar);                // add buf read to stationSignal
        }
        if (inputFieldno == 3) {    // get length of qsoFreq data
          strcat(mystationSignal, workChar);                // add buf read to mystationSignal
        }
        endSubmenu = true;
        break;
      }
      if (ch == PS2_BACKSPACE || ch == PS2_LEFTARROW) {
          bufLength = buf.length();  //get the length of data read
          String wrkBuf;
          wrkBuf = buf.substring(0, buf.length());
          buf = wrkBuf.substring(0, wrkBuf.length() - 1);
          if (inputFieldno == 0) {
              tft.fillRect(358, 168, 100, 20, BLUE);
              tft.setCursor(360, 170);
              tft.print(buf);
              stationIdctr--;
              if (stationIdctr < 0) {
                  stationIdctr = 0;
              }
          }
          if (inputFieldno == 1) {
              tft.fillRect(358, 188, 100, 20, BLUE);
              tft.setCursor(360, 190);
              tft.print(buf);
              stationSignalctr--;
              if (stationSignalctr < 0) {
                  stationSignalctr = 0;
              }
          }
          if (inputFieldno == 2)  {
              tft.fillRect(358, 208, 100, 20, BLUE);
              tft.setCursor(360, 210);
              tft.print(buf);
              qsoFreqctr--;
              if (qsoFreqctr < 0) {
                  qsoFreqctr = 0;
              }
          }
         if (inputFieldno == 3) {    // get length of qsoFreq data
             tft.fillRect(358, 228, 100, 20, BLUE);
             tft.setCursor(360, 230);
             tft.print(buf);
             mystationSignalctr--;
             if (mystationSignalctr < 0) {
                 mystationSignalctr = 0;
             }
         }
        break;
      }      
      
      /********************************************************************
                           reading section
      ********************************************************************/
      // set cursor for what you are reading if = max resize one byte less
      // display   ch2 & _
      if (inputFieldno == 0 && stationIdctr < STATIONIDSIZE) {       // ok concat
        tft.print(ch2);       // print receved char
        buf += ch2;          // add ch2 to buf
        stationIdctr++;
      }
      if (inputFieldno == 1 && stationSignalctr < STATIONSIGNALSIZE) {       // ok concat
        tft.print(ch2);       // print receved char
        buf += ch2;          // add ch2 to buf
        stationSignalctr++;
      }
      if (inputFieldno == 2 && qsoFreqctr < QSOFREQSIZE) {       // ok concat
        tft.print(ch2);       // print receved char
        buf += ch2;          // add ch2 to buf
        qsoFreqctr++;
      }
      if (inputFieldno == 3 && mystationSignalctr < STATIONSIGNALSIZE) {       // ok concat
        tft.print(ch2);       // print receved char
        buf += ch2;          // add ch2 to buf
        mystationSignalctr++;
      }

    }   // end of kb read
  }    // end of submenu
  clearCursor();
}

void clearCursor()
{
  tft.setTextColor(WHITE, BLUE);
  tft.setCursor(260, 170);
  tft.print("Rx Id  -");
  tft.fillRect(358, 168, 100, 20, BLUE);
  tft.setCursor(360, 170);
  tft.print(stationId);
  tft.setCursor(260, 190);
  tft.print("Rx Sig -");
  tft.fillRect(358, 188, 100, 20, BLUE);
  tft.setCursor(360, 190);
  tft.print(stationSignal);
  tft.setCursor(260, 210);
  tft.print("Freq   -");
  tft.fillRect(358, 208, 100, 20, BLUE);
  tft.setCursor(360, 210);
  tft.print(qsoFreq);
  tft.setCursor(260, 230);
  tft.print("My Sig -");
  tft.fillRect(358, 228, 100, 20, BLUE);
  tft.setCursor(360, 230);
  tft.print(mystationSignal);

}

void printCursors(int fieldNo)
{
#ifdef DEBUG
  Serial.print("fieldNo = "); Serial.println(fieldNo);
#endif
  if (fieldNo == 0) {
    tft.setTextColor(WHITE, BLUE);
    tft.setCursor(260, 190);
    tft.print("Rx Sig -");
    tft.fillRect(358, 188, 100, 20, BLUE);
    tft.setCursor(360, 190);
    tft.print(stationSignal);
    tft.setCursor(260, 210);
    tft.print("Freq   -");
    tft.fillRect(358, 208, 100, 20, BLUE);
    tft.setCursor(360, 210);
    tft.print(qsoFreq);
    tft.setCursor(260, 230);
    tft.print("My Sig -");
    tft.fillRect(358, 228, 100, 20, BLUE);
    tft.setCursor(360, 230);
    tft.print(mystationSignal);
    tft.setTextColor(GREEN, BLUE);
    tft.setCursor(260, 170);
    tft.print("Rx Id  -");
    tft.fillRect(358, 168, 100, 20, BLUE);
    tft.setCursor(360, 170);
    tft.print(stationId);
  }
  if (fieldNo == 1) {
    tft.setTextColor(WHITE, BLUE);
    tft.setCursor(260, 170);
    tft.print("Rx Id  -");
    tft.fillRect(358, 168, 100, 20, BLUE);
    tft.setCursor(360, 170);
    tft.print(stationId);
    tft.setCursor(260, 210);
    tft.print("Freq   -");
    tft.fillRect(358, 208, 100, 20, BLUE);
    tft.setCursor(360, 210);
    tft.print(qsoFreq);
    tft.setCursor(260, 230);
    tft.print("My Sig -");
    tft.fillRect(358, 228, 100, 20, BLUE);
    tft.setCursor(360, 230);
    tft.print(mystationSignal);
    tft.setTextColor(GREEN, BLUE);
    tft.setCursor(260, 190);
    tft.print("Rx Sig -");
    tft.fillRect(358, 188, 100, 20, BLUE);
    tft.setCursor(360, 190);
    tft.print(stationSignal);
  }
  if (fieldNo == 2)  {
    tft.setTextColor(WHITE, BLUE);
    tft.setCursor(260, 170);
    tft.print("Rx Id  -");
    tft.fillRect(358, 168, 100, 20, BLUE);
    tft.setCursor(360, 170);
    tft.print(stationId);
    tft.setCursor(260, 190);
    tft.print("Rx Sig -");
    tft.fillRect(358, 188, 100, 20, BLUE);
    tft.setCursor(360, 190);
    tft.print(stationSignal);
    tft.setCursor(260, 230);
    tft.print("My Sig -");
    tft.fillRect(358, 228, 100, 20, BLUE);
    tft.setCursor(360, 230);
    tft.print(mystationSignal);
    tft.setTextColor(GREEN, BLUE);
    tft.setCursor(260, 210);
    tft.print("Freq   -");
    tft.fillRect(358, 208, 100, 20, BLUE);
    tft.setCursor(360, 210);
    tft.print(qsoFreq);
  }
  if (fieldNo == 3)  {
    tft.setTextColor(WHITE, BLUE);
    tft.setCursor(260, 170);
    tft.print("Rx Id  -");
    tft.fillRect(358, 168, 100, 20, BLUE);
    tft.setCursor(360, 170);
    tft.print(stationId);
    tft.setCursor(260, 190);
    tft.print("Rx Sig -");
    tft.fillRect(358, 188, 100, 20, BLUE);
    tft.setCursor(360, 190);
    tft.print(stationSignal);
    tft.setCursor(260, 210);
    tft.print("Freq   -");
    tft.fillRect(358, 208, 100, 20, BLUE);
    tft.setCursor(360, 210);
    tft.print(qsoFreq);
    tft.setTextColor(GREEN, BLUE);
    tft.setCursor(260, 230);
    tft.print("My Sig -");
    tft.fillRect(358, 228, 100, 20, BLUE);
    tft.setCursor(360, 230);
    tft.print(mystationSignal);

  }

}
void writeLoggingqso()
{
  sprintf(qsoEndtime, "%i:%i:%i", myHour, myMinute, mySecond);
#ifdef DEBUG 1
  Serial.print(rtc.getDateStr(FORMAT_SHORT, FORMAT_MIDDLEENDIAN, '/'));
  Serial.print(",");
  Serial.print(qsoStarttime);
  Serial.print(",");
  Serial.print(qsoEndtime);
  Serial.print(",");
  Serial.print(qsoFreq);
  Serial.print(",");
  Serial.print(qsoMode);
  Serial.print(",");
  Serial.print(qsoPower);
  Serial.print(",");
  Serial.print(stationId);
  Serial.print(",");
  Serial.print(stationSignal);
  Serial.print(",");
  Serial.println(mystationSignal);
  Serial.println();
#endif
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("qsolog.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print(rtc.getDateStr(FORMAT_SHORT, FORMAT_MIDDLEENDIAN, '/'));
    myFile.print(",");
    myFile.print(qsoStarttime);
    myFile.print(",");
    myFile.print(qsoEndtime);
    myFile.print(",");
    myFile.print(qsoFreq);
    myFile.print(",");
    myFile.print(qsoMode);
    myFile.print(",");
    myFile.print(qsoPower);
    myFile.print(",");
    myFile.print(stationId);
    myFile.print(",");
    for (int i = 0; i < STATIONSIGNALSIZE; i++) {
      myFile.print(stationSignal[i]);
    }
    myFile.print(",");
    for (int i = 0; i < STATIONSIGNALSIZE; i++) {
      myFile.print(mystationSignal[i]);
    }
    myFile.println();
    // close the file:
    myFile.close();


    for (int i = 0; i < STATIONIDSIZE; i++) {             // clear my input fields
      stationId[i] = '\0';
    }
    for (int i = 0; i < STATIONSIGNALSIZE; i++) {
      stationSignal[i] = '\0';
    }
    for (int i = 0; i < QSOFREQSIZE; i++) {
      qsoFreq[i] = '\0';
    }
    for (int i = 0; i < STATIONSIGNALSIZE; i++) {
      mystationSignal[i] = '\0';
    }
    stationIdctr = 0;
    stationSignalctr = 0;
    mystationSignalctr = 0;
    qsoFreqctr = 0;
    clearCursor();
#ifdef DEBUG 1
    Serial.println("done.");
#endif
  } else {
    // if the file didn't open, print an error:
#ifdef DEBUG 1
    Serial.println("error opening test.txt");
#endif
  }


}
void sendTnx4call()
{
  char responSe[128];

  strcpy(responSe, stationId);
  strcat(responSe, qsoMessage1); //DE WD9GYM GM TNX CALL UR
  strcat(responSe, stationSignal);
  strcat(responSe, qsoMessage2);  // HR IN SKOKIE IL SKOKIE IL BT NAME IS MARTY MARTY HW CPY? ";
  strcat(responSe, stationId);
  strcat(responSe, qsoMessage3); // DE WD9GYM KN
  BufferAdd(responSe);

}

void sendgoodBye()
{
  char responSe[128];
  strcpy(responSe, stationId);
  strcat(responSe, qsoMessage4);  // DE WD9GYM TNX FER FB QSO HP CU AGN BT 73 TO U
  strcat(responSe, stationId);
  strcat(responSe, qsoMessage5);  // DE WD9GYM K
  BufferAdd(responSe);
}
