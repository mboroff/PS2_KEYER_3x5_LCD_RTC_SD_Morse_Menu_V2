void DisplayDateAndTime()
{
  digitalWrite(10, HIGH);
  if (prevDay != myMonthDay) {
      prevDay = myMonthDay;
      tft.fillRect(48, 89, 100, 16, BLUE);
  }

  tft.setCursor(50, 90);
  tft.print(rtc.getDateStr(FORMAT_SHORT, FORMAT_MIDDLEENDIAN, '/'));
     
  tft.setCursor(320, 90);   
  workYear = myYear;
  if (checkLeapYear(myYear)) {
      daysInmonth[1] = 29;
  }
  workMonth = myMonth;
  workDay = myMonthDay;
  if (prevLocalDay != workDay) {
      prevLocalDay = workDay; 
      tft.fillRect(318, 89, 105, 16, BLUE);
  }
  tft.setCursor(320, 90);
  if (localHour > myHour) {       // UTC is next day so let us adjust the date
      workDay = myMonthDay - 1;     //    display to previous day put in leap year later
      if (workDay < 1) {          // change table from static and adjust with get NTP
          workMonth = myMonth - 1;
          if (workMonth < 0) {
              workMonth = 11;
          }
          workDay = daysInmonth[workMonth-1];
      }
      
      if (workMonth < 10) {
          tft.print("0");
      }
      tft.print(workMonth);
      tft.print("/");

      if (workDay < 10) {
          tft.print("0");

      }
      tft.print(workDay);
      tft.print("/");
      tft.print(workYear- 2000);

  }
  else {
     tft.print(rtc.getDateStr(FORMAT_SHORT, FORMAT_MIDDLEENDIAN, '/'));
  }
  
  // Get time
  tft.setCursor(50, 70);                 // print UTC time
  tft.print(rtc.getTimeStr());
  tft.setCursor(320, 70);                 // print UTC time
  localHour = myHour - utcOffset;  
  
  //   the adjustment following is for DST good till 2099
  
  int beginDSTDay = (14 - (1 + myYear * 5 / 4) % 7);  
  int beginDSTMonth=3;
  int endDSTDay = (7 - (1 + myYear * 5 / 4) % 7);
  int endDSTMonth=11;
  if (((workMonth > beginDSTMonth) && (workMonth < endDSTMonth))
   || ((workMonth == beginDSTMonth) && (workDay > beginDSTDay))
   || ((workMonth == beginDSTMonth) && (workDay == beginDSTDay) && (myHour >= 2))
   || ((workMonth == endDSTMonth) && (workDay < endDSTDay))
   || ((workMonth == endDSTMonth) && (workDay == endDSTDay) && (myHour < 1))) {
        localHour++;
        }
 
  if (localHour < 0) {
      localHour = localHour +24;
  }
  workHour = localHour;            // print local time
  
  if (workHour == 0) {                 // adjust hour for 12 hour format
      workHour = 12;
  }
  if (workHour > 12) {
      workHour = workHour - 12;
  }
  if (workHour < 10) {
      tft.print("0");
  }
  tft.print(workHour);
  tft.print(":");

  if (myMinute < 10) {
      tft.print("0");
  }
  tft.print(myMinute);
  tft.print(":");

  if (mySecond < 10) {
      tft.print("0");
  }
  tft.print(mySecond);

}/* --(end main loop )-- */

boolean checkLeapYear(int yearRequest){              // check for leap year
 if ( yearRequest%4 == 0 && yearRequest%100 != 0 || yearRequest % 400 == 0 ) return true;
     else  return false;
}

