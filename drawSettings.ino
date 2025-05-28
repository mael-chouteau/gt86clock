// Settings
void drawSettingsWifi(bool updateCompleteDisplay)
{   
  static unsigned long screenStart = 0;
  if (updateCompleteDisplay) {
    u8g2.clearDisplay();  
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_crox2tb_tf);
    String ipaddress = WiFi.localIP().toString();
    u8g2.drawStr(1,16,("SSID:"+WiFi.SSID()).c_str()); 
    u8g2.drawStr(1,32,ipaddress.c_str());
    u8g2.sendBuffer(); 
    screenStart = millis();
  }
  if (millis() - screenStart > 2000) {
    modeCurrent++;
    modeOld = modeCurrent;
  }
}

void drawSettingsClock(bool updateCompleteDisplay)
{   
  static unsigned long screenStart = 0;
  if (updateCompleteDisplay) {
    char temp[20];
    u8g2.clearDisplay();  
    sprintf(temp, "%d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
    u8g2.drawStr(1,16, temp);
    if ( clock24h ) 
      u8g2.drawStr(1,32,"CLOCK: 24h");       
    else
      u8g2.drawStr(1,32,"CLOCK: 12h");
    u8g2.sendBuffer(); 
    screenStart = millis();
  }
  if (millis() - screenStart > 2000) {
    modeCurrent++;
    modeOld = modeCurrent;
  }
}

void drawSettingsUnits(bool updateCompleteDisplay)
{      
  static unsigned long screenStart = 0;
  if (updateCompleteDisplay) {
    u8g2.clearDisplay();
    if ( temperatureCelsius )
      u8g2.drawStr(1,16,"TEMP.: C"); 
    else
      u8g2.drawStr(1,16,"TEMP.: F"); 
    if ( pressureBar )
      u8g2.drawStr(1,32,"PRESSURE: bar");
    else
      u8g2.drawStr(1,32,"PRESSURE: psi"); 
    u8g2.sendBuffer(); 
    screenStart = millis();
  }
  if (millis() - screenStart > 2000) {
    modeCurrent++;
    modeOld = modeCurrent;
  }
}

void drawSettingsO2(bool updateCompleteDisplay)
{             
  static unsigned long screenStart = 0;
  if (updateCompleteDisplay) {
    u8g2.clearDisplay();
    if ( o2afr )  
      u8g2.drawStr(1,16,"O2: AFR");
    else
      u8g2.drawStr(1,16,"O2: Lambda");
    u8g2.sendBuffer(); 
    screenStart = millis();
  }
  if (millis() - screenStart > 2000) {
    modeCurrent=SETTINGSWIFI;
    modeOld=modeCurrent;
  }
}
