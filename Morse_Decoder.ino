void decoderStartup() {

 // memcpy(mySet, "##TEMNAIOGKDWRUS##QZYCXBJP#L#FVH09#8###7#####/-61#######2###3#45", 128);
  justDid = true; // Makes sure we only print one space during long gaps
  characterDone = true; // A full character has been sent
  ditOrDah = true;  // We have either a full dit or a full dah

  // The following values will auto adjust to the sender's speed
  averageDah = ditSpeed * 3;     // A dah should be 3 times as long as a dit
  dit2 = ditSpeed;             // We start by defining a dit as 80 milliseconds
  realDit = dit2;
  myBounce = 2;        // Used as a short delay between key up and down
  myNum = 0;           // We will turn dits and dahs into a binary number stored here
  downTime = 0;        // How long the tone was on in milliseconds
  upTime = 0;          // How long the tone was off in milliseconds
  startDownTime = 0;  // Arduino's internal timer when tone first comes on
  startUpTime = 0;    // Arduino's internal timer when tone first goes off

  memset(lcdBuffer2, '\0', LCDCOLUMNS + 1);

}


/*****
  This function is called when the uC senses the start of a Morse character. There are a number of
  global variables set here.
  
  Parameter list:
    void
    
  Return value:
    void

*****/

 void keyIsDown() {
   // The decoder is detecting our tone
   // The LEDs on the decoder and Arduino will blink on in unison
   digitalWrite(LEDPIN,1);            // turn on Arduino's LED
     
   if (startUpTime > 0){
     // We only need to do once, when the key first goes down
     startUpTime = 0;    // clear the 'Key Up' timer
   }
   if (startDownTime == 0) {
     startDownTime = millis();  // get Arduino's current clock time
   }
 
   characterDone = false; // we're still building a character
   ditOrDah = false;      // the key is still down we're not done with the tone
   delay(myBounce);     // Take a short breath here
     
   if (myNum == 0) {      // myNum will equal zero at the beginning of a character
      myNum = 1;          // This is our start bit - it only does this once per letter
   }
 }

/*****
  This function is called when the uC senses the end of a Morse character by the absence of an
  audio tone. Again, there are a number of global variables set here.
  
  Parameter list:
    void
    
  Return value:
    void

*****/
 
void keyIsUp() {
int farnsworthCutoff;

  // If we haven't already started our timer, do it now
  if (startUpTime == 0){
    startUpTime = millis();
  }

  // Find out how long we've gone with no tone
  upTime = millis() - startUpTime;


  // If it is twice as long as a dah print a space
#ifdef DEBUG1
  Serial.print("upTime = ");
  Serial.print(upTime);
  Serial.print(" dit = ");
  Serial.print(dit);
  Serial.print(" realDit = ");
  Serial.print(realDit);
  Serial.print(" aveDah = ");
  Serial.print(averageDah);
  Serial.print(" myNum = ");
  Serial.print(myNum);
  Serial.print(" lcdGuy = ");
  Serial.print(lcdGuy);
  Serial.print(" m[] = ");
  Serial.print(mySet[myNum]);
  Serial.print("  farn = ");
  Serial.println(farnsworthCutoff);
#endif
  if (upTime < FALSEREAD)              // Static?
    return;

  if (realDit < 90) {
    farnsworthCutoff = averageDah * 2;
  } 
  else {
    if (realDit > 90 && realDit < 105) {
      farnsworthCutoff = averageDah + realDit;
    } 
    else {
      farnsworthCutoff = averageDah * 3;
    }
  }
#ifdef DEBUG1
  Serial.println("");
  Serial.print("upTime = ");
  Serial.print(upTime);
  Serial.print("   dit = ");
  Serial.print(dit);
  Serial.print("   realDit = ");
  Serial.print(realDit);
  Serial.print("   avedah = ");
  Serial.print(averageDah);
  Serial.print("   farn = ");
  Serial.print(farnsworthCutoff);
  Serial.print("  lcdGuy = ");
  Serial.println(lcdGuy);
#endif

  //if (upTime > (averageDah * 2)) 
  if (upTime > (realDit * 3)) {         // ***** PROBLEM AREA
    //if (upTime > farnsworthCutoff) 
    //if (upTime > dit) {
    printSpace();
  }

  // Only do this once after the key goes up
  if (startDownTime > 0){
    downTime = millis() - startDownTime;  // how long was the tone on?
    startDownTime = 0;                    // clear the 'Key Down' timer
  }

  if (!ditOrDah) {   // We don't know if it was a dit or a dah yet
    shiftBits();    // let's go find out! And do our Magic with the bits
  }

  // If we are still building a character ...
  if (!characterDone) {       // Are we done yet?
#ifdef DEBUG1
    Serial.print("upTime = ");
    Serial.print(upTime);
    Serial.print("   dit = ");
    Serial.print(dit);
    Serial.print("   realDit = ");
    Serial.print(realDit);
    Serial.print("   myNum = ");
    Serial.print(myNum);
    Serial.print("   lcdGuy = ");
    Serial.print(lcdGuy);
    Serial.print("   m[] = ");
    Serial.println(mySet[myNum]);
#endif

    //if (upTime > dit)  
    if (upTime > realDit) {      // BINGO! we're done with this one  
      printCharacter();         // Go figure out what character it was and print it       
      characterDone = true;     // We got him, we're done here
      myNum = 0;                // This sets us up for getting the next start bit
    }
    downTime = 0;                // Reset our keyDown counter
  
  }
    
  
  
}
   
void shiftBits() {
  // we know we've got a dit or a dah, let's find out which
  // then we will shift the bits in myNum and then add 1 or not add 1
  
  if (downTime < dit2 / 3) 
    return;                // ignore QRN - UnREM this line if your have a real noisy receiver
                           // it will limit the top speed that you can copy.
                          
  myNum = myNum << 1;    // shift bits left
  ditOrDah = true;       // we will know which one in two lines 
 
  // If it is a dit we add 1. If it is a dah we do nothing!
  if (downTime < dit2) {
     myNum++;           // add one because it is a dit
     realDit = (downTime + realDit) / 2;
 } else { 
    // The next three lines handle the automatic speed adjustment:
    averageDah = (downTime + averageDah) / 2;  // running average of dahs
    dit2 = averageDah / 3;                      // normal dit would be this
    realDit = dit2;                             // Track this for timing
    dit2 = dit2 * 2;                             // double for threshold between dits and dahs
  }
}


void printCharacter() {           
  justDid = false;         // OK to print a space again after this
  
  // Punctuation marks will make a BIG myNum
  if (myNum > 63) {  
    printPunctuation();  // The value we parsed is bigger than our character array
                         // It is probably a punctuation mark so go figure it out.
    return;              // Go back to the main loop(), we're done here.
  }
  lcdGuy = mySet[myNum]; // Find the letter in the character set
  if (lcdGuy != '#')
      DoMyLCD();           // Go figure out where to put in on the display
}

void printSpace() {
  if (justDid) {
    return;  // only one space, no matter how long the gap
  }
  
  justDid = true;       // so we don't do this twice  
  lcdGuy = ' ';            // this is going to go to the LCD 
  DoMyLCD();         // go figure out where to put it on the display

}

/*****
  Punctuation marks are made up of more dits and dahs than letters and numbers. 
  Rather than extend the character array out to reach these higher numbers we 
  will simply check for them here. This funtion only gets called when myNum is 
  greater than 63.

  Parameter List:
    void
    
  Return value:
    void
*****/
void printPunctuation() {
  switch (myNum) {
    case 71:
      lcdGuy = ':';
      break;
    case 76:
      lcdGuy = ',';
      break;
    case 84:
      lcdGuy = '!';
      break;
    case 94:
      lcdGuy = '-';
      break;
    case 97:
      lcdGuy = 39;    // Apostrophe
      break;
    case 101:
      lcdGuy = '@';
      break;
    case 106:
      lcdGuy = '.';
      break;
    case 115:
      lcdGuy = '?';
      break;
    case 246:
      lcdGuy = '$';
      break;
    case 122:
      lcdGuy = 's';
      DoMyLCD();
      lcdGuy = 'k';
      break;
    default:
      lcdGuy = '#';    // Should not get here
      break;
  }
 
  DoMyLCD();    // go figure out where to put it on the display
}

/*****
  This function moves the current character read to the LCD display. It assumes that
  lcdGuy has been set prior to calling this function.
  
  Parameter list:
    void
    
  Return value:
    void
    
*****/
void DoMyLCD() {

  if (passCounter % 100 == 0) {
    wpm = 1200 / realDit + (realDit * .06);
    itoa(wpm, numBuff, 10);
    strcpy(outBuff, "WPM = ");
    strcat(outBuff, numBuff);
    strcat(outBuff, " ");
    tft.setCursor(330, 276);
    tft.print(outBuff);
#ifdef DEBUG                         // my debug routine
Serial.println(outBuff);
#endif
    passCounter = 0;
  }
  passCounter++;
  memcpy(lcdBuffer2, &lcdBuffer2[1], LCDCOLUMNS - 1);
  lcdBuffer2[LCDCOLUMNS - 1] = lcdGuy;
  tft.setCursor(80, 276);
  tft.print(lcdBuffer2);
#ifdef DEBUG                       // my debug routine
  Serial.println(lcdBuffer2);
#endif
}




