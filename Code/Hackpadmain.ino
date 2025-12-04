#include <Adafruit_TinyUSB.h>

// HID report: keyboard + consumer control
uint8_t const hidReportDescriptor[] = {
  TUD_HID_REPORT_DESC_KEYBOARD(),
  TUD_HID_REPORT_DESC_CONSUMER()
};

Adafruit_USBD_HID usb_hid(hidReportDescriptor, sizeof(hidReportDescriptor));

// ---------------------------
// Pin assignments
// ---------------------------
int encoderA = 1;
int encoderB = 2;

int btnFirefox   = 8;
int btnExplorer  = 9;
int btnTaskMgr   = 10;
int btnMinecraft = 11;

int ledPin = 5;

// Encoder tracking
int lastA = HIGH;

// ---------------------------
// Utility: Flash LED
// ---------------------------
void flashLED() {
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
}

// ---------------------------
// Utility: Type ASCII text
// (TinyUSB requires report_id = 0)
// ---------------------------
void typeText(const char* txt) {
  while (*txt) {
    usb_hid.keyboardPress(0, *txt);  // report 0, ASCII char
    delay(5);
    usb_hid.keyboardRelease(0);
    delay(5);
    txt++;
  }
}

// ---------------------------
// Send consumer control (volume)
// ---------------------------
void sendConsumer(uint16_t code) {
  usb_hid.sendReport16(1, code);   // report 1 = consumer
  delay(5);
  usb_hid.sendReport16(1, 0);
}

void setup() {
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);

  pinMode(btnFirefox, INPUT_PULLUP);
  pinMode(btnExplorer, INPUT_PULLUP);
  pinMode(btnTaskMgr, INPUT_PULLUP);
  pinMode(btnMinecraft, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT);

  usb_hid.begin();
  TinyUSBDevice.begin();
}

void loop() {

  // ---------------------------
  // ROTARY ENCODER VOLUME
  // ---------------------------
  int a = digitalRead(encoderA);
  if (a != lastA && a == LOW) {
    int b = digitalRead(encoderB);

    if (b == HIGH) {
      sendConsumer(HID_USAGE_CONSUMER_VOLUME_INCREMENT);
    } else {
      sendConsumer(HID_USAGE_CONSUMER_VOLUME_DECREMENT);
    }
  }
  lastA = a;

  // ---------------------------
  // BUTTONS
  // ---------------------------

  if (!digitalRead(btnFirefox)) {
    flashLED();
    typeText("firefox.exe\n");
    delay(300);
  }

  if (!digitalRead(btnExplorer)) {
    flashLED();
    typeText("explorer.exe\n");
    delay(300);
  }

  if (!digitalRead(btnTaskMgr)) {
    flashLED();

    // CTRL + SHIFT + ESC (TinyUSB way)
    uint8_t keys[6] = { HID_KEY_ESCAPE, 0,0,0,0,0 };
    usb_hid.keyboardReport(0, HID_KEY_CONTROL_LEFT | HID_KEY_SHIFT_LEFT, keys);

    delay(50);

    usb_hid.keyboardRelease(0);
    delay(300);
  }

  if (!digitalRead(btnMinecraft)) {
    flashLED();
    typeText("MinecraftLauncher.exe\n");
    delay(300);
  }
}
