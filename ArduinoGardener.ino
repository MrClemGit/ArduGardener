//A FAIRE : 
//Algo d'ouverture de la valve pendant un temps T puis attente pendant un certain (attente de l'impregnation de l'eau) et relancer la detection de secheresse
//Ajout Ecran pour message voir configuration des temps : OK
//Utilisation de la shield Ethernet pour envoi de donnée sur serveur ou sd card pour log. OK
//V1.1=>V1.2
//Changement pin analogique pour mettre le shield LCD4884
//V1.2=>V1.3
//Adaptation des Pins pour utilisation du shield Ethernet. (10-11-12-13 non utilisable) il ne reste plus que 8-9-(0-1 non accessible mais dispo) 
//V1.2=>V1.4 utilisation de la lib xively

#include "LCD4884.h"
#include <SPI.h>


//#define _TRACE

int frequency = 330; // delay between updates (seconds)
unsigned int WateringTime = 6000; //2s
#define WateringLevel 330
#define TrybeforeWatering 3 // must be 3 times at the WateringLevel before watering  (avoiding clitches)

#define MENU_X	1		// 0-83
#define MENU_Y	0		// 0-5

char string[30];

int PinAnalogiqueHumidite=1;       //Broche Analogique de mesure d'humidité
//    int PinNumeriqueHumidite=9;        //Broche Numérique mesure de l'humidité
int PinAlim=/*10*/9;
//int PinLed=11;    //LED témoin de seuil de  sécheresse
int PinValve=8;


int hsol;  //Humidite su sol, mesure analogique
int secheresse=0;  //0 ou 1 si seuil atteint

int i_try = 0;

void setup()
{ // Initialisation
    lcd.LCD_init();
    lcd.LCD_clear();
    lcd.backlight(OFF);
    
    Serial.begin(9600);  //Connection série à 9600 baud
    /*while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
    }*/
    pinMode(PinAnalogiqueHumidite, INPUT);       //pin A0 en entrée analogique
//    pinMode(PinNumeriqueHumidite, INPUT);  //pin 3 en entrée numérique
    pinMode(PinAlim, OUTPUT);  //pin 9 pour controler l'alim du module
//   pinMode(PinLed, OUTPUT);   //LED témoin
    pinMode(PinValve, OUTPUT);   //Valve
    digitalWrite(PinAlim, LOW);
    digitalWrite(PinValve, LOW);
    
    init_MENU();
   
}

void ActiveValve()
{
   digitalWrite(PinValve, HIGH);  
}

void DesactiveValve()
{
    digitalWrite(PinValve, LOW);  
}
void init_MENU(void){

  byte i;
  lcd.LCD_clear();
  lcd.LCD_write_string(MENU_X+10, MENU_Y, "Moisture", MENU_HIGHLIGHT );

}

void Watering(unsigned int ui_msOpeningTime)
{
   ActiveValve();
   delay(ui_msOpeningTime);
   DesactiveValve();
}

void DisplayValue(int i_iValue,int i_iSeuil)
{
    memset(string,'\0',30);
    sprintf(string,"Value %03d (%d)",i_iValue,i_iSeuil);
    lcd.LCD_write_string(MENU_X, MENU_Y + 1, string, MENU_NORMAL);
    if (i_try > 0)
    {
      sprintf(string,"Try (%d)",i_try);
      lcd.LCD_write_string(MENU_X, MENU_Y + 2, string, MENU_NORMAL);
    }
    else
      lcd.LCD_write_string(MENU_X, MENU_Y + 2, "", MENU_NORMAL);
}
void WaitDisplay(int i_iSecond)
{
  int bAnswerOK=0; 
  for (int i= 0;i<i_iSecond+1;i++)
  {
    char temp [30];
    memset(temp,'\0',30);
    sprintf(temp,"measure in %02ds",i_iSecond-i);
    
    lcd.LCD_write_string(MENU_X-10, MENU_Y + 2, temp, MENU_NORMAL); 
   
   
    delay(1000); 
   
  } 
  
  
}

void loop() { //boucle principale
    digitalWrite(PinAlim, HIGH);
    delay(10);
    hsol = analogRead(PinAnalogiqueHumidite); // Lit la tension analogique
    digitalWrite(PinAlim, LOW);
    
    //oscillo processing
    /*Serial.write( 0xff );
    Serial.write( (hsol >> 8) & 0xff );
    Serial.write( hsol & 0xff );*/
   
    if (hsol >= WateringLevel)
    {
//      digitalWrite(PinLed, HIGH);   // LED allumée
      if (i_try == 3)
      {
        lcd.backlight(ON);
        secheresse=1;
        Watering(WateringTime);
      }
      
      i_try++;
    }
    else 
    {
      lcd.backlight(OFF);
      secheresse=0;
      i_try = 0; 
          
//      digitalWrite(PinLed, LOW);   // LED off
    }
    
    DisplayValue(hsol,secheresse);
    Serial.println(hsol); // afficher la mesure
    Serial.print("  ");
    Serial.println(secheresse);  //0 ou 1 si le seuil est dépassé
        
    WaitDisplay(frequency);
    
    
    
}




