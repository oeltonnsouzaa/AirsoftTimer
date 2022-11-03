void domination(){

  //SETUP INITIAL TIME 
  int minutos = GAMEMINUTES-1;
  boolean showGameTime=true;
  unsigned long a;
  unsigned long iTime=millis(); //  initialTime in millisec 
  unsigned long aTime;
 
  team=0;
  iZoneTime=0;
  aTime=0;
  redTime=0;
  greenTime=0;

  int largoTono = 50;
  // 0 = neutral, 1 = green team, 2 = red team
  a=millis();
  //Starting Game Code
  while(1)  // this is the important code, is a little messy but works good.
  {
    if(endGame){
      gameOver();
    }
    
    keypad.getKey();
    aTime=millis()- iTime;
    //Code for led blinking
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      if(team==1)digitalWrite(GREENLED, HIGH);  
      if(team==2)digitalWrite(REDLED, HIGH);  
    }
    if(timeCalcVar >= 50 && timeCalcVar <= 100)
    {    
      if(team==1)digitalWrite(GREENLED, LOW);  
      if(team==2)digitalWrite(REDLED, LOW);
    }
    // Sound!!! same as Destroy 
    if(timeCalcVar >= 0 && timeCalcVar <= 40 && soundEnable)tone(tonepin,activeTone,largoTono);

    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,activeTone,largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)tone(tonepin,activeTone,largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,activeTone,largoTono);
    //Help to count 3 secs
    if(a+2000<millis()){
      a=millis();   
      showGameTime=!showGameTime;
      cls();
    }
    //THE NEXT TO METHODS SHOW "GAME TIME" AND "CONTROLED ZONE TIME" IT SHOWS 2 AND 2 SEC EACH

    if(showGameTime){ //THE SECOND IS /2
      lcd.setCursor(1,0);
      lcd.print("TEMPO DE JOGO");
      lcd.setCursor(3,1);
      printTime(minutos, aTime);
    }
    else if (!showGameTime){

      lcd.setCursor(1,0);
      if(team == 0)lcd.print(" ZONA NEUTRA");
      if(team == 1)lcd.print("  ZONA VERDE");
      if(team == 2)lcd.print("ZONA VERMELHA");

      if(team>0){
        lcd.setCursor(3,1);
        printTimeDom(millis()-iZoneTime,true);
      }
    }

    //###########################CHECKINGS##################

    //Check If Game End
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    {
      gameOver();
    }

    //Check If IS neutral
    while((defusing || fusing) && team > 0)
    {
      cls();
      if(team>0)lcd.print("NEUTRALIZANDO...");
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(fusing || defusing)
      {
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        
        keypad.getKey();
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          if(soundEnable)tone(tonepin,alarmTone1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)
          tone(tonepin,alarmTone2,200);
      
          digitalWrite(REDLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawBar(percent);

        if(percent >= 100)

        
        {
     tone(tonepin,alarmTone2,200);
     delay(1000);
  

          if(team==1){ 
            greenTime+=millis()-iZoneTime;
            iZoneTime=0; 

          }
          if(team==2){ 
            redTime+=millis()-iZoneTime;
            iZoneTime=0; 
          }
          team=0;
          break;
        }
      }
      
      cls();
    }

    //Capturing red

    while(fusing && team == 0 )
    {
      cls();
      if(team==0)lcd.print("CAPTURANDO ZONA");
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(fusing)
      {
        keypad.getKey();
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(REDLED, HIGH);  
          if(soundEnable)tone(tonepin,alarmTone1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,alarmTone2,200);
          digitalWrite(REDLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawBar(percent);

        if(percent >= 100)
        {
               tone(tonepin,alarmTone2,200);
     delay(200);
         tone(tonepin,alarmTone2,200);
     delay(200);
      tone(tonepin,alarmTone2,200);
          digitalWrite(GREENLED, LOW);
          team=2;
          iZoneTime=millis();
          delay(1000);
          
  digitalWrite(RELAYPIN, LOW);   
  delay(1000); 
  digitalWrite(RELAYPIN, HIGH);
          break;
        }
      }
        
      cls();
      digitalWrite(REDLED, LOW);
    }

    //getting to green zone
    while(defusing && team == 0 )
    {
      cls();
      if(team==0)lcd.print(" CAPTURANDO ZONA");
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defusing)
      {
        keypad.getKey();
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(GREENLED, HIGH);  
          if(soundEnable)tone(tonepin,alarmTone1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,alarmTone2,200);
          digitalWrite(GREENLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawBar(percent);

        if(percent >= 100)
        {
           tone(tonepin,alarmTone2,200);
     delay(200);
         tone(tonepin,alarmTone2,200);
     delay(200);
      tone(tonepin,alarmTone2,200);
          digitalWrite(GREENLED, LOW);
          team=1;
          iZoneTime=millis();
          delay(1000);
           digitalWrite(RELAYPIN, LOW);   
  delay(1000); 
  digitalWrite(RELAYPIN, HIGH);
          break;
        }
      }
      cls();
      digitalWrite(GREENLED, LOW);  
    }
  }
}

void gameOver(){

  
  if(team==1)greenTime+=millis()-iZoneTime;
  if(team==2)redTime+=millis()-iZoneTime;
  
  cls();    
  lcd.print("FIM DO JOGO ");
  digitalWrite(GREENLED, LOW);
  digitalWrite(REDLED, LOW);

   digitalWrite(RELAYPIN, LOW);   
  delay(5000); 
  digitalWrite(RELAYPIN, HIGH);
  while(!defusing){
    keypad.getKey();
    if(defusing){
      keypad.getKey();
      break;
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FIM DO JOGO ");
    lcd.setCursor(0,1);

  
    //check who team win the base
    if(greenTime>redTime){
      //greenteam wins
      lcd.print("VITORIA VERDE");
      digitalWrite(GREENLED, HIGH);
    }
    else{
      //redteam wins 
      lcd.print("VITORIA VERMELHO");
      digitalWrite(REDLED, HIGH);
    }
    delay(3000);
    keypad.getKey();
    if(defusing){
      keypad.getKey();
      break;
    }
    cls();
    lcd.print("TEMPO VERMELHO:");
    lcd.setCursor(5,1);
    printTimeDom(redTime,false);
    delay(3000);
    keypad.getKey();
    if(defusing){
      
      break;
    }
    cls();
    lcd.print("TEMPO VERDE:");
    lcd.setCursor(5,1);
    printTimeDom(greenTime,false);
    delay(3000);
    keypad.getKey();
    if(defusing){
      keypad.getKey();
      break;
    }
  }
  cls();
  delay(100);
  lcd.print("Jogar novamente?");
  lcd.setCursor(0,1);
  lcd.print("A : SIM B : NAO");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      cls();
      domination();
      break;
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();
      break;
    }  
  } 
}
