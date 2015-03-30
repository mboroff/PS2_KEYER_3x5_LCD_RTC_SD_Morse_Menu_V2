/***************************************************************
  This sketch was put together using the example from the SD library
  the DS3231 library, the PS2 Keyer and Morse Code Decoder.
  
  I created a menu system, on a 20X4 LCD, to collect data for QSO logging and 
  additional features so you do not have to remember what special keys arepress.
  
  1 - QSO RCV Info       5 - Write QSO Log      9 - Send CQ 
  2 - Send TNX           6 - Set UTC Clock      A -
  3 - My Sig Info        7 - Toggle KB Disply   B - Prev
  4 - Next               8 - Next               C - Next

***************************************************************/

/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10
 
 created   Nov 2010
 by David A. Mellis
 updated 2 Dec 2010
 by Tom Igoe
 
 - Cleaned up all SD examples included with MPIDE for consistency in defining CS pins
 revised  24 May 2013 by Jacob Christ

 This example code is in the public domain.
 	 
 */

/*
  Based on code written by Mark VandeWettering, K6HX, as found on his web site:
 
 http://brainwagon.org/2012/01/21/an-arduino-powered-ibm-ps2-morse-keyboard/
 
 You need four connections from the PS2 keyboard: 
 
 5V        Connects to pin 4 of the PS2 connector (and to 5v on the Arduino board)
 ground            "       3         "            (and to GND on the Arduino board)
 clock             "       5         "            (and to pin 3 on the Arduino board)
 data              "       1         "            (    "      4         "           )
 
 Pins 2 and 6 of the PS2 connector are not used. 
 
 */

// Version for the PS2 Keyboard
// using the library from http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html
// 
/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <Wire.h>  // Comes with Arduino IDE
#include <SD.h>
#include <stdlib.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"

// These are 'flexible' lines that can be changed
#define TFT_CS 31
#define TFT_DC 29
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);


#include <PS2Keyboard.h>

#include <DS3231.h>
#include <Time.h>

#define DEBUG           1    // For debugging statements. Comment out when not debugging
//#define CHARSONLY       1    // Only show characters, not code

#define PS2CLOCKPIN     19
#define PS2DATAPIN      22
#define PS2KEYERPIN     27
#define TONEPIN         26      // Used if you want audio via a small buzzer
#define PS2_EXCLAMATION	33 // ¡
#define LCDROWS     4
#define LCDCOLUMNS  20

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

/***********************************************************
         TFT Colors
***********************************************************/
// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


/*****************************************************************
   Defines for QSO Logging
*****************************************************************/
#define STATIONIDSIZE        8
#define STATIONSIGNALSIZE    3
#define QSOFREQSIZE          8


// Init the DS3231 using the hardware interface will not be able to share
//                 I2C interface with 20X4 LCD
#define rtcSDA 23
#define rtcSCL 25
DS3231  rtc(rtcSDA, rtcSCL);
File myFile;

// On the Ethernet Shield, CS is pin 4. It's set as an output by default.
// Note that even if it's not used as the CS pin, the hardware SS pin 
// (10 on most Arduino boards, 53 on the Mega) must be left as an output 
// or the SD library functions will not work. 

// Default SD chip select for Uno and Mega type devices
const int chipSelect_SD_default = 49; // Change 10 to 53 for a Mega

// chipSelect_SD can be changed if you do not use default CS pin
const int chipSelect_SD = chipSelect_SD_default;
#define SD_CS 49
/*-----( Declare Variables )-----*/

#define LEDPIN        13
#define RESETSWITCHPIN 8

#define IDTIMESLICE      600000  // 10 minutes. Change to shorter period for debugging...
#define IDTIMESLICE      10000    // For debugging, comment one above out...
#define HALFSECOND       500
#define DEBOUNCEDELAY    20
#define ERRORREPEAT      6       // The number of times to repeat blinking error LED indicator
#define FLIPDELAY        1       // Alternates LCD asterisk after this many seconds
#define MODESWITCHDELAY  5000    // If they hold the push botton longer than 5 seconds, activeTimer
                                 // changes state.
                                 
#define MILLENIA         2000
Time t; // structure for time

boolean activeTimer;        // Holds state of timer: 1 = active, 0 = inactive
long millisStart, millisCurrent;
unsigned long currentTime;
String myTime;

int daysInmonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define pF(string_pointer) (reinterpret_cast<const __FlashStringHelper *>(pgm_read_word(string_pointer)))

char   janStr[]  = "Jan";
char   febStr[]  = "Feb";
char   marStr[]  = "Mar";
char   aprStr[]  = "Apr";
char   mayStr[]  = "May";
char   junStr[]  = "Jun";
char   julStr[]  = "Jul";
char   augStr[]  = "Aug";
char   sepStr[]  = "Sep";
char   octStr[]  = "Oct";
char   novStr[]  = "Nov";
char   decStr[]  = "Dec"; 

char*  monthStrs[] = { janStr, febStr, marStr, aprStr, mayStr, junStr,
                                          julStr, augStr, sepStr, octStr, novStr, decStr}; 


const int LOCALUTCOFFSET = 6;   // CST during non-dst.


int myMonth = 0, myMonthDay = 0, myYear = 0;     // used for setDate
int myHour = 0, myMinute = 0, mySecond = 0;      // used for setTime
int prevSecond = 0;
int prevDay = 0;
int prevLocalDay = 0;
int prevDisplayHour = 0;
int prevDisplayMinute = 0;
int prevDisplaySecond = 0;
int prevDisplayLocalHour = 0;
int prevDisplayLocalMinute = 0;
int prevDisplayLocalSecond = 0;
int localHour;
int localOffset = 6;      // CST UTC offset non DST
char wrkMonth[4] = "   ";                        // temp work values
char wrkMonthday[3] = "  ";
char wrkYear[5] = "    ";
char wrkHour[3] = " ";
char wrkMinute[3] = "  ";
char wrkSecond[3] = "  ";
int workMonth, workDay, workYear, workHour;
int searchIndex = 0, foundIndex = 0;
String foundValue = "";

boolean dateFlipped = false;

#define SIDETONEFREQ    700
#define ESCAPEKEY        27
#define PERCENTKEY       37
#define NEWLINE         '\n'

#define QUEUESIZE        128

#define HOURIN    3
#define MINUTEIN  3
#define MONTHIN       3
#define DAYIN         3
#define YEARIN        5

#define LCDLINESIZE 20
#define LCDROWSIZE  4

#define QUEUEMASK (QUEUESIZE-1)
#define HOURINMASK (HOURIN - 1)
#define MINUTEINMASK (MINUTEIN - 1)
#define DAYINMASK (DAYIN-1)
#define MONTHINMASK (MONTHIN-1)
#define YEARINMASK (YEARIN-1)

#define DEFAULTWPM	15	// MIN_WPM <= DEFAULT_WPM <= MAX_WPM
#define	MINWPM		 5
#define MAXWPM		50
#define NOCHARAVAILABLE -1
// ========================= Global data definitions =====================

/*********************************************************
             From PS2 Keyer
*********************************************************/
//int longMessageProcessing = 0;    // Used when buffering keystrokes and not sending them
char buffer[QUEUESIZE];

int aborted = 0;
int bufferHead = 0;
int bufferTail = 0;             

int wordsPerMinute = DEFAULTWPM;         // Default speed
boolean sideTone = false;                // Default is no sidetone
boolean	speedChange = false;	         // 'true' indicates speed change requested

int ditlen = 1200 / wordsPerMinute;

String ReceivingId;

PS2Keyboard kbd;

/*****************************************************************
            From Morse Code Decoder
*****************************************************************/

#define NOSIGNALMAX       10     // Wait for this number of passes through the loop before saying
                                  // there's no more sending going on.

#define AUDIOPIN    A5

#define FALSEREAD   10   // Caused when an audio burst, like QRN, generates a short false signal.
#define LINEEND     (LCDCOLUMNS + 1)
const unsigned long TIMETOSTARTDISPLAY = 10000;   // 10 seconds
unsigned long startTime;

int audioPin = AUDIOPIN;  // Jacks decoder pin
int audio = 1;            // will store the value we read on this pin

int LCDline = 1;          // keeps track of which line we're printing on
int lineEnd = LINEEND;         // One more than number of characters across display
int letterCount = 0;      // keeps track of how may characters were printed on the line
int lastWordCount = 0;    // keeps track of how may characters are in the current word
int lastSpace = 0;        // keeps track of the location of the last 'space'

// The place a letter appears here matches myNum that we parsed out of the code
// #'s are miscopied characters
char mySet[] = "##TEMNAIOGKDWRUS##QZYCXBJP#L#FVH09#8###7#####/-61#######2###3#45";
               
char lcdGuy = ' ';       // We will store the actual character decoded here
char lcdBuffer2[LCDCOLUMNS + 1];

boolean justDid = true; // Makes sure we only print one space during long gaps
boolean characterDone = true; // A full character has been sent
boolean ditOrDah = true;  // We have either a full dit or a full dah

// The following values will auto adjust to the sender's speed
static int passCounter = 0;
int wpm;
char numBuff[5];
char outBuff[17];

int ditSpeed = 80;
int averageDah = 240;     // A dah should be 3 times as long as a dit
int dit2 = 80;             // We start by defining a dit as 80 milliseconds
int realDit = dit2;
int myBounce = 2;        // Used as a short delay between key up and down
int myNum = 0;           // We will turn dits and dahs into a binary number stored here

long downTime = 0;        // How long the tone was on in milliseconds
long upTime = 0;          // How long the tone was off in milliseconds
long startDownTime = 0;  // Arduino's internal timer when tone first comes on
long startUpTime = 0;    // Arduino's internal timer when tone first goes off

/*************************************************************************
      Globals for QSO Logging and other mods
*************************************************************************/
int hourInbufferHead = 0;                  // used to keep track of input info
int hourINbufferTail = 0;                  //  when setting clock
int minuteInbufferHead = 0;
int minuteINbufferTail = 0;             
int monthInbufferHead = 0;
int monthINbufferTail = 0;             
int dayInbufferHead = 0;
int dayINbufferTail = 0;             
int yearInbufferHead = 0;
int yearINbufferTail = 0;             

int utcOffset = 6;                        // <---- CST Offset change for your time zone

int inputFieldno = 0;                     // keep track of which input field
int stationIdctr = 0;                     // being inputted and number of characters inputted
int stationSignalctr = 0;
int mystationSignalctr = 0;
int qsoFreqctr = 0;

uint8_t lcdRightarrow = 126;                 // cursor for input
uint8_t lcdLeftarrow = 127;                 // cursor for input

char lcdBuffer[21] = "                    ";  // working buffer when using an LCD

char myHourin[HOURIN];                         // time and date input fields
char myMinutein[MINUTEIN];
char myMonthin[MONTHIN];
char myDayin[DAYIN];
char myYearin[YEARIN];

char qsoStarttime[9];                          // QSO time stamps
char qsoEndtime[9];

char stationId[STATIONIDSIZE+1];           // define input field one byte larger than size for null byte
char stationSignal[STATIONSIGNALSIZE+1];
String qsoFreq;
char mystationSignal[STATIONSIGNALSIZE+1];

String qsoDate;                                // working area for file write

boolean displayKeyboard = false;              // toggle whether or not to display characters typed in
boolean sdDisable = false;                    // notify sd is not usable
boolean endQSOlogging = false;               // default not logging
boolean endInputstationid = false;          // default not getting station id
boolean endInputstationsignal = false;      // default not getting signal received
boolean endFrequency = false;               // default not getting freq
boolean waitingForPatriot = true;
String receiveFreq;
String ritDisplay;
String txFreq;
String bandDisplay;
String stepDisplay;
String bandWidth;
String modeDisplay;

char smeterValue[5];
char batValue[5];
char powerDisplay[5];
char codeReadValue[5];
char codeSpeedValue[5];




const char qsoMessage1[34] = " DE WD9GYM TNX CALL BT UR RST ";
const char qsoMessage2[59] = " HR IN SKOKIE IL SKOKIE IL BT NAME IS MARTY MARTY HW CPY? ";
const char qsoMessage3[15] = " DE WD9GYM KN "; 
const char qsoMessage4[48] = " DE WD9GYM TNX FER FB QSO HP CU AGN BT 73 TO U ";
const char qsoMessage5[14] = " DE WD9GYM K "; 
const char qsoMode[3] = "CW";                  // default mode and power for qso log
const char qsoPower[8] = "5 watts";

void setup()
{
  pinMode(53, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  pinMode(TONEPIN, OUTPUT);
  pinMode(PS2KEYERPIN, OUTPUT);
  kbd.begin(PS2DATAPIN, PS2CLOCKPIN);
 Serial3.begin(4800);
#ifdef DEBUG  
  Serial.begin(9600);
#endif  
 
/******************************************************
    TFT Begin
*******************************************************/
  Serial.println("HX8357D Test!"); 

  tft.begin(HX8357D);

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(HX8357_RDPOWMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDCOLMOD);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDDIM);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDDSDR);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 

  tft.setRotation(1);
  tft.fillScreen(BLUE);
  tft.setTextColor(WHITE); 
  tft.setTextSize(3);

/*******************************************************
    RTC Begin
*******************************************************/
  rtc.begin();
  
  t = rtc.getTime();                // get the time  
   Serial.println(rtc.getDateStr(FORMAT_SHORT, FORMAT_MIDDLEENDIAN, '/'));
  // that are connected to the SPI bus do not hold drive bus
  // The following lines can be uncommented to set the date and time
 // rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
 // rtc.setTime(4, 9, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(26, 2, 2015);   // Set the date to January 1st, 2014
  tft.setCursor(0, 10);
  tft.println("Initializing SD card...");
#ifdef  DEBUG
 Serial.print("Initializing SD card...");
#endif
  if (!SD.begin(SD_CS)) {
#ifdef DEBUG
    Serial.println("failed!");
#endif
    tft.println("Failed");
    sdDisable = true;
    
  } else {
#ifdef DEBUG  
  Serial.println("OK!");
#endif
   tft.println("Ok");
  }
  
//  DecoderStartup();


  tft.println("PS2 keyboard ready");

#ifdef DEBUG  
  Serial.println("PS2 keyboard ready:");    
 #endif
 if (sdDisable == false) {
   if (SD.exists("qsohdr.csv")) {
     SD.remove("qsohdr.csv");
   }
    myFile = SD.open("qsohdr.csv", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
      myFile.println(",,,,,,Station,,");
      myFile.println("Date    ,Start   ,Finish  ,Freq  ,Mode ,Power,Worked/Called,Sent,Recvd");
     // close the file:
     myFile.close();
  }
 }
  
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
  
  decoderStartup();
  
#ifdef DEBUG 1  
  Serial.println("initialization done.");
#endif  
  tft.println("initialization done.");
 delay(3000);
displaySetup();
displayRadio();

}

void loop()
{
 receivePatriot();
   audio = digitalRead(audioPin); // What is the tone decoder doing?
/*
   if (audio == 0) {
       int passCount = 0;
       upTime = 0;
       while(true) {
           audio = digitalRead(audioPin); // What is the tone decoder doing?
Serial.println(audio);
          if (!audio) {
              keyIsDown();       // LOW, or 0, means tone is being decoded
              passCount = 0;
          }
          if (audio) {
              keyIsUp();          // HIGH, or 1
              passCount++;
          }
#ifdef DEBUG          
Serial.print("in decoder, audio = ");
Serial.print(audio);
Serial.print(" passCount = ");
Serial.println(passCount);
#endif
          if (passCount > NOSIGNALMAX && audio == 1) {  // No more code being received...
              break;
          }
       }
   } else {
*/  
     t = rtc.getTime();                // get the time
           myMonth = t.mon;
           myMonthDay = t.date;
           myYear = t.year;
           myHour = t.hour;
           myMinute = t.min;
           mySecond = t.sec;                 // get the second
           if (prevSecond != mySecond) {
               prevSecond = mySecond;
               DisplayDateAndTime();
           }
   
           ps2poll();              // Loop for a keystroke

           if (bufferHead != bufferTail)      // If there's a keystroke present in the buffer...
           send(BufferPopCharacter());   // ...send it along.
  // }
}


/*****
 * This method adds a character to the buffer.
 * 
 * Parameters:
 * char ch      the character to add
 * 
 * Return value:
 * void
 *****/
void BufferAdd(char ch)
{
  buffer[bufferTail++] = ch;
  bufferTail &= QUEUEMASK;   
}

/*****
 * This method adds a character to the buffer. See text as to how this method shares the same method name as
 * the one above without creating a duplicate definition error
 * 
 * Parameters:
 * char ch      the character to add
 * 
 * Return value:
 * void
 *
 *  CAUTION: If any part of an existing message is in the buufer when this function is called, it is lost.
 *****/
void BufferAdd(char *s)
{

  int len;
  len = strlen(s);
  if (len > QUEUEMASK) { // If the message is too long, kill it.
    *s = '\0';
    return;
  }
  BufferReset();         // Override anything in buffer  
  while (*s)
    BufferAdd(*s++);
}

/*****
 * This method removes a character from the buffer.
 * 
 * Parameters:
 * void
 * 
 * Return value:
 * char        the character that is copied from the buffer
 *****/
char BufferPopCharacter()
{
  char ch;
  ch = buffer[bufferHead++];
  bufferHead &= QUEUEMASK;
  return ch;
}


/*****
 * This method reset the buffer.
 * 
 * Parameters:
 * void
 * 
 * Return value:
 * void
 *****/
void BufferReset()
{
  bufferHead = 0;                // Reset to first character in buffer
  bufferTail = 0;
  memset(buffer, 0, QUEUESIZE);  // Clear out previous contents
}

/*****
 * This method polls the keyboard looking for a keystroke. If a character is available, it is added to the 
 * keyboard input buffer. The inline modifier is a hint to the compiler to generate inline code if possible,
 * to improve the performance of the method.
 * 
 * Parameters:
 * void
 * 
 * Return value:
 * void
 *****/
inline void ps2poll()
{
  char ch;
  while (kbd.available()) {
    if (((bufferTail+1)%QUEUESIZE) == bufferHead) {	// is buffer full ?
#ifdef DEBUG
      Serial.println("== Buffer Full ==");
#endif
      break;
    } else {
      ch=kbd.read();
      switch (ch) {
        case ESCAPEKEY:                               // case '\033':
          BufferReset();
  #ifdef DEBUG
          Serial.flush();
          Serial.println("== Buffer reset ==");
  #endif
          aborted = 1;
          break;
        case PERCENTKEY:
           optionsWhite();
           mainMenu();
           displayRadio();
           updateRadio();
           break;
        case '(':              // Start buffering without transmitting
          DelayedTransmit();
          break;
        case '#':              // Change keying speed
          ChangeSendingSpeed();
          speedChange = true;
          break;
        case '~':              // Change sidetone. Default is no tone (0)
          sideTone = !sideTone;
          break;
        case '*':
          break;
        case ')':
          break;
          /*
          case YOURCHARACTER:
           BufferAdd("Your special message here");
           break;
           */
        default:
          BufferAdd(ch);
          break;
      }
    }
  }
}

/*****
 * This method generates a delay based on the millis() method call. This is the preferred way to perform a
 * delay because it doesn't put the board to sleep during the delay.
 * 
 * Parameters:
 * unsigned ling ms    the number of milliseconds to delay
 * 
 * Return value:
 * void
 *****/
void mydelay(unsigned long ms)
{
  unsigned long t = millis();
  while (millis()-t < ms) {
    ps2poll();
  }
}

/*****
 * This method generates a tone if speakers are hooked to the system. The TONEPIN value determines which pin
 * is used to generate the tone.
 * 
 * Parameters:
 * void
 * 
 * Return value:
 * void
 *****/
void scale()
{
  long f = 220L;
  int i;

  for (i=0; i<=12; i++) {
    tone(TONEPIN, (int)f);
    f *= 1059L;
    f /= 1000L;
#ifdef DEBUG
    Serial.println(f);
#endif
    delay(300);
  }
  noTone(TONEPIN);     
}

/*****
 * This method generates a single dit in the sequence necessary to form a character in Morse Code.
 * 
 * Parameters:
 * void
 * 
 * Return value:
 * void
 *****/
void dit()
{ 

  if (sideTone) {
    digitalWrite(LEDPIN, HIGH);
    digitalWrite(PS2KEYERPIN, HIGH);
    tone(TONEPIN, SIDETONEFREQ);
    mydelay(ditlen);
    digitalWrite(LEDPIN, LOW);
    digitalWrite(PS2KEYERPIN, LOW);
    noTone(TONEPIN);
    mydelay(ditlen);
  } else {
    digitalWrite(PS2KEYERPIN, HIGH);
    mydelay(ditlen);
    digitalWrite(PS2KEYERPIN, LOW);
    mydelay(ditlen);
    
  }

#ifdef CHARSONLY
  return;
#endif

#ifdef DEBUG
  Serial.print(".");
#endif 
}

/*****
 * This method generates a single dah in the sequence necessary to form a character in Morse Code.
 * 
 * Parameters:
 * void
 * 
 * Return value:
 * void
 *****/
void dah()
{
  
  if (sideTone) {
    digitalWrite(LEDPIN, HIGH);
    digitalWrite(PS2KEYERPIN, HIGH);
    tone(TONEPIN, SIDETONEFREQ);
    mydelay(3*ditlen);
    digitalWrite(LEDPIN, LOW);
    digitalWrite(PS2KEYERPIN, LOW);
    noTone(TONEPIN);
    mydelay(ditlen);
  } else {
    digitalWrite(PS2KEYERPIN, HIGH);
    mydelay(3*ditlen);
    digitalWrite(PS2KEYERPIN, LOW);
    mydelay(ditlen);
  }
#ifdef CHARSONLY
  return;
#endif
#ifdef DEBUG
  Serial.print("_");
#endif
}


// The coded byte values for the letters of the alphabet. See text for explanation
char ltab[] = {
  0b101,              // A
  0b11000,            // B 
  0b11010,            // C
  0b1100,             // D
  0b10,               // E
  0b10010,            // F
  0b1110,             // G
  0b10000,            // H
  0b100,              // I
  0b10111,            // J
  0b1101,             // K
  0b10100,            // L
  0b111,              // M
  0b110,              // N
  0b1111,             // O
  0b10110,            // P
  0b11101,            // Q
  0b1010,             // R
  0b1000,             // S
  0b11,               // T
  0b1001,             // U
  0b10001,            // V
  0b1011,             // W
  0b11001,            // X
  0b11011,            // Y
  0b11100             // Z
};

// The coded byte values for numbers. See text for explanation

char ntab[] = {
  0b111111,           // 0
  0b101111,           // 1
  0b100111,           // 2
  0b100011,           // 3
  0b100001,           // 4
  0b100000,           // 5
  0b110000,           // 6
  0b111000,           // 7
  0b111100,           // 8
  0b111110            // 9
};

/*****
 * This method generates the necessary dits and dahs for a particular code.
 * 
 * Parameters:
 * char code    the byte code for the letter or number to be sent as take from the ltab[] or ntab[] arrays.
 * 
 * Return value:
 * void
 *****/
void sendcode(char code)
{
  int i;

  for (i=7; i>= 0; i--) {  // Look for start bit
    if (code & (1 << i))
      break;
  }
  for (i--; i>= 0; i--) {  // Remaining bits are the actual Morse code
    if (code & (1 << i))
      dah();
    else
      dit();
  }
  mydelay(2*ditlen);	// space between letters
#ifdef DEBUG
  Serial.print("");
#endif
}

/*****
 * This method translates and sends the character.
 * 
 * Parameters:
 * char ch      the character to be translated and sent
 * 
 * Return value:
 * void
 *****/
void send(char ch)
{

  int index;
  
/************************************************
    mod for ps2 keyboard display
************************************************/
//  if (displayKeyboard == true) {  
      for (int i = 0; i < 20; i++) { // loop through lcdbuffer moving characters
           lcdBuffer[i] = lcdBuffer[i+1];
      }
      if (ch != '\r' && ch != '\n') {
          lcdBuffer[19] = toupper(ch);
      } else {
          lcdBuffer[19] = ' ';
       }
      tft.setCursor(80, 298);
      tft.print(lcdBuffer);
//  }
/************************************************
   endkg mod for ps2 keyboard display
************************************************/  
  
  if (speedChange) {	            // use new speed
    ditlen = 1200 / wordsPerMinute;		
    speedChange = false;
  }

  if (isalpha(ch)) {
    index = toupper(ch) - 'A';     // Calculate an index into the letter array if a letter...
    sendcode(ltab[index]);
  } else if (isdigit(ch)) {
    sendcode(ntab[ch-'0']);        // Calculate an index into the numbers table if a number...
  } else if (ch == ' ' || ch == '\r' || ch == '\n') {
    mydelay(4*ditlen);
#ifdef DEBUG
    switch (ch) {
      case ' ':
        Serial.print(" ");
        break;
      default:
        Serial.println("");
        break;
    }
#endif
  } else {
    switch (ch) {                  // Punctuation and special characters. NOTE; Tree depth is 6, so
      case '.':                    // characters max out at 7 dit/dah combinations
        sendcode(0b1010101);
        break;
      case ',':
        sendcode(0b1110011);
        break;
      case  '!':
        sendcode(0b1101011);
        break;
      case  '?':
        sendcode(0b1001100);
        break;
      case  '/':
        sendcode(0b110010);
        break;
      case  '+':
        sendcode(0b101010);
        break;
      case  '-':
        sendcode(0b1100001);
        break;
      case  '=':
        sendcode(0b110001);
        break;
      case  '@':               
        sendcode(0b1011010);
        break;
      case '\'':               // ' apostrophe
        sendcode(0b1011110);
        break; 
      case '(': 
        sendcode(0b110110); 
        break;
      case ')': 
        sendcode(0b1101101); 
        break;
      case ':': 
        sendcode(0b1111000); 
        break;
      case ';': 
        sendcode(0b1101010); 
        break;
      case '"': 
        sendcode(0b1010010); 
        break;
       
      default:
        break;
      }
  }

#ifdef DEBUG
  if (!aborted) {
    Serial.println(ch);
    if (ch == 13) 
      Serial.print((char) 10);
  }
#endif
  aborted = 0;
}



/*****
 * This method flashes the LED on pin 13 if the input buffer is close to being full.
 *
 * Parameters:
 *   void
 * 
 * Return value:
 *   void
 *****/

void FlashBufferFullWarning()
{
  int i;
#ifdef DEBUG
  Serial.print("************* Approaching buffer full ==============");
  //Serial.println(longMessageProcessing);
#endif

  for (i = 0; i < 10; i++) {
    digitalWrite(LEDPIN, HIGH);  // Visual
    delay(100);
    digitalWrite(LEDPIN, LOW);
    delay(100);
    tone(TONEPIN, SIDETONEFREQ);         // Audio...if available
    delay(100);
    noTone(TONEPIN);
    delay(100);
  }
}

/*****
 * This method buffer all keystrokes after reading the leading '(' until it reads a ')'. At that
 * time, the buffer is sent to the transmitter.
 * 
 * Parameters:
 *   void
 * 
 * Return value:
 *   int          the number of characters buffered.
 *****/

int DelayedTransmit()
{
  char ch;
  int charCount = 0;
  int i;

  memset(buffer, '\0', sizeof(char));
  
  bufferTail = 0;
  BufferReset();              // Clear the buffer and start over...

  while (true) {
    if (kbd.available()) {
      ch = kbd.read();
      if (ch == ESCAPEKEY) {  // They want to terminate message
        BufferReset();        // Clear all and start over
        return 0;
      }
      charCount++;
      if (ch == ')' || charCount == QUEUEMASK || ch == NEWLINE) { // Is long message finished or terminated?
        for (i = 0; i < charCount; i++) {
           send(buffer[i]);
        }
        BufferReset();              // Clear the buffer and start over...
        break;
      } else if ((ch == '(') || (ch == ESCAPEKEY) || (ch == NOCHARAVAILABLE)) {
          ; // ignore character
      } else {
        buffer[charCount++] = ch;
        if (charCount > (QUEUEMASK - 20)) {   // Approaching full buffer
#ifdef DEBUG
  Serial.print("  charCount = ");
  Serial.println(charCount);
#endif
          FlashBufferFullWarning();
        }
      }
    }
  }
  return charCount;
}


void ChangeSendingSpeed()
{
  char ch;
  
  int wereDone = 0;

#ifdef DEBUG  
  Serial.print(" wordsPerMinute at start =");
  Serial.println(wordsPerMinute);     
#endif
  while (true) {
    if (kbd.available()) {
      ch=kbd.read();
#ifdef DEBUG  
      Serial.print("Speed change ch =");
      Serial.println(ch);
#endif
      tft.setCursor(330, 298);
      tft.print("Wpm = ");
      tft.print(wordsPerMinute);
      switch (ch) {
        case '>':  
          wordsPerMinute++;
          break;
        case '<':
          wordsPerMinute--;
          break;
        case '#':
          wereDone = 1;
          tft.fillRect(329, 298, 140, 16, BLUE);
          break;
        default:
          break;
      }
    }
    if (wereDone)
      break;
  }
  ditlen = 1200 / wordsPerMinute;
}

void clearLine(int lineNo) 
{
   
}

