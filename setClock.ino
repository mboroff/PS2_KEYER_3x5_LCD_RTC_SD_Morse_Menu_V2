void setClock()
{
int promptNo = 0;  
boolean gettingTime = false;          
  DisplayDateAndTime();
  tft.setTextColor(GREEN, BLUE); 
  tft.setCursor(10, 230);
  tft.print("5 - Set UTC Clock");
  minuteInbufferHead = 0;
  minuteINbufferTail = 0;             
  monthInbufferHead = 0;
  monthINbufferTail = 0;             
  dayInbufferHead = 0;
  dayINbufferTail = 0;             
  yearInbufferHead = 0;
  yearINbufferTail = 0;             

  for (int i = 0 ; i < HOURIN; i++) {
       myHourin[i] = ' ';
  }
  for (int i = 0 ; i < MINUTEIN; i++) {
       myMinutein[i] = ' ';
  }
  myMonthin[0] = '/0';
  myDayin[0] = '/0';
  myYearin[0] = '/0';

  tft.setCursor(120, 298);
  tft.print("UTC Hour = ");
  while (gettingTime == false) { 
   char ch;
   while (kbd.available()) {
      switch (ch=kbd.read()) {

        case PS2_EXCLAMATION:
             return;
             break;
        case PS2_ENTER:              // Start buffering without transmitting
            tft.fillRect(80,296, 349, 20, BLUE);
            promptNo++;
            if (promptNo == 1) {       // prompt for signal report
                tft.fillRect(80,296, 349, 20, BLUE);
                tft.setCursor(120, 298);
                tft.print("Minute = ");
                break;
            }
            if (promptNo == 2) {       // prompt for signal report
                tft.fillRect(80,296, 349, 20, BLUE);
                tft.setCursor(120, 298);
                tft.print("Month = ");
                break;
            }
            if (promptNo == 3) {       // prompt for signal report
                tft.fillRect(80,296, 349, 20, BLUE);
                tft.setCursor(120, 298);
                tft.print("Day = ");
                break;
            }
            if (promptNo == 4) {       // prompt for signal report
                tft.fillRect(80,296, 349, 20, BLUE);
                tft.setCursor(120, 298);
                tft.print("Year = ");
                break;
            }

            if (promptNo > 4) {
               gettingTime = true;
               myMonth = atoi(myMonthin);
               myMonthDay = atoi(myDayin);
               myYear = atoi(myYearin);
               rtc.setDate(myMonthDay, myMonth, myYear);
               myHour = atoi(myHourin);
               myMinute = atoi(myMinutein);
               rtc.setTime(myHour, myMinute, 0);
            }

          break;
        default:
//          char ch2 = toupper(ch);
          tft.print(ch);

          if (promptNo == 0) {
              myHourin[hourInbufferHead++] = ch;
              hourINbufferTail &= HOURINMASK;             
#ifdef DEBUG
              Serial.println(myHourin);
#endif
              break;
          }

          if (promptNo == 1) {
              myMinutein[minuteInbufferHead++] = ch;
              minuteINbufferTail &= MINUTEINMASK;             
#ifdef DEBUG                
              Serial.println(myMinutein);
#endif              
              break;
          }
          if (promptNo == 2) {
              myMonthin[monthInbufferHead++] = ch;
              monthINbufferTail &= MONTHINMASK;          
#ifdef DEBUG                
              Serial.println(myMonthin);
#endif              
              break;
          }
          if (promptNo == 3) {
              myDayin[dayInbufferHead++] = ch;
              dayINbufferTail &= DAYINMASK;             
#ifdef DEBUG  
              Serial.println(myDayin);
#endif              
              break;
          }
          if (promptNo == 4) {
              myYearin[yearInbufferHead++] = ch;
              yearINbufferTail &= YEARINMASK;      
#ifdef DEBUG                
              Serial.println(myYearin);
#endif              
              break;
          }

      }
   }
  }
#ifdef DEBUG    
  Serial.println("returning");
#endif  
  tft.setTextColor(WHITE, BLUE); 
  tft.setCursor(10, 230);
  tft.print("5 - Set UTC Clock");

}
