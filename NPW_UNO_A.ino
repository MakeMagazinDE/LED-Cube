// Filename als String
String filn = "NPW_UNO_A_v2.ino"; 
/*  
 *  ###########################################
 *  Neopixelwürfel für Arduino UNO,  NPW_UNO_A_vX.ino = Figuren, Funkeln, Säulen, Sprudel, Schlange
 *                                   NPW_UNO_B_vX.ino = Würfel, Funkeln, Sprudel
 *  
 *  Das Programm startet im Demo Modus und zeigt vorhandene Figuren und Animationen
 *  
 * im seriellen monitor muß no line ending eingestellt sein (unten neben Baudrate) 
 * und 115200 Baud
 *  
 * Steuerbefehle:
 * 0 = alle Leds aus
 * 3 = mehr Funkel Leds
 * 4 = weniger Funkel Leds
 * 5 = Animationsgeschwindigkeit default = 60
 * 
 * a = Animationen aus
 * b = bunte Leds ein/aus
 * D = Demo ein
 * d = Demo aus
 * F = alle Leds auf default Farbwerte (wie beim Programmstart)
 * G = Animations Geschwindigkeit mit Tastatur / Poti einstellbar
 * g = Animations Geschwindigkeit mit Tastatur / Poti aus
 * H = Helligkeit erhöhen, primäre und sekundäre Vordergrundfarbe (Hue3,Hue2)
 * h = Helligkeit weniger, primäre und sekundäre Vordergrundfarbe (Hue3,Hue2)
 * i = Info
 * n = automatischer Farbwechsel an/aus  
 * r = Richtung vorwärts/ rückwärts 
 * S = stop animation
 * s = step animation 
 * w = animation weiter
 * t = Test (Funktionstest-Neopixelwürfel)
 * 
 * + = Tastaturpoti +1
 * - = Tastaturpoti -1
 * P = Tastaturpoti +15
 * M = Tastaturpoti -15
 * 
 * Anmerkung: Es wird im Programm nur das "Tastaturpoti" verwendet, der Code für ein 
 *              richtiges Poti ist auskommentiert            
 * 
 * Farbsätze mit Tastatur / Poti einstellbar, dazu auto. Farbwechsel ausschalten mit n,
 * ist mit einem weiteren  n  wieder einschaltbar
 *  als nächstens eine Animation starten z.B. Schlange oder 
 *  Würfelumriss mit Farbanimation (fi3)  
 *  dann:
 *  F1h, F1s, F1v   Hintergrund (Led nahezu aus) Farbe h= hue, s= saturation, v= value
 *  F2h, F2s, F2v   sekundäre Vordergrund Farbe (Led ein)                                          
 *  F3h, F3s, F3v   primäre Vordergrund Farbe (Led ein) 
 *  mit +, -, P, M ändern
 *  F00 = Farbsätze einstellen - aus
 *  
 * t = Testprogramm - Funktionskontrolle für alle Leds, blinkt weiss, heller, dunkler
 * 
 * Figuren
 * fi1 = Umriss des Neopixel Würfels
 * fi2 = Würfel Umriss ein mit Farbanimation
 * fi3 = Würfelumriss und kleines Drehkreuz in Mitte mit Farbanimation 
 * 
 * --- Animationen ---
 * Funkeln
 * fu0 = Dauer funkeln aus
 * fu1 = funkeln dauernd, vorher mit fuw,fur,fuy,fug,fub oder fuB, Farbe wählen
 * fuw = Funkeln weiss - kurzzeitig
 * fur = Funkeln rot - kurzzeitig
 * fuy = Funkeln yellow/gelb - kurzzeitig
 * fug = Funkeln grün - kurzzeitig
 * fub = Funkeln blau - kurzzeitig
 * fuB = Funkeln bunt - kurzzeitig
 * 
 * Säulen
 * sl1 = Schrägsäule
 * sl2 = doppelschrägsäulen
 * sl3 = doppelschrägsäulen ausgefüllt
 * sl4 = Säulen äußere Säulen umlaufend, dann nach innen
 * sl5 = Säulen äußere Säulen umlaufend 
 * sl6 = Säulen zick-zack,
 * sl7 = umlaufend innen
 * sl8 = diagonal
 * sl9 = 2 Wendel diagonal 
 * 
 * Schlange
 * sc1 = Schlange rechtwinklig
 * sc2 = Schlange 45 grad
 * sc3 = Schlange rechtwinklig & 45 grad 
 * 
 * Sprudel
 * sp1 = sprudel langsame Farbänderung
 * sp2 = sprudel bunt
 * 
 * 
 * verwendete GPIO Ports:
 *  GPIO 6 = LED_OUT         // Datenausgang zum Neopixelwürfel
 *  GPIO 13 = CLED          //interne LED blinkt alle 2 sek. wenn PRGM. läuft
*/

#include <FastLED.h>
#define NUM_LEDS 130            // How many leds in your strip? war 140
#define LED_OUT 6              // Datenausgang zum Neopixelwürfel                                // 
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB 
#define BRIGHTNESS 240
CRGB leds[NUM_LEDS];


#define CLED 13           // Arduino ATMega 2650 und UNO blinkt die interne LED alle 2 sek. wenn PRGM. läuft
                          
//int potpin = 0;            // analog pin used to connect the Potentiometer

//global variable
word flagr;  // Flagregister 
//Flags können gesetzt werden
//bit 0 = flag animation enable = 1,für halt und step mode,  bit 1 = flag demo aktiv,  1 = aktiv
//bit 2 = flag animation Farbwechsel an / aus = 0,           bit 3 = flag poti enable = 1, 0 ist Tastatursteuerung
//bit 4 = flag animationsrichtung - links oder rechtsrum     bit 5 = flag bunt = 1

// Flags für interne funktionen
//bit 15 = flag für funkel Leds                               bit 14 = flag koordinaten ok = 1 für aniwu2()
//bit 13 = flag Würfelanimation Farbwechsel 

byte wert1 = 0;     //werte vom seriellen Monitor, für setups, poti und Animationen usw.
byte wert2 = 0;      //  "
byte wert3 = 0;      //  "
byte ALed = 0;          // für Led an oder andere Farbe 
byte hue1 = 0;         //  Hintergrund Farbe für Leds, als aus Wert
byte sat1 = 0;        // 
byte valu1 = 0;       // 
byte hue2 = 0;         // Sekundär Farbe Vordergrund hell
byte sat2 = 0;        // 
byte valu2 = 0;       // 
byte hue3 = 0;         // Primär Farbe Vordergrund hell 
byte sat3 = 0;        // 
byte valu3 = 0;       //
byte sekco = 0;       // Sekundencounter
byte anima = 0;         // animations auswahl, welche anim gezeigt wird, 0 = keine
byte gesa = 0;       // für anim Geschwindigkeit 
word anico = 0;     //  animations counter - zählt die animationsdurchläufe
byte anig = 60;          // animations geschwindigkeit mit Poti / Tastatur einstellbar
word pot = 0;           //poti am analogeingang A0
byte pota = 60;          // alter wert von pot auf 0-255 gemapped
byte pots = 0;          // poti / Tastaturselekt, was gesteuert werden soll
byte arstp = 0;     // array startpunkt einer animation
byte ablco = 0;         // Ablaufcounter Animationen
byte ablco1 = 0;         // Ablaufcounter Animationen Würfel
byte anza = 4;      // Anzahl Animationen bis der Startpunkt wieder erreicht ist - hier 4 
byte lepra = 0;     // anzahl der Leds pro animation
byte aelp = 0;      //pointer für ael[] - Deuted auf die Speicherstelle, wo die aktuellen Leds 
                    // eingetragen und eingeschaltet werden sollen
byte alsm = 7;        //max. anzahl leuchtender Säulen
byte asa = 0;       // Säulenleds aus oder def. Farbe
byte ase = 0;        //Säulenleds ein oder neue Farbe
byte anima1 = 0;        // für Funkelanimationen
byte funkc = 0;         // counter für funkeln, abstand der blitze
byte funkl = 6;        // Anzahl der Leds die funkeln
byte funkf = 0;        // Funkel Farbe
byte funkz = 0;         // Funkel Zeit, wird runtergezählt alle 8ms, bei 0 kein funkeln mehr
byte scal = 8;      // Schlange länge
byte rws = 1;       // Startwert für Random generator
byte rwe = 7;       // Endwert für Random generator


// Die in den Arrays stehenden Zahlen sind die Led Nummern
//Säulenarray
byte asg[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,
              35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
              65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,
              95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,
              115,116,117,118,119,120,121,122,123,124};

 //Diagonale, doppeldiagonal, doppeldiagonal ausgefüllt,   
byte asd[] = {4,43,62,81,120,   //hinten links oben -> vorne rechts unten,      8 X 5er gruppen für diagonal
              14,38,62,86,110,  //mitte hinten oben -> mitte vorne unten
              24,33,62,91,100,  //hinten rechts oben -> vorne links unten
              50,56,62,68,74,   //mitte rechts oben -> mitte links unten
              0,41,62,83,124,   //vorne rechts oben -> hinten links unten
              10,36,62,88,114,  //mitte vorne oben -> mitte hinten unten 
              20,31,62,93,104,  //vorne links oben -> hinten rechts unten 
              54,58,62,66,70,  //mitte links oben -> mitte rechts unten      letzter Wert in 39 von 0 aus gezählt 
                                            
              4,43,62,81,120,0,41,62,83,124, 14,38,62,86,110,10,36,62,88,114,       // 4 x 10er gruppen doppeldiagonal
              24,33,62,91,100,20,31,62,93,104, 50,56,62,68,74,54,58,62,66,70,    //letzter Wert in 79 von 0 aus gezählt 
              
              0,1,2,3,4,43,42,41,62,81,82,83,120,121,122,123,124,        // 4 x 17er gruppen doppeldiagonal ausgefüllt
              10,11,12,13,14,36,37,38,62,86,87,88,110,111,112,113,114,
              20,21,22,23,24,31,32,33,62,91,92,93,100,101,102,103,104,
              70,71,72,73,74,66,67,68,62,56,57,58,50,51,52,53,54};

  // Translation Array, übersetzt Koordinaten in die LED Nummern - für Schlangenanimation
word atl1[] = {4,104,204,304,404,14,114,214,314,414,24,124,224,324,424,34,134,234,334,434,44,144,244,344,444,
                 3,103,203,303,403,13,113,213,313,413,23,123,223,323,423,33,133,233,333,433,43,143,243,343,443,
                 2,102,202,302,402,12,112,212,312,412,22,122,222,322,422,32,132,232,332,432,42,142,242,342,442,
                 1,101,201,301,401,11,111,211,311,411,21,121,221,321,421,31,131,231,331,431,41,141,241,341,441,
                 0,100,200,300,400,10,110,210,310,410,20,120,220,320,420,30,130,230,330,430,40,140,240,340,440,
                 // ab hier LED Ketten Nr.
                 4,9,14,19,24,49,44,39,34,29,54,59,64,69,74,99,94,89,84,79,104,109,114,119,124,
                 3,8,13,18,23,48,43,38,33,28,53,58,63,68,73,98,93,88,83,78,103,108,113,118,123,
                 2,7,12,17,22,47,42,37,32,27,52,57,62,67,72,97,92,87,82,77,102,107,112,117,122,
                 1,6,11,16,21,46,41,36,31,26,51,56,61,66,71,96,91,86,81,76,101,106,111,116,121,
                 0,5,10,15,20,45,40,35,30,25,50,55,60,65,70,95,90,85,80,75,100,105,110,115,120};  
//Umriss
byte umr[] = {0,1,2,3,4,9,14,19,24,23,22,21,20,15,10,5,45,25,70,50,95,75,49,29,54,74,99,79,
                100,101,102,103,104,109,114,119,124,123,122,121,120,115,110,105,  //Umriss des Würfels
                                                         //letzter Wert in 43 von 0 aus gezählt, lepra = 43
                57,62,67,37,87,63,61,      // kleines Kreuz in Mitte arstp = 44, lepra = 7
                61,62,63,42,32,92,82};      // kleines Kreuz in Mitte diagonal arstp = 51, lepra = 7
//
byte as4[] = {95,101,107,93,99, 15,31,27,23,19, 90,96,102,108,94, 20,16,32,28,24,   //Säulen 18,19,20,21,3,4,5,6
               105,91,97,103,109, 25,21,17,33,29, 100,106,92,98,104, 30,26,22,18,34}; // Diagonal
 

 
byte ael[50]; //  - array eingeschalteter Leds, 0-9 ebene0, 10-19 ebene1,.....
byte farr[32];    // array für funkeln mit mehreren LEDs
byte dea[6];              // demo array, damit nicht gleiche Zufallswerte hintereinander kommen
///////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(filn);                 // filename ausgeben
  FastLED.addLeds<LED_TYPE, LED_OUT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS ); // gilt für alle LEDS
  pinMode(CLED, OUTPUT);             //extern angeschlossene Led blinkt alle 2 sek. wenn PRGM. läuft
  pinMode(LED_OUT, OUTPUT);             //
  setfa();         //set alle Leds auf default Farbwerte
  //
  // Demo Mode einschalten
  anima = 0;
  anima1 = 0;
  flagr = 0x0007;        //bit 0 = flag animation enable = 1,    bit 1 = demo mode, 1 = aktiv
                        //bit 2 = flag animation Farbwechsel an / aus = 0, 
  anico = 0;          //animcounter  
  Serial.println();
  Serial.println(F("Im DEMO Mode, aus mit d"));                
}

///////////////////////////////////////////////////////////////////////////////////

void loop() {
  static unsigned long oldmillis;   // für millis prgm teil
  static unsigned long newmillis;   // für millis prgm teil
  static word tc;        //Timecounter 16bit  1ms zählt bis 65532 ms,für millis prgm teil
  word ttyb;            //Anzahl char im TTY Buffer
  byte ttyin;           //empfangener char.
  byte scratch;       // für Random 
   
    newmillis = millis();         // Programmteil millis
    if (newmillis != oldmillis) {             // ungleich - 
    ////////// jede millisekunde //////////////  
      oldmillis = newmillis;
      tc++;                      // increment timecounter jede millisekunde  
      //      
    // Prgm teil um zu bestimmten Zeiten weitere Programme auszuführen
    //         
             // Zeit Werte  ((tc & 0x????) == 0x????)
            //                0001 = alle 2ms         0003 = alle 4ms         0007 = alle 8ms                
            //                000F = alle 16ms        001F = alle 32ms        003F = alle 64ms              
            //                007F = alle 128ms       00FF = alle 256ms       01FF = alle 512ms               
            //                03FF = alle 1024ms      07FF = alle 2048ms      0FFF = alle 4096ms                
            //                1FFF = alle 8192ms      3FFF = alle 16xxxms     7FFF = alle 32xxxms                 
            //                FFFF = alle 65xxxms  
////////////////////////// alle 2 ms /////////////////////
       if ((tc & 0x0001) == 0x0001) {  // alle 2 ms
         //Hier werden die Animationen gestartet
         if (bitRead(flagr,0) == 1)  {       //flag bit 0, animation enable = 1, für halt und step mode  
            gesa ++;
            if (gesa == anig) {          // für animations geschwindigkeit
              anico ++;       //  animations counter - zählt die anaimationsdurchläufe
              anista();       // ruft die jeweiligen Animationen auf
              gesa = 0;
            }
         }        
       }// ende alle xx ms

////////////////////////// alle 8 ms /////////////////////
       if ((tc & 0x0007) == 0x0007) {  // alle x ms
          // Für Funkeln
          if (anima1 == 1) {
            if (funkc == 0)  {         // counter für funkeln, abstand der blitze
              anifu0();            // Funkeln 0 dauernd
            }
            else  {
              funkc--;
            }
          } 
          else if (anima1 == 2) {
            if (funkc == 0)  {         // counter für funkeln, abstand der blitze
              anifu1();           // Funkeln was dann verlöscht und aufhört
            }
            else  {
              funkc--;
            }
          }      
       }

////////////////////////// alle 16 ms /////////////////////
       if ((tc & 0x000f) == 0x000f) {  // alle xx ms       
        digitalWrite(CLED, LOW); // intern/extern angeschlossene Led ausschalten
       }// ende alle xx ms 
////////////////////////// alle 512 ms /////////////////////
       if ((tc & 0x01ff) == 0x01ff) {  // alle xx ms 
    
     /////////////// Serial Input zur Befehlseingabe über den seriellen Monitor ////////////     
        if (Serial.available() > 0) { //Eingabebuffer checken
          // im seriellen monitor muß no line ending eingestellt sein (unten neben Baudrate)
          ttyb = Serial.available();        //Anzahl char im Buffer 
          if (ttyb == 1)  {
            ttyin = Serial.read();           // lese ersten char. ein 
            switch (ttyin) {
             //
             case '0':              //alle Leds aus 
              Serial.println(F("Alle Leds aus"));
               FastLED.clear();
               FastLED.show();
              break;
             case '1':          //               
              if (scal < 13)  {
                scal ++;      // Schlange länger 
              }
              Serial.print(F("Schlange länger "));Serial.println(scal);
              break;             
             //
             case '2':          // 
              anima = 0;                   // animation aus
              if (scal > 6)  {
                scal --;      // Schlange kürzer 
              }
              anima = 5;                   // Schlangenanimation
              Serial.print(F("Schlange kürzer "));Serial.println(scal);
              break;                          
             //
             case '3':          //               
              if (funkl < 20) {
                funkl +=2;              // Anzahl der Leds die funkeln 
              }
              Serial.print(F("+ Funkelleds ")); Serial.println(funkl);              
              break;
             //
             case '4':          //              
              if (funkl > 2) {
                funkl -= 2;            // Anzahl der Leds die funkeln
              }
              Serial.print(F("- Funkelleds ")); Serial.println(funkl);               
              break;
              //
             case '5':          //              
              anig = 60;
              pota = anig; 
              Serial.print(F("Anim.Tempo def. ")); Serial.println(anig);               
              break;              //
              //
             case 'a':          //animation aus
              bitSet(flagr,0);      //bit 0, animation enable = 1, für halt und step mode
              anima = 0;                   // animation aus
              anima1 = 0;           //Funkelnd animations auswahl, welche anim gezeigt wird, 0 = keine
              pots = 0;                   // Poti aus
              farb1();                // Farbwert 1 für alle Leds
              anig = 60;          // animations geschwindigkeit auf default
              break;
              //
             case 'b':        //toggle bunt an/aus 
              Serial.print(F("bunt "));           
              if (bitRead(flagr,5) == 1)  {       //bit 5 = flag bunt an / aus = 0,
                bitClear(flagr,5);      //
                Serial.println(F("aus"));
              }
              else if (bitRead(flagr,5) == 0)  {
                bitSet(flagr,5);      //
                Serial.println(F("ein"));
              }               
              break;             
              //
             case 'H':          //Helligkeit increment               
              if (valu3 < 238)  {
                valu3 += 16;            
                valu2 += 16;       //                
              } 
              Serial.print(F("Helligkeit "));Serial.println(valu3);             
             break; 
             //            
             case 'h':          //brightness decrement 
             if (valu3 > 15)  {
              valu3 -= 16;            
               valu2 -= 16;       //               
              } 
              Serial.print(F("Helligkeit "));Serial.println(valu3);              
             break;             
              //
             case 'D':          //  
              Serial.println(F("Demo ein")); 
              anima = 0;
              anima1 = 0;
              bitSet(flagr,1);      //flag bit 1 = demo mode, 1 = ein 
              anico = 0;          //animcounter
              farb1();                // Farbwert 1 für alle Leds
              break;
             //
             case 'd':          // 
              Serial.println(F("Demo aus")); 
              bitClear(flagr,1);      //flag bit 1 = demo mode, 0 = aus          //
              anima = 0;
              anima1 = 0;
              farb1();                // Farbwert 1 für alle Leds
              break;
              //
             case 'F':          // setzt default Farbwerte
              bitClear(flagr,5);        //bunt aus
              setfa();         //set alle Leds auf default Farbwerte                //
              Serial.println(F("Def. Farben"));
              break;
              //
             case 'G':                //  Animationsgeschwindigkeit mit Poti oder Tastatur einstellen
                                      //- kleinere Werte = höhere Geschwindigkeit
             Serial.println(F("Ani.Tempo adj."));    // Geschwindigkeit mit Poti / Tastatur
              pots = 20;
              pota = anig;           
              break;
             //
             case 'g':          // Poti oder Tastatur einstellen aus
              Serial.println(F("Ani.Tempo aus")); 
              pots = 0;
              break;
              //              
             case 'i':
              Serial.println();
              Serial.print(F("Info: "));Serial.println(filn); 
              Serial.print(F("flagr "));Serial.println(flagr, BIN);                                         
              Serial.print(F(" Anim Nr. ")); Serial.print(anima); // welche Animation läuft                           
              Serial.print(F("\tGeschw. "));Serial.println(anig); // Animationsgeschwindigkeit
              prifa();    
              break;
              //
             case 'n':        //toggle langsamer Farbwechsel an/aus 
              Serial.print(F("Farbwechsel "));           
              if (bitRead(flagr,2) == 1)  {       //bit 2 = flag animation Farbwechsel an / aus = 0,
                bitClear(flagr,2);      //
                Serial.println(F("aus"));
              }
              else if (bitRead(flagr,2) == 0)  {
                bitSet(flagr,2);      //
                Serial.println(F("ein"));
              }               
              break;
             //
             case 'r':          //Toggle Richtung 
              Serial.println(F("toogle Richtung")); //
              if (bitRead(flagr,4) == 1)  {       //bit 4 flag animationsrichtung - links oder rechtsrum
                bitClear(flagr,4);      //
              }
              else if (bitRead(flagr,4) == 0)  {
                bitSet(flagr,4);      //
              }
              break;             
             // 
             case 'S':          //  Animation Halt
             Serial.println(F("Halt Anim.")); 
              bitClear(flagr,0);      //bit 0, animation enable = 1, für halt und step mode
              break;
             //
             case 's':          
             Serial.println(F("Step Anim.")); 
              anista();       // ruft die jeweilige Animation einmal auf
              break;
             //
             case 'w':          // Animation weiter
              Serial.println(F("Anim. weiter")); 
              bitSet(flagr,0);      //bit 0, animation enable = 1, für halt und step mode
              break;
             //
             case 't':          // test
              Serial.println(F("Test"));
              anima = 4; 
              break;
              //
             case '+':          // Tastaturpoti +1
              pota ++;
              poti(); 
              break;
              //
             case '-':          // Tastaturpoti -1
              pota --;
              poti(); 
              break;
              //            
             case 'P':          // Tastaturpoti +15
              pota += 15;
              poti(); 
              break;
              //
             case 'M':          // Tastaturpoti -15
              pota -= 15;
              poti(); 
              break;
              //                        
            }
           }//ende if (ttyb == 1)

            else if (ttyb == 3)  {               // für 3 char. Befehle
              for (byte i = 0; i < 4; i++)  {
                switch (i)  {
                  case 0:
                   wert1 = Serial.read();           // lese ersten char. ein
                   break;
                  case 1:
                   wert2 = Serial.read();           // lese 2. char. ein
                   break;
                  case 2:
                   wert3 = Serial.read();           // lese 3. char. ein
                   break;
                }            
              }
               Serial.print(F("Serialin: "));Serial.write(wert1);
               Serial.write(wert2);Serial.write(wert3); Serial.println();
  
            inp3();          // Input bei 3 char  
                                     
            } // ende if (ttyb == 3)
          //          
            else {     //es wurden mit send weniger oder mehr als 3 ASCII Zeichen übertragen
              Serial.println(F("Fehler <>3 char.input"));    
              while (Serial.available() > 0) {
              Serial.read(); //ttyinbuffer leer machen                                    
              }
            }           
          }//ende if (Serial.available()
         
        }// ende alle 512ms

////////////////////////// alle 1024 ms /////////////////////
       if ((tc & 0x03ff) == 0x03ff) {  // alle x ms 
        if (sekco > 0)  {       // Sekundencounter für Demo Mode
          sekco --;
        } 
        else if (bitRead(flagr,1) == 1)  {       //flag bit 1 = demo mode, 1 = ein 
          demo();            // Demo mode 
        }
        scratch = random8(0,255);
       }// ende alle xx ms
      
////////////////////////// alle 2048 ms /////////////////////
       if ((tc & 0x07ff) == 0x07ff) {  // alle x ms 
        digitalWrite(CLED, HIGH); // intern/extern angeschlossene Led an einschalten
       }// ende alle xx ms
///////////////////////////////////////////////////////////////////////////////////
            
    }// ende if (newmillis != oldmillis) 
        
}// ende void loop

//###########################################################################################

////////////////////////// AB hier Hilfsprogramme ////////////////////////////////////

void anista() {         //von hier werden die einzelnen Animationen aufgerufen
  switch (anima)  {
    case 0:             // keine animation
     anico = 0;
     break;                
    case 2:          //
     anispru();            //Sprudel / Blasen  Animation
     break; 
    case 4:
     test();        // Testprgm.
     break;
    case 5:
     anisc0();        // Schlange
     break;
    case 7:
     anisl0();      // Animationen Säulen mit Schrägsäulen
     break;
    case 8:
     anisl1();         // Animationen Säulen gerade
     break;
    case 9:
     anisl2();     //Farb animation mit 4er Säulen Wendel
     break;
    case 10:
     figu1();           // Figur Würfel Umriss Farbanimation
     break;
     case 11:
      figu2();      // Figur Würfel Umriss mit inneren Würfel Farbanimation
     break;
    default:
    anico = 0;
    break;             
  }// ende switch      
}

//###########################################################################################
void setfa()  {       //set default Farbwerte
  FastLED.setBrightness( BRIGHTNESS ); // gilt für alle LEDS  
  hue1 = 169;         // variable für HUE1  blau        Hintergrund oder Led aus
  sat1 = 180;        // variable für Saturation1          
  valu1 = 44;       // variable für Value1        // je nach Variante des WS2812 von           
  farb1();                                        // 26 bis 46, Leds sollten etwas glimmen
  hue2 = 255;         // variable für HUE -rot      Sekundär Farbe Vordergrund 
  sat2 = 220;        // variable für Saturation
  valu2 = 112;       // variable für Value
  hue3 = 120;         // variable für HUE (grün-blau)     primär Farbe Vordergrund 
  sat3 = 220;        // variable für Saturation
  valu3 = 112;       // variable für Value 
}
///////////////////////////////////////////////////////////////////////////////////

void farb1()  {           //alle Leds auf aus Farbwerte schalten, sollen dann nur glimmen
  for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue1, sat1, valu1);
  }
  FastLED.show();
}
///////////////////////////////////////////////////////////////////////////////////////////

void richt()  {     // Animations richtung & Farben für Säulen, Würfel 
  if (bitRead(flagr,4) == 0)  {       //bit 4 flag animationsrichtung - links oder rechtsrum    
    if (ablco1 < anza)  {           // anza = anzahl animationen
      ablco1 ++;
    }
    else  {
      ablco1 = 0;
      neufa();     
    }    
  }
  else  {
    if (ablco1 > 0)  {
      ablco1 --;
    }    
    else  {
      ablco1 = anza;
      neufa();
    }
  } 
}
////////////////////////////////////////////////////////////////////////////////////////
void neufa()  {         //  Neue Farbe, sorgt für langsame laufende LED Farbenänderung
  //static byte fdiff;          // für Farbdifferenz
  static byte nefac;         // neufa counter zählt Durchläufe
  static byte neuvar;
//
  if (bitRead(flagr,2) == 1)  {       //bit 2 = flag animation Farbwechsel an / aus = 0,
    hue3 +=7;         //  pro Durchlauf wird hue3 erhöht  
    //-------------------------------------------------------------------------      
      for (byte fdiff = 0; fdiff < 31; fdiff ++) {   //sorgt für einen Farbabstand Hue3 - Hue2
        neuvar = hue3 + fdiff;      
        if (neuvar == hue2) {
          hue3 = (hue2 + 16);
          hue2 -= 15;
          fdiff = 31;         
        }            
      }
    //-------------------------------------------------------------------------
    if ( nefac < 30)  {
      nefac ++;             // neufa counter zählt Durchläufe 
    }
    else  {                 //Led aus Farbe Hue1 wird erhöht
      nefac = 0;
      hue1 +=11;         // 
    } 
  }
}

////////////////////////////////////////////////////////////////////////////////////////
void prifa()  {             // Serial print farbe
  Serial.print(F("H1 ")); Serial.print(hue1); Serial.print(F(" H2 ")); Serial.print(hue2); 
  Serial.print(F(" H3 ")); Serial.println(hue3); 
  Serial.print(F("S1 ")); Serial.print(sat1); Serial.print(F(" S2 ")); Serial.print(sat2); 
  Serial.print(F(" S3. ")); Serial.println(sat3); 
  Serial.print(F("V1 ")); Serial.print(valu1); Serial.print(F(" V2 ")); Serial.print(valu2); 
  Serial.print(F(" V3 ")); Serial.println(valu3);
}

////////////////////////////////////////////////////////////////////////////////////////
void poti() {
// pot = analogRead(potpin);    // reads the potentiometer (value between 0 and 1023)
// pot = map(pot, 0, 1023, 0, 255);     // scale it value between 0 and 255
 
//  if (pot != pota)  {
//    pota = pot;
    Serial.print(F("P/T Wert ")); Serial.println(pota);
    switch (pots) {       
      case 1:
       hue1 = pota;
      break;
      //
      case 2:
       sat1 = pota;        
      break;
      //      
      case 3:
       valu1 = pota; 
      break;
      //
      case 4:
       hue2 = pota;
      break;
      //
      case 5:
       sat2 = pota;
      break;
      //     
      case 6:
       valu2 = pota;
      break;
      // 
      case 7:
       hue3 = pota;
      break;
      //
      case 8:
       sat3 = pota;
      break;
      //     
      case 9:
       valu3 = pota;
      break;
      //       
      case 20:           //Geschwindigkeit Animation mit Tastatur / Poti einstellen
        anig = pota;
      break;
      //    
    }// ende switch    
  //} 
}
////////////////////////// Ende Hilfprogramme ////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
void demo()  {     // Demo mode    - abhängig vom sekunden counter werden Animationen per Zufall gestartet      
  sekco = 8;          // Sekunden counter
  byte demco = 0;
  byte v1 = 0;
  byte a1;
  byte a2;

  do {
    demco = random8(0,17);           // random demo 
    for( a1 = 0; a1 < 6; a1++) {
      v1 = dea[a1];              //  - demo array 
      if (v1 == demco)  {
        demco = 255;         
      }
    }
  } while (demco == 255);
  
  // fifo schiebt inhalt vom array um eins nach oben
  a1 = 6;
  do  {
    a2 = a1;
    a1 --;
    v1 = dea[a1];              //  - demo array hole arrayeintrag mit arraypointer 
    dea[a2] = v1;             //speicher v1 im array mit arraypointer2
  } while (a1 > 0);
  // ende fifo
  dea[a1] = demco;          // neuen wertim array abspeichern
  if (demco < 255)  {
    Serial.print(F("DEMO "));Serial.print(demco);Serial.print("\t");
  }
  switch (demco) {
     //
     case 0:
      sekco = 3;          // Sekundencounter          
      anima = 0;                   // animation aus
      farb1();                // Farbwert 1 für alle Leds 
      wert1 = 'n';  wert2 = 'i';  wert3 = 'x';   // dummy       
     break;
     //     
     case 1: 
        sekco = 10;          // Sekundencounter        
        wert1 = 'f';  wert2 = 'i';  wert3 = '2';   //Würfel Umriss ein mit Farbanimation  
     break;
     //
     case 2: 
        wert1 = 's';  wert2 = 'l';  wert3 = '1';   //Säule 1 Doppel Schrägsäule
      break;
     // 
     case 3:   
        //drehrichtung ändern 
        if (bitRead(flagr,4) == 1)  {       //bit 4 flag animationsrichtung - links oder rechtsrum
          bitClear(flagr,4);      //
        }
        else if (bitRead(flagr,4) == 0)  {
          bitSet(flagr,4);      //
        }        
        wert1 = 's';  wert2 = 'l';  wert3 = '2';   //Säule 2 Doppel Schrägsäule ausgefüllt
      break;
     //
     case 4: 
      sekco = 4;          // Sekundencounter geändert
        wert1 = 's';  wert2 = 'l';  wert3 = '4';   //Säule 4 umlaufend
      break;
     //
     case 5: 
        wert1 = 's';  wert2 = 'l';  wert3 = '8';   // säulen anim diagonal
      break;
     // 
     case 6:
        anima = 0;
        sekco = 12;          // Sekundencounter geändert
        wert1 = 's';  wert2 = 'c';  wert3 = '2'; //   = Schlange 45 grad 
      break;
      //
     case 7:
        sekco = 10;          // Sekundencounter geändert
        wert1 = 's';  wert2 = 'p';  wert3 = '1'; //Sprudel langsame Farbänderung
      break;
      // 
     case 8:
        anima = 0;           // animationen aus
        farb1();                // Farbwert 1 für alle Leds
        sekco = 4;          // Sekundencounter geändert
        wert1 = 'f';  wert2 = 'u';  wert3 = 'w'; //fuw Funkeln weiss kurz,
      break;
      //
     case 9: 
        wert1 = 's';  wert2 = 'p';  wert3 = '2'; //Sprudel bunt 
      break;
      //       
     case 10:
        anima = 0;
        sekco = 4;          // Sekundencounter geändert
        wert1 = 'f';  wert2 = 'u';  wert3 = 'g'; //Funkeln grün kurz,
      break;
      //
     case 11:
        anima = 0;
        sekco = 12;          // Sekundencounter geändert
        wert1 = 's';  wert2 = 'c';  wert3 = '1'; //   = Schlange rechtwinklig 
      break;
      //
     case 12:
        sekco = 4;          // Sekundencounter geändert
        wert1 = 'f';  wert2 = 'u';  wert3 = 'b'; //Funkeln blau kurz,
      break;
      //
     case 13:
        sekco = 4;          // Sekundencounter geändert
        wert1 = 'f';  wert2 = 'u';  wert3 = 'y'; //Funkeln gelb kurz,
      break;
      //
      //
     case 14:               // standart Farben oder bunte Leds
     sekco = 4;          // Sekundencounter     
      Serial.print(F("bunt "));           
      if (bitRead(flagr,5) == 1)  {       //bit 5 = flag bunt an / aus = 0,
        bitClear(flagr,5);      //
        Serial.print(F("aus  "));
      }
      else if (bitRead(flagr,5) == 0)  {
        bitSet(flagr,5);      //
        Serial.print(F("ein  "));
      } 
       wert1 = 'f';  wert2 = 'u';  wert3 = 'B'; //Funkeln bunt kurz,
     break;
      //
     case 15:
        anima = 0;
        sekco = 12;          // Sekundencounter geändert
        wert1 = 's';  wert2 = 'c';  wert3 = '3'; //   = Schlange rechtwinklig und 45 grad 
      break;
      //
     case 16: 
        sekco = 10;          // Sekundencounter        
        wert1 = 'f'; wert2 = 'i'; wert3 = '3';   //Figurumriss Würfel und kleines Drehkreuz 
                                                    // in Mitte mit Farbanimation 
     break;
     //
     default:
     demco = 0;
     anico = 0;
     break;  
  }//ende switch

  inp3();  
}//ende void demo
////////////////////////// Ab hier Animationsprogramme ////////////////////////////

void test()  {                        // Testprgm.
  if (anico == 6) {     //
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB( 40, 40, 40);
    }
   FastLED.show();    
  }
  else if (anico > 12) {     //
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB( 90, 90, 90);
    }
   anico = 0;
   FastLED.show(); 
  }   
}
///////////////////////////////////////////////////////////////////////////////////

void figu1()  {                         // Würfelumriss
  if (anico > 40) {     //
    for( byte i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(hue1, sat1, valu1);
    }
    lepra = 44;
    valu3 -= 30;        // Helligkeit etwas runter

  if (bitRead(flagr,5) == 0)  {       //bit 5 = flag, bunt = 1    
    for (byte i = 0; i < lepra; i ++) {   
      ALed = umr[i]; // 
      leds[ALed] = CHSV(hue3, sat3, valu3);
    }
  }
  else  {
    for (byte i = 0; i < lepra; i ++) {   
      ALed = umr[i]; // 
      leds[ALed] = CHSV((random8(10,255)), sat3, valu3);
    }    
  }
    anico = 0;
    neufa();
    FastLED.show();
    valu3 += 30;        // Helligkeit default
  } 
}
///////////////////////////////////////////////////////////////////////////////////

void figu2()  {                         // Würfelumriss und kleines Kreuz in Mitte
  static byte var1;
  
  if (anico > 30) {     //
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(hue1, sat1, valu1);
    }
    lepra = 44;
    valu3 -= 30;;        // Helligkeit etwas runter
    for (byte i = 0; i < lepra; i ++) {   
      ALed = umr[i];                      // Würfelumriss
      leds[ALed] = CHSV(hue3, sat3, valu3);
    }
    if (var1 > 0) {
      arstp = 44; lepra = 7;
      for (byte i = 0; i < lepra; i ++)  {   // kleines Kreuz in Mitte arstp = 153, lepra = 7
        ALed = umr[arstp + i];  
        leds[ALed] = CHSV(hue2, sat2, valu2);
      }
      var1 = 0;
    }
    else  {
      arstp = 51; lepra = 7;
      for (byte i = 0; i < lepra; i ++)  {     // kleines Kreuz in Mitte diagonal arstp = 160, lepra = 7
        ALed = umr[arstp + i];  
        leds[ALed] = CHSV(hue2, sat2, valu2);
      } 
      var1 = 1;        
    }
    anico = 0;
    neufa();
    FastLED.show();
    valu3 += 30;        // Helligkeit default
  } 
}

////////////////////////////////////////////////////////////////////////////////////////
void anisl0() {   // Animationen Säulen mit Schrägsäulen 
  for (byte i = 0; i < lepra; i ++)  {   // schaltet vorige Säule aus oder default
    ALed = asd[arstp + (ablco1 * lepra + i)]; // arstp = array startpunkt einer animation, schrägsäulen = 0, 
                                                // doppelschrägsäulen = 40
                                                // lepra = anzahl der Leds pro animation, schrägsäulen = 5,
                                                 //doppelschrägsäulen = 10, ausgefüllt = 17
    leds[ALed] = CHSV(hue1, sat1, valu1);
  } 
  richt();            //animationsrichtung - links oder rechtsrum  
  if (bitRead(flagr,5) == 0)  {       //bit 5 = flag, bunt = 1
    for (byte i = 0; i < lepra; i ++)  {   // schaltet neue Säule ein
      ALed = asd[arstp + (ablco1 * lepra + i)]; // 
      leds[ALed] = CHSV(hue3, sat3, valu3);
    }
  }
  else  {
    for (byte i = 0; i < lepra; i ++)  {   // schaltet neue Säule ein bunt
      ALed = asd[arstp + (ablco1 * lepra + i)]; // 
      leds[ALed] = CHSV((random8(10,255)), sat3, valu3);
    }    
  }
  FastLED.show(); 
}
///////////////////////////////////////////////////////////////////////////////////////////
void anisl1() {           // Animationen Säulen gerade
    //im array sind die Säulen Nummern, die in dieser reihenfolge aufgerufen werden 
  byte ani1[] = {0,1,2,3,4,5,14,15,24,23,22,21,20,19,10,9,8,7,6,13,16,17,18,11,12,8, //umlaufend dann nach innen,  25 letzte
                 0,1,2,3,4,5,14,15,24,23,22,21,20,19,10,9,                   //anzahl im array  1-16 - umlaufend, 41 letzte
                 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,   //zick-zack, 66 letzte
                 8,11,18,17,16,13,6,7,                                       // umlaufend innen, 74 letzte
                 0,8,12,16,24,23,22,21,20,18,12,6,4,3,2,1};                   // diagonal, 90 letzte
  if (ablco == 0) {       // INIT
    farb1();           //alle Leds auf eine Farbe einschalten     
    for (aelp = 0; aelp < alsm; aelp ++) {   //  array eingeschalteter leds löschen
      ael[aelp] = 0;
    }
   ablco = 1;
   aelp = 0;           //  array pointer für ael [] 
   ablco1 = 0;        //Ablaufcounter Säulen Animationen
  }  
//--------------------------------- 
  richt();            //animationsrichtung - links oder rechtsrum  
  ase = ani1[arstp + ablco1];   // im ase steht die säulen nr. die eingeschaltet werden soll
                                //arstp = array startpunkt einer animation,
  aelp ++;              // Pointer auf ältesten Led Wert setzen
  if (aelp == alsm ) {      // umlaufpointer max. wert =  asm = max. anzahl leuchtender Säulen 
    aelp = 0;           // 
  }
  asa = ael[aelp];      //  ältesten wert nach asa = Säule aus oder def. Farbe
  ael[aelp] = ase;       // neuen Wert abspeichern, da war vorher der älteste wert      
//
//                  Leds der neuen Säule einschalten, Leds der ältesten Säule aus- oder default schalten

  if (bitRead(flagr,5) == 0)  {       //bit 5 = flag, bunt = 1
    for (byte i = 0; i < 5; i ++)  {   // schaltet neue Säule ein
      ALed = asg[ase * 5 + i];
      leds[ALed] = CHSV(hue3, sat3, valu3);
    }
  }
  else  {  
    for (byte i = 0; i < 5; i ++)  {   // schaltet neue Säule ein bunt
      ALed = asg[ase * 5 + i];
      leds[ALed] = CHSV((random8(10,255)), sat3, valu3);
    }
  } 
  for (byte i = 0; i < 5; i ++)  {   // schaltet älteste Säule aus oder default
    ALed = asg[asa * 5 + i];
    leds[ALed] = CHSV(hue1, sat1, valu1);
  }  

  FastLED.show();  
}

///////////////////////////////////////////////////////////////////////////////////
void anisl2()  {      //Farb animation mit 4er Säulen Wendel
  for (byte i = 0; i < 10; i ++)  {   // schaltet vorige Säule auf default
    ALed = as4[i + arstp];
                                              // arstp = array startpunkt einer animation
    leds[ALed] = CHSV(hue2, sat2, valu2);
  }
  if (arstp < 30)  {
    arstp += 10;
  }
  else  {
    arstp = 0;
  }
  for (byte i = 0; i < 10; i ++)  {   // 
    ALed = as4[i + arstp];
                                              // arstp = array startpunkt einer animation
    leds[ALed] = CHSV(hue3, sat3, valu3);
  }
  FastLED.show();

   if (anico == 50) {     //  animations counter - zählt die anaimationsdurchläufe 
      neufa();
    anico = 0;     
   }
}
///////////////////////////////////////////////////////////////////////////////////
void anifu0() {           // Funkeln dauernd
  if (bitRead(flagr,15) == 1) {   //bit 15 = flag für funkel Leds  
    for (byte i = 0; i < funkl; i++) {            //  funkl = Anzahl der Leds die funkeln
      ALed = random8(NUM_LEDS);
      switch (funkf)  {
        case 0:
          leds[ALed] = CRGB::White;      //Einschalten
          leds[ALed].maximizeBrightness();
        break;
        case 1:
          leds[ALed] = CRGB::Yellow;      //Einschalten 
          leds[ALed].maximizeBrightness();        
        break;         
        case 2:
          leds[ALed] = CRGB::Red;      //Einschalten
          leds[ALed].maximizeBrightness();
        break; 
        case 3:
          leds[ALed] = CRGB::Green;      //Einschalten
          leds[ALed].maximizeBrightness();
        break; 
        case 4:
          leds[ALed] = CRGB::Blue;      //Einschalten
          leds[ALed].maximizeBrightness();
        break; 
        case 5:
          //bunt
          leds[ALed] = CHSV((random8(10,255)), sat3, 255);
          leds[ALed].maximizeBrightness();
        break;
      }     
      farr[i] = {ALed};           // in array einlesen
    }
    bitClear(flagr,15);      //bit 15 = flag für funkel Leds 
  }
  else  {
    for (byte i = 0; i < funkl; i++) {            //
      ALed = farr[i];           // aus array wieder rausholen
      leds[ALed] = CHSV(hue1, sat1, valu1);      //Ausschalten / default 
    }
    bitSet(flagr,15);      // bit 15 = flag für funkel Leds
    funkc = 10;         // counter für funkeln, abstand der blitze ca. 80 ms
  } 
  FastLED.show();
}
////////////////////////////////////////////////////////////////////////////////////////

void anifu1() {           // Funkeln was dann weniger wird und aufhört 
  if (bitRead(flagr,15) == 1) {   //bit 15 = flag für funkel Leds 
    for (byte i = 0; i < funkl; i++) {             //  funkl = Anzahl der Leds die funkeln 
      ALed = random8(NUM_LEDS);
      switch (funkf)  {
        case 0:
          leds[ALed] = CRGB::White;      //Einschalten
          leds[ALed].maximizeBrightness();
        break;
        case 1:
          leds[ALed] = CRGB::Yellow;      //Einschalten
          leds[ALed].maximizeBrightness();
        break;         
        case 2:
          leds[ALed] = CRGB::Red;      //Einschalten
          leds[ALed].maximizeBrightness();
        break; 
        case 3:
          leds[ALed] = CRGB::Green;      //Einschalten
          leds[ALed].maximizeBrightness();
        break; 
        case 4:
          leds[ALed] = CRGB::Blue;      //Einschalten
          leds[ALed].maximizeBrightness();
        break; 
        case 5:
          //bunt
          leds[ALed] = CHSV((random8(10,255)), sat3, 255);
          //leds[ALed].maximizeBrightness();
        break;
      }     
      farr[i] = {ALed};           // in array einlesen
    }
    bitClear(flagr,15);      //bit 15 = flag für funkel Leds 
  }
  else  {
    for (byte i = 0; i < funkl; i++) {            //
      ALed = farr[i];           // aus array wieder rausholen
      leds[ALed] = CHSV(hue1, sat1, valu1);      //Ausschalten / default 
    }
    bitSet(flagr,15);      // bit 15 = flag für funkel Leds
    funkc = 5;         // counter für funkeln, Einschaltdauer blitze ca. 40 ms
  } 
  FastLED.show();
  //
  if (funkz > 0)  {
    funkz --;                 // Funkel Zeit, wird runtergezählt, bei 0 kein funkeln mehr & animation aus 
  }
  if (bitRead(flagr,15) == 1) {   //bit 15 = flag für funkel Leds
    switch (funkz)  {
      case 60:
        funkl = 10;               //  funkl = Anzahl der Leds die funkeln
     break;
      case 50:
        funkl = 8;               //  funkl = Anzahl der Leds die funkeln
     break;
      case 40:
        funkl = 6;               //  funkl = Anzahl der Leds die funkeln
     break;
      case 30:
        funkl = 5;               //  funkl = Anzahl der Leds die funkeln
     break;
      case 20:
        funkl = 4;               //  funkl = Anzahl der Leds die funkeln
     break;  
      case 5:
        funkl = 2;               //  funkl = Anzahl der Leds die funkeln
     break;  
      case 0:
        anima1 = 0;           // animation aus
     break;  
    }
  }
}


/////////////////////////////////////////////////////////////////////////////////

void anispru() {           //Sprudel / Blasen  Animation

  static byte rnled;      //Random Led Nr. - Sprudel
  static byte fueco;      //fünfer counter - Sprudel 
  
  if (ablco == 0) {       // INIT
    rnled = 0;      //Random Led Nr. - Sprudel
    fueco = 0;      //fünfer counter - Sprudel
    for (byte i = 0; i < 50; i++) {   //  array  alle werte auf xx setzen
      ael[i] = 125;
    }
    ablco = 1;
  }
  //
  else if (ablco == 1) {       //     
//
   for(aelp = 40; aelp < 49; aelp ++)  {               //ebene 4 Leds aus
      ALed = ael[aelp];
      leds[ALed] = CHSV(hue1, sat1, valu1);   //
    }  
//
   for(aelp = 30; aelp < 40; aelp ++)  {               //ebene 3 Lednummern + 1 nach ebene 4
      ALed = ael[aelp];
      leds[ALed] = CHSV(hue1, sat1, valu1);   //
      ALed ++;
      ael[aelp + 10] = ALed;
    }
//
   for(aelp = 20; aelp < 30; aelp ++)  {               //ebene 2 Lednummern + 1 nach ebene 3
      ALed = ael[aelp];
      leds[ALed] = CHSV(hue1, sat1, valu1);   //
      ALed ++;
      ael[aelp + 10] = ALed;
    }
//
   for(aelp = 10; aelp < 20; aelp ++)  {               //ebene 1 Lednummern + 1 nach ebene 2
      ALed = ael[aelp];
      leds[ALed] = CHSV(hue1, sat1, valu1);   //
      ALed ++;
      ael[aelp + 10] = ALed;
    } 
//
   for(aelp = 0; aelp < 10; aelp ++)  {                 //ebene 0 Lednummern + 1 nach ebene 1 
      ALed = ael[aelp];
      leds[ALed] = CHSV(hue1, sat1, valu1);   //
      ALed ++;
      ael[aelp + 10] = ALed;
    }
// 
//------------------------ Zufallsgenerator füllt ael() 0-9 --------------------
    for (byte i = 0; i < 10; i++) {   // zuerst array 0-9 werte auf xx setzen
      ael[i] = 125;
    }  
  aelp = 0;                    // arraypointer auf 0 
  lepra = random8(0,4);
  for (byte l = 0; l < lepra; l++)  {
    fueco = -5;
    while (rnled != fueco)  {
      fueco += 5;
      rnled = random8(0,125);
      if (fueco == 125)  {
        fueco = 0;       
      }       
   } //ende while (rnled != fueco)
  //check ob die Led nr. schon im array 0-9 ist
  for (byte i = 0; i < 10; i++) {   // zuerst array 0-9 werte
    ALed = ael[i];
    if (ALed == rnled)  {      //wenn gleich, dann schon vorhanden
      rnled = 150;
      break;
    }
  }
  if (rnled == 150)  {
    l --;
  }
  else  {
  ael[aelp] = rnled;    // ins array 0 - 9 eintragen
  aelp ++;
  }
 }// ende for (byte l = 0; l < lepra; l++)
//---------------------------------------------------------------------
   for(aelp = 0; aelp < 49; aelp ++)  {      //
    ALed = ael[aelp]; 
    if (bitRead(flagr,14) == 1)  {       //bit 14 koordinaten ok = 1 für Random Farben 
      hue3 = random8(0,255);           // random value1 = random number
    }  
    leds[ALed] = CHSV(hue3, sat3, valu3);   // 
   }
  }// ende if ablco = 1
  FastLED.show();
   if (anico == 100) {  // animations counter - zählt die anaimationsdurchläufe 
      neufa();          // für Farbänderungen
    anico = 0;
   }       
}// ende void anispru()

/////////////////////////////////////////////////////////////////////////////////
void anisc0() {  // ########## Animation Schlange 45 grad & rechtwinklig Abbiegen ####################
  // 
  // es gibt 2 arrays das   atl1[]    zum Übersetzen der Led Position 
  //              und das   ael[]   da sind die Leds eingetragen, die gerade leuchten. 
  static byte lrv1;    // local random value 1
  static word lrv2;    // local random value 2
  static byte lrrd;    // local random richtungs dauer
  static byte lrrdi;       // Random richtungs dauer input 0 bis 4  
  static byte lrvlc;      //Random value - loop counter
  static byte var1;        //variable
  static byte scbe;          // Schlange beisst sich counter 
  static word varw1;     // Wordvariable für alles mögliche
  static byte scap;      // Schlange pointer 
  static byte lxs;      //Schlange position
  static byte lys;      //Schlange position
  static byte lzs;      //Schlange position
  static byte lxsa;      //Schlange position alt
  static byte lysa;      //Schlange position alt
  static byte lzsa;      //Schlange position alt 
  byte ALedo;         // Led aus oder default Farbe
  byte ALedk;        // für Led an oder andere Farbe            
  if (ablco == 0) {       // INIT
    lrrdi = 0;       // Random richtungs dauer input 0 bis 4
    scbe = 0;          // Schlange beisst sich counter
    farb1();  //alle Leds auf default Farbwerde einschalten HUE1,SAT1,VALU1)
    scap = 0;           // Schlange array pointer
    for (byte i = 0; i < scal; i++) {   // Schlange array löschen, scal = Schlange länge
      ael[i] = 0;
    } 
    lrrd = 1;                // Random Richtung dauer
    lxs = 0; lys = 0; lzs = 0;
  }
   
  do  {   //ablco = 2
    ablco = 1;
    lxsa = lxs; lysa = lys; lzsa = lzs;     //  x,y,z werte sichern  
      
    do  {   //ablco = 1
      if (lrvlc >= 5) {                // 
        lrrd = 1;                // Random Richtung dauer
        lrvlc = 0;               //Random value - loop counter
      }
      if (lrrd == 1) {                // Random Richtung dauer
        lrv1 = random8(rws,rwe);           // random value1 = random number
                                        // rws - Startwert für Random generator
                                        // rwe - Endwert für Random generator            
      }                           // increment oder decrement, xs, ys, zs  
      //    
      switch (lrv1)  {         // im switch / case check, das die Schlange im Würfel bleibt
        case 1:
          if (lxs < 4)  {
            lxs ++;
            ablco ++;         // Ablaufcounter Animationen
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //
        case 2:
          if (lxs > 0)  {
            lxs --;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //
        case 3:
          if (lys < 4)  {
            lys ++;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 4:
          if (lys > 0)  {
            lys --;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 5:
          if (lzs < 4)  {
            lzs ++;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 6:
          if (lzs > 0)  {
            lzs --;
            ablco ++;         // Ablaufcounter Animationen
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //
        case 7:
          if (lxs < 4 && lys < 4)  {           
            lxs ++;
            lys ++;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //
        case 8:
          if (lxs > 0 && lys > 0)  {
            lxs --;
            lys --;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //
        case 9:
          if (lxs < 4 && lys > 0)  {
            lxs ++;
            lys --;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 10:
          if (lxs > 0 && lys < 4)  {
            lxs --;
            lys ++;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 11:
          if (lxs < 4 && lzs < 4)  {
            lxs ++;
            lzs ++;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 12:
          if (lxs > 0 && lzs > 0)  {
            lxs --;
            lzs --;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 13:
          if (lys < 4 && lzs < 4)  {
            lys ++;
            lzs ++;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 14:
          if (lys > 0 && lzs > 0)  {
            lys --;
            lzs --;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 15:
          if (lxs < 4 && lzs > 0)  {
            lxs ++;
            lzs --;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 16:
          if (lxs > 0 && lzs < 4)  {
            lxs --;
            lzs ++;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 17:
          if (lys < 4 && lzs > 0)  {
            lys ++;
            lzs --;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
        //  
        case 18:
          if (lys > 0 && lzs < 4)  {
            lys --;
            lzs ++;
            ablco ++;
          }
           else { 
            lrvlc ++;                    //Random value - loop counter
          }
        break;
      }
    } while (ablco == 1);
      
    if (lrrd == 1) { 
      if (lrrdi == 0)  {       // Random richtungs dauer input 0 bis 4
        lrrd = random8(3,5);           // random richtungs dauer 
      }  
      else  {
        lrrd = lrrdi; 
      }
    }
      lrv2 = lxs * 100 + lys * 10 + lzs;   //mach aus den 3 x,y,z Koordinaten eine Zahl, die eine LED im Würfel 
                                      // representiert                                      
      for (byte i = 0; i < 125; i++) { // läuft durch den ersten Teil des array wo die Koordinaten stehen
        varw1 = atl1[i];                            // Wordvariable für alles mögliche
        if (varw1 == lrv2) {             // vergleich, wenn gleich dann den pointer i übernehmen
          var1 = i;
          i = 125;      // Abruch wenn gleich
        }
      }
      ALed = atl1[var1 + 125]; 
      //                        Checken ob die Led schon an ist, damit die Schlange sich nicht selbst beisst
      ablco = 3;
      for (byte i = 0; i < scal; i++) {        // scal = Schlange länge  
       var1 = ael[i]; 
       if (var1 == ALed)  {
        ablco = 2;
        lxs = lxsa; lys = lysa; lzs = lzsa;     // alte x,y,z werte zurücksichern
        lrrd = 1;
        scbe ++;          // Schlange beisst sich counter
       }
      }
      if (scbe > 12)  {
        ablco = 4;
      }           
   } while (ablco == 2);  // nochmal von vorne, Schlange beisst sich
    //
   if (ablco == 3)  {     // start Schlange 
    scbe = 0;          // Schlange beisst sich counter
    ALedk = ael[scap];  //letzten Led wert holen, war der Kopf, wird dann auf Körperfarbe gesetzt
    scap ++;              // Pointer auf ältesten Led Wert setzen
    if (scap == scal ) {      // umlaufpointer max. wert =  scal (Schlange länge) 
      scap = 0;           // 
    }
    ALedo =  ael[scap];     // aray muss mindestens so groß sein wie Schlange länge - ältesten wert nach ALedo
    ael[scap] = ALed;       // neuen Wert abspeichern, da war vorher der älteste wert
    //
    leds[ALedk] = CHSV(hue3, sat3, valu3);      //einschalten / für Led an 
    leds[ALed] = CHSV(hue2, sat2, valu2);      //anders aus /
    leds[ALedo] = CHSV(hue1, sat1, valu1);      //aus - default schalten /
    lrrd --;                 // random richtungs dauer      
    FastLED.show(); 
   }                                                  // ist wie der rest 
   if (anico == 100) {     // Schlange animations counter - zählt die anaimationsdurchläufe 
      neufa();
    anico = 0;     
   }
  if (ablco == 4) {     // Schlange hat sich gebissen - Funkeln und Neustart
    anima1 = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine  
    funkf = 2;          // Funkelfarbe rot
    funkz = 70;          //Funkel Zeit, wird runtergezählt, bei 0 kein funkeln mehr
    funkl = 15;         // Startwert Ledanzahl die funkeln 
    scbe = 0;          // Schlange beisst sich counter 
    ablco = 0;         // Ablaufcounter Animationen  
  }  
} 

/////////////////////////// Ende Animationsprogramme ////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////

void inp3() {           // Serieller Input bei 3 char Eingabe

  if (wert1 == 'p') {     // Poti / Tastatursteuerung umschaltung
    //-------------------  ---------------------------------
    if (wert2 == 'o')  {     // 
      if (wert3 == '0') {    // wenn 0 dann Tastatursteuerung
        //fl_pote = 0;             // poti enable = 1, 0 ist Tastatursteuerung
        bitClear(flagr,3);      // poti enable = 1, 0 ist Tastatursteuerung
      }
      else if (wert3 == '1') {    // Potisteuerung
        //fl_pote = 1;       // poti enable = 1, 0 ist Tastatursteuerung
        bitSet(flagr,3);      //flag bit 3 = poti enable, 1 = ein, 0 ist Tastatursteuerung
      }
     }
  }
  else if (wert1 == 'F') {     // für Poti / Tastatureinstellung der Farben in animation
    bitClear(flagr,2);      //bit 2 = flag animation Farbwechsel an / aus = 0,
    
    if (wert2 == '0')  {     // 
      if (wert3 == '0') {    //
        pots = 0;                   // mit Poti / Tastatur einstellen aus
        Serial.println(F("Farbeinstellung & Farbwechsel aus"));                                                      
      }      
    }
    if (wert2 == '1')  {     //ascii 1         c1h, c1s, c1v    ist die eingabe
      if (wert3 == 'h') {    //ascii h
        pots = 1;                   // mit poti einstellen
        pota = hue1;              // auf aktuellen Wert einstellen 
      }
      else if (wert3 == 's') {    //ascii s
        pots = 2;                   // mit poti einstellen
        pota = sat1;              // auf aktuellen Wert einstellen  
      } 
      else if (wert3 == 'v') {    //ascii v
        pots = 3;                   //mit poti einstellen 
        pota = valu1;              // auf aktuellen Wert einstellen 
      }                 
    }
    if (wert2 == '2')  {     //ascii 2         c2h, c2s, c2v    ist die eingabe
      if (wert3 == 'h') {    //ascii h
        pots = 4;                   // mit poti einstellen
        pota = hue2;              // auf aktuellen Wert einstellen  
      }
      else if (wert3 == 's') {    //ascii s
        pots = 5;                   // mit poti einstellen
        pota = sat2;              // auf aktuellen Wert einstellen 
      } 
      else if (wert3 == 'v') {    //ascii v
        pots = 6;                   //mit poti einstellen 
        pota = valu2;              // auf aktuellen Wert einstellen 
      }                 
    }
    if (wert2 == '3')  {     //ascii 3         c3h, c3s, cd3v    ist die eingabe
      if (wert3 == 'h') {    //ascii h
        pots = 7;                   // mit poti einstellen
        pota = hue3;              // auf aktuellen Wert einstellen 
      }
      else if (wert3 == 's') {    //ascii s
        pots = 8;                   // mit poti einstellen
        pota = sat3;              // auf aktuellen Wert einstellen   
      } 
      else if (wert3 == 'v') {    //ascii v
        pots = 9;                   //mit poti einstellen 
        pota = valu3;              // auf aktuellen Wert einstellen  
      }                 
    }
   prifa();    
  }// ende ascii C
    //------------------- Säulen ---------------------------------
  else if (wert1 == 's') {     //    für Säulen, animationen
    if (wert2 == 'l')  {     //
      ablco = 0; 
      ablco1 = 0;
      farb1();             //alle Leds auf default Farbwerde einschalten HUE1,SAT1,VALU1)
      if (wert3 == '1') {    // = säule anim - diagonal eine schrägsäule
        anima = 7;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        anza = 7;           // Anzahl Animationen pro Säulen Durchlauf, Schrägsäulen 8 animationen = eingabe 7,
        lepra = 5;     // anzahl der Leds pro animation, schrägsäulen = 5  
        arstp = 0;     // array startpunkt einer animation, schrägsäulen = 0,         
      }
      else if (wert3 == '2') {    // = säule anim - diagonal doppelschrägsäule 
        anima = 7;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        anza = 3;           // Anzahl Animationen pro Säulen Durchlauf, doppelSchrägsäulen 4 animationen = eingabe 3,
        lepra = 10;     // anzahl der Leds pro animation, doppelschrägsäulen = 10  
        arstp = 40;     // array startpunkt einer animation, doppelschrägsäulen = 40        
      }
      else if (wert3 == '3') {    //   = säule anim - diagonal doppelschrägsäule ausgefüllt 
        anima = 7;           // animations auswahl, welche anim gezeigt wird, 0 = keine  
        anza = 3;           // Anzahl Animationen pro Säulen Durchlauf, doppelSchrägsäulen 4 animationen = eingabe 3,
        lepra = 17;     // anzahl der Leds pro animation, doppelschrägsäulen ausgefüllt = 17 
        arstp = 80;     // array startpunkt einer animation, doppelschrägsäulen ausgefüllt = 80           
      }
      else if (wert3 == '4') {    //    = säule anim umlaufend dann nach innen
        anima = 8;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        alsm = 7;        //max. anzahl leuchtender Säulen 
        anza = 25;           // Anzahl Animationen pro Säulen Durchlauf
        arstp = 0;        // array startpunkt einer animation,             
      }
      else if (wert3 == '5') {    //    = säulen anim umlaufend 
        anima = 8;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        alsm = 7;        //max. anzahl leuchtender Säulen 
        anza = 15;           // Anzahl Animationen pro Säulen Durchlauf
        arstp = 26;        // array startpunkt einer animation,             
      }
      else if (wert3 == '6') {    //    = säulen anim zick-zack, 
        anima = 8;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        alsm = 7;        //max. anzahl leuchtender Säulen 
        anza = 24;           // Anzahl Animationen pro Säulen Durchlauf
        arstp = 42;        // array startpunkt einer animation,             
      }
      else if (wert3 == '7') {    //    = säulen anim umlaufend innen
        anima = 8;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        alsm = 4;        //max. anzahl leuchtender Säulen 
        anza = 7;           // Anzahl Animationen pro Säulen Durchlauf
        arstp = 67;        // array startpunkt einer animation,             
      }
      else if (wert3 == '8') {    //    = säulen anim diagonal
        anima = 8;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        alsm = 7;        //max. anzahl leuchtender Säulen 
        anza = 15;           // Anzahl Animationen pro Säulen Durchlauf
        arstp = 75;        // array startpunkt einer animation,             
      }
      else if (wert3 == '9') {       // Wendel Uhrzeigersinn
       anima = 9;           // animations auswahl, welche anim gezeigt wird, 0 = keine    
        anico = 0;
        arstp = 0;
      }   
    } // ende l für Säulen 
    //
    else if (wert2 == 'p')  {     // Sprudel animation 
      farb1();             //alle Leds auf default Farbwerde einschalten HUE1,SAT1,VALU1)
      ablco = 0; 
      if (wert3 == '1') {    //    = Sprudel langsam wechelnde Farben
       anima = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
       bitClear(flagr,14);      //bit 14 koordinaten ok = 1 für Random Farben          
      }
     else if (wert3 == '2') {    //     = Sprudel Zufalls Farben
       anima = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine   
       bitSet(flagr,14);      // bit 14 koordinaten ok = 1 für Random Farben   
     }
    } // ende p für Sprudel
  //-------------------- schlange ----------------------------------
    else if (wert2 == 'c')  {     // Schlangen animation   
      ablco = 0; 
      scal = 8;              // Schlange länge                
      if (wert3 == '1') {    //    = Schlange rechtwinklig                 
        anima = 5;           // animations auswahl, welche anim gezeigt wird, 0 = keine           
        rws = 1;              // Startwert für Random generator
        rwe = 7;              // Endwert für Random generator = Schlange rechtwinklig
      }
      else if (wert3 == '2') {    //    =  Schlange 45 grad               
        anima = 5;           // animations auswahl, welche anim gezeigt wird, 0 = keine            
        rws = 7;              // Startwert für Random generator
        rwe = 19;              // Endwert für Random generator = Schlange 45 grad
      }
      else if (wert3 == '3') {    //     = Schlange alle richtungen               
        anima = 5;           // animations auswahl, welche anim gezeigt wird, 0 = keine            
        rws = 1;              // Startwert für Random generator
        rwe = 19;              // Endwert für Random generator = Schlange rechtwinklig & 45 grad
      }
    } // ende c für schlange
    
  } // ende säulen, schlange, sprudel

    //------------------- funkeln ---------------------------------
  else if (wert1 == 'f') {     //    für funkeln & figuren
    if (wert2 == 'u')  {     //
      if (wert3 == '0') {    //   = funkeln anim    - aus -
        anima1 = 0;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        farb1();                // Farbwert 1 für alle Leds          
      }               
      else if (wert3 == '1') {    //  = funkeln anim läuft dauernd
        anima1 = 1;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        funkl = 15;          
      }
      else if (wert3 == 'w') {    // = funkeln weiss
        anima1 = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine  
        funkf = 0;          // Funkelfarbe weiss
        funkz = 70;          //war 105 Funkel Zeit, wird runtergezählt, bei 0 kein funkeln mehr
        funkl = 15;         // Startwert Ledanzahl die funkeln         
      }
      else if (wert3 == 'r') {    //= funkeln rot
        anima1 = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        funkf = 2;          // Funkelfarbe Rot
        funkz = 70;          //Funkel Zeit, wird runtergezählt, bei 0 kein funkeln mehr
        funkl = 15;           
      }              
      else if (wert3 == 'y') {    // = funkeln gelb
        anima1 = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        funkf = 1;          // Funkelfarbe gelb
        funkz = 70;          //Funkel Zeit, wird runtergezählt, bei 0 kein funkeln mehr
        funkl = 15;           
      }
      else if (wert3 == 'g') {    // = funkeln grün
        anima1 = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        funkf = 3;          // Funkelfarbe gelb
        funkz = 70;          //Funkel Zeit, wird runtergezählt, bei 0 kein funkeln mehr
        funkl = 15;           
      }
      else if (wert3 == 'b') {    //= funkeln blau
        anima1 = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        funkf = 4;          // Funkelfarbe gelb
        funkz = 70;          //Funkel Zeit, wird runtergezählt, bei 0 kein funkeln mehr
        funkl = 15;          // Startwert Ledanzahl die funkeln           
      }
      else if (wert3 == 'B') {    //= funkeln bunt
        anima1 = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        funkf = 5;          // Funkelfarbe bunt
        funkz = 70;          //Funkel Zeit, wird runtergezählt, bei 0 kein funkeln mehr
        funkl = 15;          // Startwert Ledanzahl die funkeln           
      }
     }// ende u
    ///////////////////////////////////// Figuren ///////////////////////////////////////
      else if (wert2 == 'i')  {     //
        if (wert3 == '1') {    // schaltet Würfel Umriss ein
          lepra = 44;
          for (byte i = 0; i < lepra; i ++)  {   
            ALed = umr[i]; // 
            leds[ALed] = CHSV(hue2, sat2, valu2);
          }                   
        }                        
        else if (wert3 == '2') {            //Würfel Umriss ein mit Farbanimation
         anima = 10;           // animations auswahl, welche anim gezeigt wird, 0 = keine    
          anico = 25;
        }
        else if (wert3 == '3')  {  // Würfelumriss und kleines Drehkreuz in Mitte mit Farbanimation
         anima = 11;           // animations auswahl, welche anim gezeigt wird, 0 = keine    
          anico = 25;                      
        }
        FastLED.show();  
      }// ende i          
    }// ende f für funkeln / figuren
    Serial.write(wert1);Serial.write(wert2);Serial.write(wert3);
    Serial.println();
}// ende void inp3
