/*
 * Pro Micro Joypad by Michael Erskine (msemtd@googlemail.com)
 * 
 * USB HID joypad on Sparkfun Pro Micro and clones thereof
 * (see https://www.sparkfun.com/products/12640)
 *  
 * Requires alteration of HID.cpp and USBAPI.h in Arduino 1.6.5
 * 
 * 
 * 
 */

#include "deb.h"

#define JM_UP    0x01
#define JM_DOWN  0x02
#define JM_LEFT  0x04
#define JM_RIGHT 0x08

const int RXLED = 17;
const uint8_t d_map[16] = {2,3,4,5,6,7,8,9,
                           10,16,14,15,18,19,20,21};

static uint16_t d_inputs = 0;
static debounce_t d_deb;
static JoyState_t JoySt;

void setup() {
  TX_RX_LED_INIT;
  pinMode(RXLED, OUTPUT);
  TXLED0;
  RXLED0;

  Serial.begin(9600);
  Serial1.begin(9600);

  JoySt.XAxis = 127;
  JoySt.YAxis = 127;
  JoySt.Buttons = 0;

  inputs_setup();
}

void inputs_setup(void) {
  for (int i = 0; i < 16; i++) {
    pinMode(d_map[i], INPUT_PULLUP);
  }
  d_inputs = inputs_read();
}

uint16_t inputs_read(void)
{
  uint16_t d_samp = 0;
  for (int i = 0; i < 16; i++) {
    bitWrite(d_samp, i, (digitalRead(d_map[i]) ? 0 : 1));
  }
  return d_samp;
}

void loop()
{
  task_led_toggle();
  task_inputs();
  task_serial_read();
}

/*
 * The inputs are sampled every 5 ms (max)
 * 
 * There are two sets of inputs: one from each row of pins
 * D2-D9 and D10,D16,D14,D15,D18-D21
 * 
 */
void task_inputs(void)
{
  static uint32_t last;
  uint32_t now = millis();
  if (now - last > 5) {
    last = now;
    uint16_t d_samp = inputs_read();
    d_samp = debouncer(d_samp, &d_deb);
    if (d_samp != d_inputs) {
      d_inputs = d_samp;
      //Serial.print("Inputs: ");
      //Serial.println(da_inputs, BIN);
    }

    JoySt.YAxis = JoySt.XAxis = 127;
    if (d_samp & JM_UP)
      JoySt.YAxis = 255;
    if (d_samp & JM_DOWN)
      JoySt.YAxis = 0;
    if (d_samp & JM_LEFT)
      JoySt.XAxis = 0;
    if (d_samp & JM_RIGHT)
      JoySt.XAxis = 255;
    JoySt.Buttons = d_samp >> 4;
    Joystick.setState(&JoySt);
  }
}

void task_led_toggle(void)
{
  static uint32_t last;
  static bool on = false;
  uint32_t now = millis();
  if (now - last > 1000) {
    last = now;
    // Serial.println("Hello world");  // Print "Hello World" to the Serial Monitor
    // Serial1.println("Hello!");  // Print "Hello!" over hardware UART
    if (on) {
      //RXLED1;
      //TXLED0;
    } else {
      //RXLED0;
      //TXLED1;
    }
    on = !on;
  }
}

void task_serial_read(void) {
  // TODO
  if(Serial.available()){
    int inByte = Serial.read();
    // Echo back...
    // Serial.write(inByte);
    // Add to message buffer...
    
    
  }

}

