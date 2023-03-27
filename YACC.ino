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
unsigned long previousMillis = 0;
const long interval = 60000;  // intervale avant arrêt automatique (milliseconds)

//*******************  MAIN   *********************
void setup() {
  pinMode(OUT_PIN, OUTPUT);
  pinMode(OUT_ALIM, OUTPUT);
  digitalWrite(OUT_ALIM, HIGH) ;  // auto entretien de l'alim par saturation du 2N2907
 }

//*******************  LOOP   *********************
void loop() {
int i  = 0;
int ix = 0;
long frq;
unsigned long currentMillis = millis();

    i =  analogRead(TEST_PIN) ;delay(2);
    i += analogRead(TEST_PIN) ;delay(2);
    i += analogRead(TEST_PIN) ;delay(2);
    i += analogRead(TEST_PIN) ;delay(2);
    i = i/4; 
 
    if (i < 501) 
    {
    frq = map(i,0,500,0,24);
    tone(OUT_PIN, note[frq],100 );
    delay(100);

    if (currentMillis - previousMillis >= interval) {
        tone(OUT_PIN, 2500,1000 );
        delay(1000);  
        digitalWrite(OUT_ALIM, LOW) ;  // coupure de l'alim par désaturation du 2N2907  
      }
    }
}
  
