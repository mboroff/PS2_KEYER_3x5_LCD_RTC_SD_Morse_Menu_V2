void displaySetup()
{
  
  tft.fillScreen(BLUE);               // Clear the screen
  tft.drawRect(0, 0, 479, 319,WHITE);
  tft.fillRect(0, 108, 479, 2,WHITE);  
  tft.fillRect(0, 270, 479, 2,WHITE);  
  tft.fillRect(0, 293, 479, 2,WHITE);  
  tft.setTextColor(BLACK, BLUE); 
  tft.setCursor(174, 4);
  tft.print("WD9GYM");
  tft.setCursor(175, 5);
  tft.print("WD9GYM");
  tft.setCursor(176, 6);
  tft.setTextColor(WHITE, BLUE); 
  tft.print("WD9GYM");
  tft.setCursor(70, 40);
  tft.print("UTC");
  tft.setCursor(320, 40);
  tft.print("Local");
  tft.setTextSize(2);
  tft.setCursor(10, 276);
  tft.print("CW Rx");
  tft.setCursor(10, 298);
  tft.print("CW Tx");

}
void displayRadio() 
{
    tft.fillRect(3, 111, 473, 158, BLUE);
    tft.setTextSize(3);
    tft.setTextColor(WHITE, BLUE); 
    tft.setCursor(176, 117);
    tft.print("Patriot");
    if (waitingForPatriot == true) {
        tft.fillCircle(310, 125, 5, RED);
    }
  tft.setTextSize(2);  
  tft.setCursor(10, 150); 
  tft.print("RX");
  tft.setCursor(10, 170); 
  tft.print("TX");
  tft.setCursor(10, 190); 
  tft.print("STEP");
  tft.setCursor(10, 210); 
  tft.print("BW");

        tft.setCursor(280, 150); 
        tft.print("RIT");
        tft.setCursor(280, 170); 
        tft.print("BAND");
        tft.setCursor(280, 190); 
        tft.print("MODE");
        tft.setCursor(280, 210); 
        tft.print("BAT");
           
}
void optionsWhite()
{
  tft.fillRect(3, 111, 473, 158, BLUE);
  tft.setTextSize(3);
  tft.setTextColor(WHITE, BLUE); 
  tft.setCursor(176, 117);
  tft.print("Options");
  tft.setTextSize(2);
  tft.setCursor(10, 150); 
  tft.print("1 - QSO RCV Info");
  tft.setCursor(10, 170); 
  tft.print("2 - Send TNX");
  tft.setCursor(10, 190); 
  tft.print("3 - My Sig Info");
  tft.setCursor(10, 210); 
  tft.print("4 - Write QSO Log");
  tft.setCursor(10, 230);
  tft.print("5 - Set UTC Clock");
  tft.setCursor(10, 250);
  tft.print("S - Snd CQ  T - Tune # - Chg CW Speed");
  tft.setCursor(320, 150);
  tft.print("QSO Info");
  tft.setCursor(260, 170);
  tft.print("Rx Id  -");
  tft.setCursor(260, 190);
  tft.print("Rx Sig -");
  tft.setCursor(260, 210);
  tft.print("Freq   -");
  tft.setCursor(260, 230);
  tft.print("My Sig -");


}

void optionsGreen()
{
  tft.setTextSize(3);
  tft.setTextColor(GREEN, BLUE); 
  tft.setCursor(176, 117);
  tft.print("Options");
  tft.setTextSize(2);

}
