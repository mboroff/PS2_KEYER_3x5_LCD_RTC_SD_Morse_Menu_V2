void mainMenu()
{
#ifdef DEBUG 1
Serial.println("Main menu");
#endif
boolean endInput = false;               // default not done
  optionsGreen();

  while (endInput == false) { 
   char ch;
   ch = ' ';
   boolean doMenu = true;
   while (doMenu == true) {
   while (kbd.available()) {
      ch=kbd.read();
#ifdef DEBUG 1            
Serial.print("Main menu ch = "); Serial.println(ch, DEC);            
#endif
      switch (ch) {        
        case PS2_ENTER:              // Enter = done
            endInput = true;
            optionsWhite();
            break;
        case '1':                     // Choice 1 - QSO RCV Info
            inputFieldno = 0;
            logQSOrcv();             // call recieve log info
            endInput = true;
            doMenu = false;
            optionsWhite();
            break;
        case '2':                    // Choice 2 - Send TNX
            sendTnx4call();
            endInput = true;
            doMenu = false;
            optionsWhite();
            break;
        case '3':                    // Choice 3 - My Sig Info
            inputFieldno = 3;
            logQSOrcv();             // call recieve log info
            endInput = true;
            doMenu = false;
            optionsWhite();
            break;
        case '4':
            if (sdDisable == false) {       
                if (stationIdctr == 0) {
                    tft.setCursor(330, 298);
                    tft.print("NO Rx Id");
                    delay(3000);
                    tft.fillRect(329, 298, 140, 16, BLUE);
                    optionsWhite();
                    endInput = true;
                    doMenu = false;
                    break;
                } else if (stationSignalctr == 0) {
                           tft.setCursor(330, 298);
                           tft.print("NO Rx Sig");
                           delay(3000);
                           optionsWhite();
                           tft.fillRect(329, 298, 140, 16, BLUE);
                           endInput = true;
                           doMenu = false;
                           break;
                } else if (qsoFreqctr == 0) {
                           tft.setCursor(330, 298);         
                           tft.print("NO Freq");
                           delay(3000);
                           optionsWhite();
                           tft.fillRect(329, 298, 140, 16, BLUE);
                           endInput = true;
                           doMenu = false;
                           break;
                } else if (mystationSignalctr == 0) {
                           tft.setCursor(330, 298);
                           tft.print("NO My Sig");
                           delay(3000);
                           optionsWhite();
                           tft.fillRect(329, 298, 140, 16, BLUE);
                           endInput = true;
                           doMenu = false;
                           break;
                } else {
                           tft.setCursor(200, 298);
                           tft.print("Send CW Goodbye? Y or N");        
                           boolean yNinput = true;
                           while(yNinput == true) {
                           while (kbd.available()) {
                                  ch=kbd.read();
                                  if (ch == 'y') {
                                      sendgoodBye();
                                      yNinput = false;
                                  } else if (ch == 'n') {
                                      yNinput = false;
                                  }
                           }
                           }
                           tft.fillRect(199, 298, 277, 16, BLUE);       
                           tft.setCursor(330, 298);
                           tft.print("Saving QSO");        
                           writeLoggingqso();
                           delay(3000);
                           tft.fillRect(329, 298, 140, 16, BLUE);
                           tft.setCursor(330, 298);
                           tft.print("Saved");
                           delay(3000);                          
                           tft.fillRect(329, 298, 140, 16, BLUE);
                           optionsWhite();
                }  
            } else {
                    tft.setCursor(330, 298);              
                    tft.print("SD disabled!");
                    delay(3000);
                    tft.fillRect(329, 298, 140, 16, BLUE);
                    optionsWhite();
            }
            endInput = true;
            doMenu = false;
            break;
        case '5':
            setClock();
            endInput = true;
            doMenu = false;
            optionsWhite();
            break;
       case 's':
            optionsWhite();
            BufferAdd("CQ CQ CQ DE WD9GYM WD9GYM WD9GYM K\r\n");
            endInput = true;
            doMenu = false;
            break;
 
        case 't':
            tft.setCursor(330, 298);
            tft.print("Tuning");
            digitalWrite(PS2KEYERPIN, HIGH);
            delay(3000);
            digitalWrite(PS2KEYERPIN, LOW);
            optionsWhite();
            tft.fillRect(329, 298, 140, 16, BLUE);
            endInput = true;
            doMenu = false;
            break;
         case PERCENTKEY:
            endInput = true;
            doMenu = false;
            break;          
        case PS2_ESC:
            endInput = true;
            doMenu = false;
            break;          

        default:
          break;
          }

      }
   }
  }
#ifdef DEBUG 1
Serial.println("Returning from main menu");
#endif
}

