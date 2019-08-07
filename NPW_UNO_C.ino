// Filename als String
String filn = "NPW_UNO_C.ino"; 
/*  
 *  ###########################################
 *  Neopixelwürfel für Arduino UNO,  NPW_UNO_A.ino = Figuren, Funkeln, Säulen, Sprudel
 *                                   NPW_UNO_B.ino = Schlange, Funkeln, Sprudel
 *                                   NPW_UNO_C.ino = Würfel
 *  
 *  Das Programm startet im Demo Modus und zeigt vorhandene Figuren und Animationen
 *  
 *  LED_OUT 6           // Datenausgang zum Neopixelwürfel
 *  GPIO 13           //interne LED blinkt alle 2 sek. wenn PRGM. läuft
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
 * Farbsätze mit Tastatur / Poti einstellbar, wenn selektiert, wird 
 *   auto. Farbwechsel ausgeschaltet, mit  n  wieder einschaltbar
 *  zuerst eine Animation starten z.B. Schlange oder 
 *  Würfelumriss und kleines Drehkreuz in Mitte mit Farbanimation (fi8)  
 *  dann:
 *  F1h, F1s, F1v   Hintergrund (Led aus) Farbe h= hue, s= saturation, v= value
 *  F2h, F2s, F2v   sekundäre Vordergrund Farbe (Led ein)                                          
 *  F3h, F3s, F3v   primäre Vordergrund Farbe (Led ein) 
 *  mit +, -, P, M ändern
 *  F00 = Farbsätze einstellen - aus
 * 
  * Figuren
 * fi0 = Umriss des Neopixel Würfels
 * fi7 = Würfel Umriss ein mit Farbanimation
 * 
 * --- Animationen ---
 * t = Testprogramm - Funktionskontrolle für alle Leds, blinkt weiss, heller, dunkler
 * 
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
 * Sprudel
 * sp0 = sprudel langsame Farbänderung
 * sp1 = sprudel bunt
 * 
 * Würfel
 * wu0 = Würfel umlaufend vertikal
 * wu1 = Würfel umlaufend vertikal & horizontal  
 * wu2 = Würfel schräg vertikal & oben entlang
 * wu3 = Würfel zufallswerte rechtwinklig
 * wu4 = Würfel zufallswerte 45grad
 * wu5 = Würfel zufallswerte 45grad & rechtwinklig
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
byte gesa = 0;       // für Geschwindigkeit Animation mit Poti / Tastatur einstellbar
word anico = 0;     //  animations counter - zählt die animationsdurchläufe
byte anig = 60;          // animations geschwindigkeit
word pot = 0;           //poti am analogeingang A0
byte pota = 60;          // alter wert von pot auf 0-255 gemapped
byte pots = 0;          // poti / Tastaturselekt, was gesteuert werden soll
byte arstp = 0;     // array startpunkt einer animation
byte ablco = 0;         // Ablaufcounter Animationen
byte ablco1 = 0;         // Ablaufcounter Animationen Würfel
byte anza = 4;      // Anzahl Animationen bis der Startpunkt wieder erreicht ist - hier 4 
byte lepra = 0;     // anzahl der Leds pro animation
byte aelp = 0;      //pointer für ael[] - Deuted auf die Speicherstelle, Wo die aktuellen Leds 
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

// Würfelarray - jeweils 8 Leds bilden einen Würfel
byte wuerf[] = {0,5,45,40,1,6,46,41, 5,10,40,35,6,11,41,36, 10,15,35,30,11,16,36,31,       // würfel 0,1,2  - 24 Leds ebene 0&1
                15,20,30,25,16,21,31,26, 45,40,50,55,46,41,51,56, 40,35,55,60,41,36,56,61, // würfel 3,4,5  - 24 Leds  
                35,30,60,65,36,31,61,66, 30,25,65,70,31,26,66,71, 50,55,95,90,51,56,96,91, // würfel 6,7,8  - 24 Leds 
                55,60,90,85,56,61,91,86, 60,65,85,80,61,66,86,81, 65,70,80,75,66,71,81,76, // würfel 9,10,11  - 24 Leds 
                95,90,100,105,96,91,101,106, 90,85,105,110,91,86,106,111,                  // würfel 12,13  - 16 Leds 
                85,80,110,115,86,81,111,116, 80,75,115,120,81,76,116,121,                  // würfel 14,15  - 16 Leds
                                                                                           //       gesamt = 104 Leds
                
                1,6,46,41,2,7,47,42, 6,11,41,36,7,12,42,37, 11,16,36,31,12,17,37,32,       // würfel 16,17,18  ebene 1&2
                16,21,31,26,17,22,32,27, 46,41,51,56,47,42,52,57, 41,36,56,61,42,37,57,62, // würfel 19,20,21
                36,31,61,66,37,32,62,67, 31,26,66,71,32,27,67,72, 51,56,96,91,52,57,97,92, // würfel 22,23,24
                56,61,91,86,57,62,92,87, 61,66,86,81,62,67,87,82, 66,71,81,76,67,72,82,77, // würfel 25,26,27
                96,91,101,106,97,92,102,107, 91,86,106,111,92,87,107,112,                  // würfel 28,29
                86,81,111,116,87,82,112,117, 81,76,116,121,82,77,117,122,                  // würfel 30,31
                                                                                           //       gesamt = 104 Leds

                2,7,47,42,3,8,48,43, 7,12,42,37,8,13,43,38, 12,17,37,32,13,18,38,33,       // würfel 32,33,34  ebene 2&3
                17,22,32,27,18,23,33,28, 47,42,52,57,48,43,53,58, 42,37,57,62,43,38,58,63, // würfel 35,36,37
                37,32,62,67,38,33,63,68, 32,27,67,72,33,28,68,73, 52,57,97,92,53,58,98,93, // würfel 38,39,40
                57,62,92,87,58,63,93,88, 62,67,87,82,63,68,88,83, 67,72,82,77,68,73,83,78, // würfel 41,42,43
                97,92,102,107,98,93,103,108, 92,87,107,112,93,88,108,113,                  // würfel 44,45
                87,82,112,117,88,83,113,118, 82,77,117,122,83,78,118,123,                  // würfel 46,47

                3,8,48,43,4,9,49,44, 8,13,43,38,9,14,44,39, 13,18,38,33,14,19,39,34,       // würfel 48,49,50  ebene 3&4
                18,23,33,28,19,24,34,29, 48,43,53,58,49,44,54,59, 43,38,58,63,44,39,59,64, // würfel 51,52,53
                38,33,63,68,39,34,64,69, 33,28,68,73,34,29,69,74, 53,58,98,93,54,59,99,94, // würfel 54,55,56
                58,63,93,88,59,64,94,89, 63,68,88,83,64,69,89,84, 68,73,83,78,69,74,84,79, // würfel 57,58,59
                98,93,103,108,99,94,104,109, 93,88,108,113,94,89,109,114,                  // würfel 60,61
                88,83,113,118,89,84,114,119, 83,78,118,123,84,79,119,124};                  // würfel 62,63
//
// Translation Array, Koordinaten in Würfelpositionen 0 - 63
word rwtl[] = {0,100,200,300,10,110,210,310,20,120,220,320,30,130,230,330,        //Würfel 0 - 15 ebene 0&1
               1,101,201,301,11,111,211,311,21,121,221,321,31,131,231,331,       //Würfel 16 - 31   ebene 1&2
               2,102,202,302,12,112,212,312,22,122,222,322,32,132,232,332,       //Würfel 32 - 47   ebene 2&3
               3,103,203,303,13,113,213,313,23,123,223,323,33,133,233,333};      //Würfel 48 - 63   ebene 3&4
              
//Umriss
byte umr[] = {0,1,2,3,4,9,14,19,24,23,22,21,20,15,10,5,45,25,70,50,95,75,49,29,54,74,99,79,
                100,101,102,103,104,109,114,119,124,123,122,121,120,115,110,105};  //Umriss des Würfels
                                                         //letzter Wert in 43 von 0 aus gezählt, lepra = 43
 
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
  Serial.println("DEMO Mode, aus mit d");
                  
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
        digitalWrite(CLED, LOW); // extern angeschlossene Led ausschalten
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
              Serial.println("Alle Leds aus");
               FastLED.clear();
               FastLED.show();
              break;                         
             //
             case '3':          //               
              if (funkl < 20) {
                funkl +=2;              // Anzahl der Leds die funkeln 
              }
              Serial.print("Funkelleds + "); Serial.println(funkl);              
              break;
             //
             case '4':          //              
              if (funkl > 2) {
                funkl -= 2;            // Anzahl der Leds die funkeln
              }
              Serial.print("Funkelleds - "); Serial.println(funkl);               
              break;
              //
             case '5':          //              
              anig = 60;
              pota = anig; 
              Serial.print("Anim.Tempo def. "); Serial.println(anig);               
              break;              //
              //
             case 'a':          //animation aus
              Serial.println("Anim. aus");
              anima = 0;                   // animation aus
              pots = 0;                   // Poti aus
              farb1();                // Farbwert 1 für alle Leds
              break;
              //
             case 'b':        //toggle bunt an/aus 
              Serial.print("bunt ");           
              if (bitRead(flagr,5) == 1)  {       //bit 5 = flag bunt an / aus = 0,
                bitClear(flagr,5);      //
                Serial.println("aus");
              }
              else if (bitRead(flagr,5) == 0)  {
                bitSet(flagr,5);      //
                Serial.println("ein");
              }               
              break;             
              //
             case 'H':          //Helligkeit increment               
              if (valu3 < 238)  {
                valu3 += 16;            
                valu2 += 16;       //                
              } 
              Serial.print("Hell+ ");Serial.println(valu3);             
             break; 
             //            
             case 'h':          //brightness decrement 
             if (valu3 > 15)  {
              valu3 -= 16;            
               valu2 -= 16;       //               
              } 
              Serial.print("Hell- ");Serial.println(valu3);              
             break;             
              //
             case 'D':          //  
              Serial.println("Demo ein"); 
              anima = 0;
              anima1 = 0;
              bitSet(flagr,1);      //flag bit 1 = demo mode, 1 = ein 
              anico = 0;          //animcounter
              farb1();                // Farbwert 1 für alle Leds
              break;
             //
             case 'd':          // 
              Serial.println("Demo aus"); 
              bitClear(flagr,1);      //flag bit 1 = demo mode, 0 = aus          //
              anima = 0;
              anima1 = 0;
              farb1();                // Farbwert 1 für alle Leds
              break;
              //
             case 'F':          // setzt default Farbwerte
              bitClear(flagr,5);        //bunt aus
              setfa();         //set alle Leds auf default Farbwerte                //
              Serial.println("Def. Farben");
              break;
              //
             case 'G':                //  Animationsgeschwindigkeit mit Poti oder Tastatur einstellen
                                      //- kleinere Werte = höhere Geschwindigkeit
             Serial.println("Ani.Tempo adj.");    // Geschwindigkeit mit Poti / Tastatur
              pots = 20; 
              pota = anig;          
              break;
             //
             case 'g':          // Poti oder Tastatur einstellen aus
              Serial.println("Ani.Tempo aus"); 
              pots = 0;
              break;
              //              
             case 'i':
              Serial.println();
              Serial.print("Info: ");Serial.println(filn); 
              Serial.print("flagr ");Serial.println(flagr, BIN);                                         
              Serial.print(" Anim. Nr. "); Serial.print(anima); // welche Animation läuft                           
              Serial.print("\tGeschw. ");Serial.println(anig); // Animationsgeschwindigkeit
              prifa();    
              break;
              //
             case 'n':        //toggle langsamer Farbwechsel an/aus 
              Serial.print("Farbwechsel ");           
              if (bitRead(flagr,2) == 1)  {       //bit 2 = flag animation Farbwechsel an / aus = 0,
                bitClear(flagr,2);      //
                Serial.println("aus");
              }
              else if (bitRead(flagr,2) == 0)  {
                bitSet(flagr,2);      //
                Serial.println("ein");
              }               
              break;
             //             
             // 
             case 'S':          //  Animation Halt
             Serial.println("Halt Anim."); 
              bitClear(flagr,0);      //bit 0, animation enable = 1, für halt und step mode
              break;
             //
             case 's':          
             Serial.println("Step Anim."); 
              anista();       // ruft die jeweiligen Animationen einmal auf
              break;
             //
             case 'w':          // Animation weiter
              Serial.println("Anim. weiter"); 
              bitSet(flagr,0);      //bit 0, animation enable = 1, für halt und step mode
              break;
             //
             case 't':          // test
              Serial.println("Test");
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
               Serial.print("Serial in: ");Serial.write(wert1);
               Serial.write(wert2);Serial.write(wert3); Serial.println();
  
            inp3();          // Input bei 3 char  
                                     
            } // ende if (ttyb == 3)
          //          
            else {     //es wurden mit send mehr als 3 ASCII Zeichen übertragen
              Serial.println("Fehler >3 char. input");    
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
        digitalWrite(CLED, HIGH); // extern angeschlossene Led an einschalten
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
    case 1:
     figu1();           // Figur Würfel Umriss Farbanimation
     break;
    case 2:          //
     anispru();            //Sprudel / Blasen  Animation
     break;
    case 3:          //Würfel mit Zufallswerten
     aniwu2();
     break; 
    case 4:
     test();        // Testprgm.
     break;
    case 10:
     figu1();           // Figur Würfel Umriss Farbanimation
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
void prifa()  {             // Serial print farbe H = Hue, S = Saturation, V = Value
  Serial.print("H1 "); Serial.print(hue1); Serial.print(" H2 "); Serial.print(hue2); 
  Serial.print(" H3 "); Serial.println(hue3); 
  Serial.print("S1 "); Serial.print(sat1); Serial.print(" S2 "); Serial.print(sat2); 
  Serial.print(" S3. "); Serial.println(sat3); 
  Serial.print("V1 "); Serial.print(valu1); Serial.print(" V2 "); Serial.print(valu2); 
  Serial.print(" V3 "); Serial.println(valu3);
}

////////////////////////////////////////////////////////////////////////////////////////
void poti() {
// pot = analogRead(potpin);    // reads the potentiometer (value between 0 and 1023)
// pot = map(pot, 0, 1023, 0, 255);     // scale it value between 0 and 255
 
//  if (pot != pota)  {
//    pota = pot;
    Serial.print("P/T Wert "); Serial.println(pota);
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
    demco = random8(0,12);           // random demo 
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
    Serial.print("DEMO ");Serial.print(demco);Serial.print("\t");
  }
  switch (demco) {
     //
     case 0:
      sekco = 3;          // Sekundencounter          
      anima = 0;                   // animation aus
      farb1();                // Farbwert 1 für alle Leds          
     break;
     //     
     case 1: 
        sekco = 12;          // Sekundencounter        
        wert1 = 'f';  wert2 = 'i';  wert3 = '7';   //fi7 Figurumriss ganzer Würfel 
     break;
      //
     case 2:
        sekco = 10;          // Sekundencounter geändert
        wert1 = 's';  wert2 = 'p';  wert3 = '1'; //Sprudel bunt
      break;
      //    
     case 3:
        anima = 0;           // animationen aus
        farb1();                // Farbwert 1 für alle Leds
        sekco = 4;          // Sekundencounter geändert
        wert1 = 'f';  wert2 = 'u';  wert3 = 'w'; //fuw Funkeln weiss kurz,
      break;
      //
     case 4: 
        wert1 = 's';  wert2 = 'p';  wert3 = '0'; //Sprudel 
      break;
      //       
     case 5:
        anima = 0;
        sekco = 4;          // Sekundencounter geändert
        wert1 = 'f';  wert2 = 'u';  wert3 = 'g'; //Funkeln grün kurz,
      break;
      //
     case 6:
        sekco = 4;          // Sekundencounter geändert
        wert1 = 'f';  wert2 = 'u';  wert3 = 'b'; //Funkeln blau kurz,
      break;
      //
     case 7:
        sekco = 4;          // Sekundencounter geändert
        wert1 = 'f';  wert2 = 'u';  wert3 = 'y'; //Funkeln gelb kurz,
      break;
      //
     case 8:
        sekco = 12;          // Sekundencounter
        wert1 = 'w';  wert2 = 'u';  wert3 = '3'; //Zufallswürfel rechtwinklig
      break;
      //
     case 9:
        sekco = 12;          // Sekundencounter
        wert1 = 'w';  wert2 = 'u';  wert3 = '4'; //Zufallswürfel 45 grad
      break;
      //
     case 10:               // standart Farben oder bunte Leds
     sekco = 4;          // Sekundencounter     
      Serial.print("bunt ");           
      if (bitRead(flagr,5) == 1)  {       //bit 5 = flag bunt an / aus = 0,
        bitClear(flagr,5);      //
        Serial.print("aus  ");
      }
      else if (bitRead(flagr,5) == 0)  {
        bitSet(flagr,5);      //
        Serial.print("ein  ");
      } 
       wert1 = 'f';  wert2 = 'u';  wert3 = 'B'; //Funkeln bunt kurz,
     break;
      //
     case 11:
        sekco = 12;          // Sekundencounter
        wert1 = 'w';  wert2 = 'u';  wert3 = '5'; //Zufallswürfel rechtwinklig & 45 grad
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
void aniwu2() {           // Animationen miniWürfel2 per Zufallszahlen 
                          //es gibt 0 - 63 mögliche miniWürfelpositionen
  // lrv1 = variable für neue Richtung des miniWürfels
  // lxw, lyw, lzw = local static variable für miniWürfel koordinaten
  // lrv2 = variable für zusammengefasste x,y,z miniWürfel koordinaten
                          
//lokale variable
  byte v1;    // variable 1
  byte v2;    // variable 2 
  byte a1;    //arraypointer 1
  byte a2;    //arraypointer 2
  static byte lrv1;    // local random value 1
  static word lrv2;    // local random value 2
  static byte lrrd;    // local random richtungs dauer
  byte z1;    //zähler 1
  static byte lxw;     // lokal x würfel
  static byte lyw;     // lokal y würfel
  static byte lzw;     // lokal z würfel
  static byte lxwa;     // lokal xw würfel alter wert
  static byte lywa;     // lokal yw würfel alter wert 
  static byte lzwa;     // lokal zw würfel alter wert
  static word varw1;     // Wordvariable für alles mögliche 
   
  if (ablco == 0) {       // INIT 
    lrrd = 0;                // Random Richtung dauer
    for (byte a1 = 0; a1 < 4; a1++) {   //  array alle werte auf 255
      ael[a1] = 255;
    }
  ablco = 1;
  v1 = 0;
  z1 = 0;

  lxwa = 0; lywa = 0; lzwa = 0;
  
  }
  //
  else if  (ablco == 1) {       //   
   do {                     // große schleife check ob wert schon im array

    lxw =lxwa; lyw = lywa; lzw = lzwa; 
    do {                    // kleine schleife check ob innerhalb Würfel
      v1 = 0;               // wird hier als flag verwendet
      if (lrrd == 0) {
        lrv1 = random8(rws,rwe);           // random value1 = random number für Würfelposition 0 bis 18 
        lrrd = random8(1,3);           // random richtungs dauer    
      }
      else {
        lrrd --;                   
      }                                   
          switch (lrv1)  {         // im switch / case check, das Würfel im Würfel bleibt
            case 1:
              if (lxw < 3)  {
                lxw ++;
                v1 = 100;
              }
            break;
            case 2:
              if (lxw > 0)  {
                lxw --;
                v1 = 100;
              }
            break;
            case 3:
              if (lyw < 3)  {
                lyw ++;
                v1 = 100;
              }
            break; 
            case 4:
              if (lyw > 0)  {
                lyw --;
                v1 = 100;
              }
            break; 
            case 5:
              if (lzw < 3)  {
                lzw ++;
                v1 = 100;
              }
            break;
            case 6:
              if (lzw > 0)  {
                lzw --;
                v1 = 100;
              }
            break;
            case 7:
              if (lxw < 3 && lyw < 3)  {           
                lxw ++;
                lyw ++;
                v1 = 100;
              }
            break;
            //
            case 8:
              if (lxw > 0 && lyw > 0)  {
                lxw --;
                lyw --;
                v1 = 100;
              }
            break;
            case 9:
              if (lxw < 3 && lyw > 0)  {
                lxw ++;
                lyw --;
                v1 = 100;
              }
            break; 
            case 10:
              if (lxw > 0 && lyw < 3)  {
                lxw --;
                lyw ++;
                v1 = 100;
              }
            break;
            case 11:
              if (lxw < 3 && lzw < 3)  {
                lxw ++;
                lzw ++;
                v1 = 100;
              }
            break; 
            case 12:
              if (lxw > 0 && lzw > 0)  {
                lxw --;
                lzw --;
                v1 = 100;
              }
            break;  
            case 13:
              if (lyw < 3 && lzw < 3)  {
                lyw ++;
                lzw ++;
                v1 = 100;
              }
            break; 
            case 14:
              if (lyw > 0 && lzw > 0)  {
                lyw --;
                lzw --;
                v1 = 100;
              }
            break;  
            case 15:
              if (lxw < 3 && lzw > 0)  {
                lxw ++;
                lzw --;
                v1 = 100;
              }
            break; 
            case 16:
              if (lxw > 0 && lzw < 3)  {
                lxw --;
                lzw ++;
                v1 = 100;
              }
            break;
            case 17:
              if (lyw < 3 && lzw > 0)  {
                lyw ++;
                lzw --;
                v1 = 100;
              }
            break;  
            case 18:
              if (lyw > 0 && lzw < 3)  {
                lyw --;
                lzw ++;
                v1 = 100;
              }
            break;
          }// ende switch 
      if (v1 != 100)  {         // <100 ist ausserhalb Würfel
        lrrd = 0;
       z1 ++;
       if (z1 > 20) {
        ablco = 2;     // looped, prgm abbrechen 
        goto ausbruch;
       }
       
      }
      //
      } while (v1 != 100);  //schleifenabruch wenn v1 = 100 = neuer wert ok
  //
        z1 = 0;         //schleifenzähler auf 0

      lrv2 = lxw * 100 + lyw * 10 + lzw;   //mach aus den 3 x,y,z Koordinaten eine Zahl, die eine LED im Würfel 
                                      // representiert       // lrv2 = Random value 2
  
      for (a1 = 0; a1 < 64; a1++) { // läuft durch das array wo die Koordinaten stehen
        varw1 = rwtl[a1];                            // Wordvariable für alles mögliche
        if (varw1 == lrv2) {             // vergleich, wenn gleich dann den pointer a1 übernehmen
          ase = a1;      // hier steht dann die Würfelnummer, ase = Säulenleds ein oder neue Farbe
          break;
        }
      }
      // check auf zufallszahlenwiederholung
      v2 = 0;
        for( a1 = 0; a1 < 4; a1++) {
          v1 = ael[a1];              //   array 
          if (v1 == ase)  {
            v2 = 100;        
          }
        }      
     } while (v2 == 100);  //schleifenabruch wenn lrv1 != 100  neuer wert ist gültig
     //
    lxwa = lxw; lywa = lyw; lzwa = lzw;     //  x,y,z werte sichern
    // neue miniWürfelnummer in ael() eintragen
    // fifo schiebt inhalt vom array um eins nach oben
    a1 = 4;
    do  {
      a2 = a1;
      a1 --;
      v1 = ael[a1];              //  - demo array hole arrayeintrag mit arraypointer 
      ael[a2] = v1;             //speicher v1 im array mit arraypointer2
      
    } while (a1 > 0);
    // ende fifo
    ael[a1] = ase;          // neue miniWürfelnummer im array abspeichern

    // alten miniWürfel aus, neuen miniWürfel einschalten 
    for (a1 = 0; a1 < lepra; a1 ++)  {   // schaltet vorigen Würfel aus oder default
      ALed = wuerf[asa * lepra + a1];     // lepra = anzahl der Leds (8) pro animation 
      leds[ALed] = CHSV(hue1, sat1, valu1);
    }   
      asa = ase;                    // asa = Säulenleds aus oder def. Farbe, ase = Säulenleds ein oder neue Farbe 
                                   // wird hier für Würfel verwendet  
    if (bitRead(flagr,5) == 0)  {       //bit 5 = flag, bunt = 1
      for (byte i = 0; i < lepra; i ++)  {   // schaltet neuen Würfel ein
        ALed = wuerf[ase * lepra + i]; // 
        leds[ALed] = CHSV(hue3, sat3, valu3);
      }
    }
    else  {
    for (byte i = 0; i < lepra; i ++)  {   // schaltet neuen bunten Würfel ein
      ALed = wuerf[ase * lepra + i]; // 
      leds[ALed] = CHSV((random8(10,255)), sat3, valu3);
      }
    } 
    FastLED.show();
    
  }// ende ablco == 1
  
  else if  (ablco == 2) {       // fehler - abbrechen
   ablco = 0;
  }
  
  if (anico == 60) {  // animations counter - zählt die anaimationsdurchläufe 
    neufa();
    anico = 0;
  }
  ausbruch:           //goto befehl 
  anico += 1; anico -=1; // dummy 
   
}// ende void aniwu2() {           // Animationen miniWürfel2 per Zufallszahlen 

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
    //fl_neuf = 0;                // auto. Farbwechsel aus
    bitClear(flagr,2);      //bit 2 = flag animation Farbwechsel an / aus = 0,
    
    if (wert2 == '0')  {     // 
      if (wert3 == '0') {    //
        pots = 0;                   // mit Poti / Tastatur einstellen aus
        Serial.println("Farbeinstellung & Farbwechsel aus");                                                      
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
  else if (wert1 == 's') {     //    für sprudel animationen
    //
    if (wert2 == 'p')  {     // Sprudel animation 
      //farb1();             //alle Leds auf default Farbwerde einschalten HUE1,SAT1,VALU1)
      ablco = 0; 
      if (wert3 == '0') {    //    = Sprudel langsam wechelnde Farben
       anima = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
       bitClear(flagr,14);      //bit 14 koordinaten ok = 1 für Random Farben          
      }
     else if (wert3 == '1') {    //     = Sprudel Zufalls Farben
       anima = 2;           // animations auswahl, welche anim gezeigt wird, 0 = keine   
       bitSet(flagr,14);      // bit 14 koordinaten ok = 1 für Random Farben   
     }
    } // ende p für Sprudel
    Serial.write(wert1);Serial.write(wert2);Serial.write(wert3);
    Serial.println();
  } // ende säulen, schlange, sprudel
    //------------------- Würfel  ---------------------------------
  else if (wert1 == 'w') {     // ascii w    
    if (wert2 == 'u')  {     //ascii u für Würfel animationen
      anima = 6;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
      ablco1 = 0;
      ablco = 0;  
      lepra = 8;     // anzahl der Leds pro animation, Würfel 8
      farb1();  //alle Leds auf default Farbwerde einschalten HUE1,SAT1,VALU1)
      //
      if (wert3 == '3') {    //animation Würfel Zufallswerte rechtwinklige Bewegung 
      anima = 3;           // animations auswahl, welche anim gezeigt wird, 0 = keine      
        rws = 1;              // Startwert für Random generator
        rwe = 7;              // Endwert für Random generator                   
      }
      else if (wert3 == '4') {    //animation Würfel Zufallswerte 45 grad Bewegung
      anima = 3;           // animations auswahl, welche anim gezeigt wird, 0 = keine      
        rws = 7;              // Startwert für Random generator
        rwe = 19;              // Endwert für Random generator                   
      }      
      else if (wert3 == '5') {    //animation Würfel Zufallswerte rechtwinklige & 45 grad Bewegung
      anima = 3;           // animations auswahl, welche anim gezeigt wird, 0 = keine      
        rws = 1;              // Startwert für Random generator
        rwe = 19;              // Endwert für Random generator                  
      }
    } // ende u für würfelanimationen
    Serial.write(wert1);Serial.write(wert2);Serial.write(wert3);
    Serial.println();
  } //ende w für würfelanimationen
    //------------------- funkeln ---------------------------------
  else if (wert1 == 'f') {     //    für funkeln & figuren
    if (wert2 == 'u')  {     //
      if (wert3 == '0') {    //   = funkeln anim    - aus -
        anima1 = 0;           // animations auswahl, welche anim gezeigt wird, 0 = keine 
        farb1();                // Farbwert 1 für alle Leds          
      }               
      else if (wert3 == '1') {    //  = funkeln anim
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
        if (wert3 == '0') {    //// schaltet Würfel Umriss ein
          lepra = 44;
          for (byte i = 0; i < lepra; i ++)  {   
            ALed = umr[i]; // 
            leds[ALed] = CHSV(hue2, sat2, valu2);
          }                   
        }                        
        else if (wert3 == '7') {            //Würfel Umriss ein mit Farbanimation
         anima = 10;           // animations auswahl, welche anim gezeigt wird, 0 = keine    
          anico = 25;
        }
        FastLED.show();  
      }// ende i      
    Serial.write(wert1);Serial.write(wert2);Serial.write(wert3);
    Serial.println();      
    }// ende f für funkeln / figuren

}// ende void inp3
