ArduGardener
============

Humidity detection and automatic watering with arduino

V1.4 : 
- Arduino Uno
- Ethernet Shield
- LCD4884 LCD Shield (http://tronixstuff.com/2011/03/12/the-dfrobot-lcd4884-lcd-shield/)
![LCD4884](http://i1.wp.com/tronixstuff.com/wp-content/uploads/2011/03/image1.jpeg) 
- Flexible pipe
- pump ![pump](http://img.dxcdn.com/productimages/sku_205019_2.jpg)
- soil moisture sensor 
- ![soil moisture sensor](http://i.ebayimg.com/00/s/NTAwWDUwMA==/z/6ucAAOSwEK9T9x8o/$_12.JPG)
- wires
- TIP120
- ...




Changelog before the first commit on Github : 

//Algo d'ouverture de la valve pendant un temps T puis attente pendant un certain (attente de l'impregnation de l'eau) et relancer la detection de secheresse
//Ajout Ecran pour message voir configuration des temps : OK
//Utilisation de la shield Ethernet pour envoi de donnée sur serveur ou sd card pour log. OK
//V1.1=>V1.2
//Changement pin analogique pour mettre le shield LCD4884
//V1.2=>V1.3
//Adaptation des Pins pour utilisation du shield Ethernet. (10-11-12-13 non utilisable) il ne reste plus que 8-9-(0-1 non accessible mais dispo) 
//V1.2=>V1.4 utilisation de la lib xively


A FAIRE : 

- supprimer le shield LCD
- supprimer le shield Ethernet => remplacer par le WEDO de DFRObot avec wifi intégré.
- ajouter un petit écran OLED pour test.
- ...
