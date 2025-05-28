void drawAfrAndVoltage(bool updateCompleteDisplay)
{ 
  if ( o2afr )
  {
    char afrChar[4];
    dtostrf(afr,1,1,afrChar);
    char voltageChar[4];
    
    dtostrf(voltage,1,1,voltageChar);
    drawTwoValues(o2Width     , o2Height     , o2Bits     , afrChar    , countDigits(float(round((afr/14.7)*100))/100), "" , 
                  batteryWidth, batteryHeight, batteryBits, voltageChar, 4               , "V", updateCompleteDisplay);
  }
  else
  {
    char afrChar[4];
    dtostrf(afr,1,2,afrChar);
    char voltageChar[4];
    dtostrf(voltage,1,1,voltageChar);
    
    drawTwoValues(o2Width     , o2Height     , o2Bits     , afrChar    , countDigits(afr), "" , 
                  batteryWidth, batteryHeight, batteryBits, voltageChar, 4               , "V", updateCompleteDisplay);
  }
}

void drawCoolantOilTemp(bool updateCompleteDisplay)
{ 
  if ( temperatureCelsius )
    drawTwoValues(iconCoolantWidth, iconCoolantHeight, iconCoolantBits, (char*)String(coolantTemp).c_str(), countDigits(coolantTemp) , "C", 
                  iconOilCanWidth , iconOilCanHeight , iconOilCanBits , (char*)String(oilTemp).c_str()    , countDigits(oilTemp)     , "C", updateCompleteDisplay);
  else
    drawTwoValues(iconCoolantWidth, iconCoolantHeight, iconCoolantBits, (char*)String(coolantTemp*1.8+32).c_str(), countDigits(coolantTemp*1.8+32) , "F", 
                  iconOilCanWidth , iconOilCanHeight , iconOilCanBits , (char*)String(oilTemp*1.8+32).c_str()    , countDigits(oilTemp*1.8+32)     , "F", updateCompleteDisplay);
}

void drawCoolantTemp(bool updateCompleteDisplay)
{ 
   if ( temperatureCelsius )
     drawSingleValue(iconCoolantWidth, iconCoolantHeight, iconCoolantBits, (char*)String(coolantTemp).c_str()   , countDigits(coolantTemp)       , "C", updateCompleteDisplay);
   else
     drawSingleValue(iconCoolantWidth, iconCoolantHeight, iconCoolantBits, (char*)String(oilTemp*1.8+32).c_str(), countDigits(coolantTemp*1.8+32), "F", updateCompleteDisplay);
}

void drawOilTemp(bool updateCompleteDisplay)
{ 
   if ( temperatureCelsius )
     drawSingleValue(iconOilCanWidth, iconOilCanHeight, iconOilCanBits, (char*)String(oilTemp).c_str()       , countDigits(oilTemp)       , "C", updateCompleteDisplay);
   else
     drawSingleValue(iconOilCanWidth, iconOilCanHeight, iconOilCanBits, (char*)String(oilTemp*1.8+32).c_str(), countDigits(oilTemp*1.8+32), "F", updateCompleteDisplay);
}

void drawOilPressure(bool updateCompleteDisplay)
{ 
   if ( pressureBar )
   {
     char oilPressureChar[4];
     dtostrf(oilPressure,1,1,oilPressureChar);
     drawSingleValue(iconOilCanWidth, iconOilCanHeight, iconOilCanBits, oilPressureChar, 3, "bar", updateCompleteDisplay);
   }
   else
   { 
     char oilPressureChar[4];
     dtostrf(oilPressure,1,0,oilPressureChar);
     drawSingleValue(iconOilCanWidth, iconOilCanHeight, iconOilCanBits, oilPressureChar, 2, "psi", updateCompleteDisplay);
   }

  customDelay(250);
}

void drawLogo(bool updateCompleteDisplay)
{   
   if ( updateCompleteDisplay == true )
   {
     u8g2.clearDisplay();  
     u8g2.drawXBM( 0, 0, logoWidth, logoHeight, logoBits);
     u8g2.sendBuffer(); 
     wifiManager.autoConnect("gt86clock");
     WiFi.hostname("gt86clock");
     modeCurrent=modeSaved;
     delay(1000);
   }
}

int countDigits(int n) 
{ 
  if ( n < 0 )
    return floor(log10(abs(n))+2);
  else if ( n > 0 )
    return floor(log10(abs(n))+1); 
  else  
    return 1;
} 

void drawSingleValue(int iconWidth, int iconHeight, unsigned char iconBits[], const char* value, int valueLength, const char* unit, bool updateCompleteDisplay)
{ 
  static int lastValue = -9999;
  static char lastUnit[4] = "";
  static int lastIconWidth = -1, lastIconHeight = -1;
  static unsigned char* lastIconBits = nullptr;
  bool valueChanged = (strcmp(value, "") != 0 && (atoi(value) != lastValue || strcmp(unit, lastUnit) != 0 || iconWidth != lastIconWidth || iconHeight != lastIconHeight || iconBits != lastIconBits));

  if ( updateCompleteDisplay || valueChanged )
  {
    u8g2.clearDisplay();
    u8g2.drawXBM( 2, round((32-iconHeight)/2), iconWidth, iconHeight, iconBits);

    if (strcmp(unit, "C") == 0 || strcmp(unit, "F") == 0)
    {
      u8g2.drawCircle(118,3,3);
      u8g2.setFont(u8g2_font_crox3cb_mf);
      u8g2.drawStr(116,32,unit);
    }
    else if (strlen(unit) >= 3)
    {
      u8g2.setFont(u8g2_font_pxplusibmvga9_mf);
      u8g2.drawStr(120,32,&unit[2]);
      u8g2.drawStr(120,21,&unit[1]);
      u8g2.drawStr(120,10,&unit[0]);
    }

    u8g2.setDrawColor(0);
    u8g2.drawBox(55,0,60,32);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_logisoso32_tn);
    u8g2.drawStr(115-valueLength*20,32,value);
    u8g2.sendBuffer();  
    lastValue = atoi(value);
    strncpy(lastUnit, unit, 3); lastUnit[3] = '\0';
    lastIconWidth = iconWidth; lastIconHeight = iconHeight; lastIconBits = iconBits;
  }
}

void drawTwoValues(int iconUpWidth, int iconUpHeight, unsigned char iconUpBits[], const char* valueUp, int valueUpLength, const char* unitUp, int iconDownWidth, int iconDownHeight, unsigned char iconDownBits[], const char* valueDown, int valueDownLength, const char* unitDown, bool updateCompleteDisplay)
{ 
  static int lastValueUp = -9999, lastValueDown = -9999;
  static char lastUnitUp[4] = "", lastUnitDown[4] = "";
  static int lastIconUpWidth = -1, lastIconUpHeight = -1, lastIconDownWidth = -1, lastIconDownHeight = -1;
  static unsigned char* lastIconUpBits = nullptr, *lastIconDownBits = nullptr;
  bool valueChanged = (strcmp(valueUp, "") != 0 && (atoi(valueUp) != lastValueUp || atoi(valueDown) != lastValueDown || strcmp(unitUp, lastUnitUp) != 0 || strcmp(unitDown, lastUnitDown) != 0 || iconUpWidth != lastIconUpWidth || iconUpHeight != lastIconUpHeight || iconDownWidth != lastIconDownWidth || iconDownHeight != lastIconDownHeight || iconUpBits != lastIconUpBits || iconDownBits != lastIconDownBits));

  if ( updateCompleteDisplay || valueChanged )
  {
    u8g2.clearDisplay();
    u8g2.drawXBM( 4, -17, iconUpWidth  , iconUpHeight  , iconUpBits );
    u8g2.drawXBM( 2,  18, iconDownWidth, iconDownHeight, iconDownBits);
    
    if ((strcmp(unitUp, "C") == 0) || (strcmp(unitDown, "F") == 0))
    {
      u8g2.setFont(u8g2_font_crox3cb_mf);
      u8g2.drawCircle(112,18,2);
      u8g2.drawCircle(112,2,2);
      u8g2.drawStr(116,16,unitUp);
      u8g2.drawStr(116,32,unitDown);
    }
    else if (strcmp(unitDown, "V") == 0) {
      u8g2.setFont(u8g2_font_crox3cb_mf);
      u8g2.drawStr(116,32,unitDown);
    }

    u8g2.setDrawColor(0);
    u8g2.drawBox(70,0,39,32);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_crox3cb_mf);
    u8g2.drawStr(108-valueUpLength*13,16,valueUp);
    u8g2.drawStr(108-valueDownLength*13,32,valueDown);
    u8g2.sendBuffer();  
    lastValueUp = atoi(valueUp); lastValueDown = atoi(valueDown);
    strncpy(lastUnitUp, unitUp, 3); lastUnitUp[3] = '\0';
    strncpy(lastUnitDown, unitDown, 3); lastUnitDown[3] = '\0';
    lastIconUpWidth = iconUpWidth; lastIconUpHeight = iconUpHeight; lastIconDownWidth = iconDownWidth; lastIconDownHeight = iconDownHeight;
    lastIconUpBits = iconUpBits; lastIconDownBits = iconDownBits;
  }
}
