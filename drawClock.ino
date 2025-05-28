// Clock
void drawClock(bool updateCompleteDisplay)
{ 
  static int lastHour = -1;
  static int lastMinute = -1;
  static bool lastDrawDots = false;

  // Only redraw the full clock if the time has changed or a full refresh is requested
  if (updateCompleteDisplay || clockHour != lastHour || clockMinute != lastMinute) {
    u8g2.clearDisplay();
    u8g2.setDrawColor(1);  
    u8g2.drawXBM( 96, 0, iconGTWidth, iconGTHeight, iconGTBits);
    u8g2.setFont(u8g2_font_logisoso32_tn);
    if ( clockHour>9 )
      u8g2.drawStr(0,32,String(clockHour).c_str());
    else {
      u8g2.drawStr(0,32,"0");
      u8g2.drawStr(20,32,String(clockHour).c_str());
    }
    if ( clockMinute>9 ) {
      u8g2.drawStr(47,32,String(clockMinute).c_str());
    } else {
      u8g2.drawStr(47,32,"0");
      u8g2.drawStr(67,32,String(clockMinute).c_str());
    }
    lastHour = clockHour;
    lastMinute = clockMinute;
    // Draw the dots in the correct state
    u8g2.setDrawColor(drawDots);
    u8g2.drawBox(42,8,3,3);
    u8g2.drawBox(42,20,3,3);
    u8g2.setDrawColor(1);
    u8g2.sendBuffer();
    lastDrawDots = drawDots;
  } else if (drawDots != lastDrawDots) {
    // Only update the dots (erase old, draw new)
    u8g2.setDrawColor(0); // Erase old dots
    u8g2.drawBox(42,8,3,3);
    u8g2.drawBox(42,20,3,3);
    u8g2.setDrawColor(drawDots); // Draw new dots
    u8g2.drawBox(42,8,3,3);
    u8g2.drawBox(42,20,3,3);
    u8g2.setDrawColor(1);
    u8g2.sendBuffer();
    lastDrawDots = drawDots;
  }
}
