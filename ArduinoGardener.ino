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
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

//#define _TRACE
// Your Xively key to let you upload data
char xivelyKey[] = "n4XEgDceFq6VjOM6ZOxY09OvEx8z1V9azBAkN4q8eadi8YoK"; // enter the key, under API Keys
unsigned long feedId = /*297694980*/1530032864; // enter your feed ID, under Activated
int frequency = 330; // delay between updates (seconds)

// Define the strings for our datastream IDs
char sensorId[] = "sensor1";
char sensor2Id[] = "drought1";

XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorId, strlen(sensorId), DATASTREAM_INT),
  XivelyDatastream(sensor2Id, strlen(sensor2Id), DATASTREAM_INT),
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(feedId, datastreams, 2 /* number of datastreams */);
// assign a MAC address for the ethernet controller.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
// fill in your address here:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(192,168,1,27);
// initialize the library instance:
EthernetClient client;
XivelyClient xivelyclient(client);

#define MENU_X	1		// 0-83
#define MENU_Y	0		// 0-5
#define WateringLevel 330
char string[30];

int PinAnalogiqueHumidite=1;       //Broche Analogique de mesure d'humidité
//    int PinNumeriqueHumidite=9;        //Broche Numérique mesure de l'humidité
int PinAlim=/*10*/9;
//int PinLed=11;    //LED témoin de seuil de  sécheresse
int PinValve=8;

unsigned int WateringTime = 4000; //2s
int hsol;  //Humidite su sol, mesure analogique
int secheresse=0;  //0 ou 1 si seuil atteint

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
    // start the Ethernet connection:
  lcd.LCD_write_string(MENU_X, MENU_Y + 5, "DHCP...", MENU_NORMAL);
  if (Ethernet.begin(mac) == 0) 
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    lcd.LCD_write_string(MENU_X, MENU_Y + 4, "DHCP KO", MENU_NORMAL);
    lcd.LCD_write_string(MENU_X, MENU_Y + 4, "IP Fixe...", MENU_NORMAL);
    // DHCP failed, so use a fixed IP address:
    Ethernet.begin(mac, ip);
  }
  else
    lcd.LCD_write_string(MENU_X, MENU_Y + 5, "DHCP OK", MENU_NORMAL);
  
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) 
  {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  sprintf(string,"%d.%d.%d.%d",Ethernet.localIP()[0],Ethernet.localIP()[1],Ethernet.localIP()[2],Ethernet.localIP()[3]);
    lcd.LCD_write_string(MENU_X, MENU_Y + 5, string, MENU_NORMAL);
    
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
 //   secheresse = digitalRead(PinNumeriqueHumidite);
//     secheresse=0;
    
    //oscillo processing
    /*Serial.write( 0xff );
    Serial.write( (hsol >> 8) & 0xff );
    Serial.write( hsol & 0xff );*/
   
    if (hsol >= WateringLevel)
    {
//      digitalWrite(PinLed, HIGH);   // LED allumée
      lcd.backlight(ON);
      secheresse=1;
     
      Watering(WateringTime);
    }
    else 
    {
      lcd.backlight(OFF);
      secheresse=0; 
          
//      digitalWrite(PinLed, LOW);   // LED off
    }
    
    DisplayValue(hsol,secheresse);
    Serial.println(hsol); // afficher la mesure
    Serial.print("  ");
    Serial.println(secheresse);  //0 ou 1 si le seuil est dépassé
    
    
    
    datastreams[0].setInt(hsol);
    datastreams[1].setInt(secheresse);
    Serial.print("Moisture: ");
    Serial.print(datastreams[0].getInt());
    Serial.print("drought: ");
    Serial.print(datastreams[1].getInt());
    Serial.println("Uploading it to Xively");
    int ret = xivelyclient.put(feed, xivelyKey);
    Serial.print("xivelyclient.put returned ");
    Serial.println(ret);
    Serial.println();
   
    WaitDisplay(frequency);
    
    
    
}




