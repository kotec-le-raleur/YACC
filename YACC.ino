//  René Lefebvre 27 mars 2023
// programme testeur de continuité
// il faut installer le core suivant: http://drazzy.com/package_drazzy.com_index.json
//
// PINS    ATT85
//        ________U_______
//        | 1:RST  8: Vcc |
//        | 2:PB3  7: PB2 | 
//        | 3:PB4  6: PB1 | 
//        | 4:Gnd  5: PB0 | 
//        -----------------

    // PB5----pin 1 0 pin 8----Vcc
// A3  PB3----pin 2   pin 7----PB2 A1
// A2  PB4----pin 3   pin 6----PB1
    // Gnd----pin 4   pin 5----PB0
	
/*	
	           _________
             |   U   |
Reset (PB5)  | 1   8 |  VCC (+)
(A3)   PB3   | 2   7 |  PB2 (A1, SCK, SCL)
(A2)   PB4   | 3   6 |  PB1 (PWM, MISO)
(-)    GND   | 4   5 |  PB0 (PWM, AREF, MOSI, SDA)
             _________

Notes:
* An means "Analog Input n"
* PB means PORTB
* PB5 requires a "fuse" to be set in order to use it.


IN DEPTH:
Pin 1: PB5: PCINT5  ~RESET  ADC0 
Pin 2: PB3: PCINT3  XTAL1   CLKI  ~OC1B   ADC3
Pin 3: PB4: PCINT4  XTAL2   CLKO   OC1B   ADC2
Pin 4: GND
Pin 5: PB0: MOSI DI SDA AIN0 OC0A ~OC1A AREF PCINT0
Pin 6: PB1: MISO DO AIN1 OC0B OC1A PCINT1
Pin 7: PB2: SCK USCK SCL ADC1 T0 INT0 PCINT2
Pin 8: VCC
*/

#define OUT_PIN PB4     // pin 3  Buzzer
#define OUT_ALIM PB0     // pin 5  vers la base du 2N2222 pour  l'auto-alimentation
#define TEST_PIN PB3    // pin 2	vers les fils de test
int note[] = {262,277,294,311,330,349,370,392,415,440,466,494,523,554,587,622,659,740,784,830,880,932,988,2000};
unsigned long previousMillis;
const long interval = 60000;  // intervale avant arrêt automatique (milliseconds)

//*******************  MAIN   *********************
void setup() {
  pinMode(OUT_PIN, OUTPUT);
  pinMode(OUT_ALIM, OUTPUT);
  digitalWrite(OUT_ALIM, HIGH) ;  // auto entretien de l'alim par saturation du 2N2907
  previousMillis = 0;
  tone(OUT_PIN, 880,500 );
   
  
 }

//*******************  LOOP   *********************
void loop() {
int i  = 0;
int ix = 0, octa = 1;
long frq;
unsigned long  currentMillis ;

    currentMillis = millis();
    i =  analogRead(TEST_PIN) ;delay(2);
    i += analogRead(TEST_PIN) ;delay(2);
    i += analogRead(TEST_PIN) ;delay(2);
    i += analogRead(TEST_PIN) ;delay(2);
    i = i/4; 
 
    if (i < 501) // contact !
    {
    previousMillis = millis()-100;  // on ré-arme le timer
    frq = map(i,0,500,0,24);
    tone(OUT_PIN, note[frq],100 );
    delay(100);
    }
    if (currentMillis - previousMillis > interval)   // beep beep et on coupe l'alim !
     {
        tone(OUT_PIN, 247*octa,500 );
        delay(500);  
        tone(OUT_PIN, 165*octa,500 );
        delay(500);  
        tone(OUT_PIN, 185*octa,500 );
        delay(500);  
        tone(OUT_PIN, 208*octa,900 );
        delay(2000);  

        // tone(OUT_PIN, 208*octa,500 );
        // delay(500);  
        // tone(OUT_PIN, 185*octa,500 );
        // delay(500);  
        // tone(OUT_PIN, 165*octa,500 );
        // delay(500);  
        // tone(OUT_PIN, 247*octa,900 );
        // delay(1000);  

        digitalWrite(OUT_ALIM, LOW) ;  // coupure de l'alim par désaturation du 2N2907  
      }
    
}
  
/*
  Les quarts sont sonnés sur quatre cloches pesant environ 1.000 kg, 1.300 kg, 1.700 kg et 4.000 kg, 
  répondant aux notes la, sol, ta, do, tandis que celle des heures, d'environ 13.500 kg, 
  répond au fa; ces cloches servent exclusivement il la sonnerie de l'horloge. 
  Le déclenchement de la sonnerie des quarts a lieu un peu avant l'heure et les fonctions 
  sont réglées de telle manière que le premier coup frappé sur la cloche des heures marque 
  le moment exact de l'heure entière. Afin de réaliser cette exactitude, les marteaux sont, 
  au repos, tenus soulevés, de sorte qu'il ne faut qu'environ une seconde pour leur chute sur la cloche.
   Cette sonnerie forme un carillon particulièrement harmonieux qui est reproduit par beaucoup d'horloges
    d'appartements, et qui parfois est transmis par les émissions radiophoniques de T.S.F. de Londres,
     ce qui permet aux personnes possédant un bon poste récepteur de prendre l'heure d'après cette sonnerie.

  Les notes sont les suivantes, un groupe de quatre notes répondant toujours à un quart:
  Au 1er quart: la, sol, fa, do ;
  Au 2e quart: fa, la, sol, do - fa, sol, la, fa
  Au 3e quart: la, fa, sol, do - do, sol, la, fa - la, sol, fa, do
  Au 4e quart: fa, la, sol, do - fa, sol, la, fa - la, fa, sol, do - do, sol, la, fa et suivent les heures sur la cloche fa.
*/
