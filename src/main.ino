// #include <Arduino.h>
#include <SPI.h>
#include <Reader.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define DEBUG 1

// nfc readers
#define RST_PIN 10
#define SS1_PIN 9
#define SS2_PIN 8
#define SS3_PIN 7
#define SS4_PIN 6
MFRC522 nfc1(SS1_PIN, RST_PIN);
MFRC522 nfc2(SS2_PIN, RST_PIN);
MFRC522 nfc3(SS3_PIN, RST_PIN);
MFRC522 nfc4(SS4_PIN, RST_PIN);
Reader reader1(nfc1, RST_PIN, "ok");
Reader reader2(nfc2, RST_PIN, "ok");
Reader reader3(nfc3, RST_PIN, "ok");
Reader reader4(nfc4, RST_PIN, "ok");

// audio player
#define RX_PIN 11
#define TX_PIN 12
SoftwareSerial sws(RX_PIN, TX_PIN); // RX, TX
DFRobotDFPlayerMini mp3;

// relais
#define RELAY_PIN 5

bool previous = false;

void setup() {
	if (DEBUG) Serial.begin(115200);
	// audio player
	sws.begin(9600);
	if (!mp3.begin(sws)) while (true);
	mp3.volume(25);
	// nfc readers
	SPI.begin();
	reader1.init();
	reader2.init();
	reader3.init();
	reader4.init();
	// all readers start in low power mode
	pinMode(RST_PIN, OUTPUT);
	digitalWrite(RST_PIN, LOW);
	// relais
	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, LOW);
}

void loop() {
	if (allReadersOk()) {
		// all readers are ok for the first time
		// unlock
		// if not the first time do nothing
		if (!previous) unlock();
	}
	// not all players are ok
	// lock
	else lock();
	if (DEBUG) debug();
	delay(100);
}

boolean allReadersOk() {
	return reader1.check() && reader2.check() && reader3.check() && reader4.check();
}

void unlock() {
	// one relay NC and one NO + sound
	if (DEBUG) Serial.println("UNLOCK");
	mp3.playMp3Folder(1);
	digitalWrite(RELAY_PIN, HIGH);
	previous = true;
}

void lock() {
	if (DEBUG) Serial.println("LOCK");
	mp3.pause();
	digitalWrite(RELAY_PIN, LOW);
	previous = false;
}

void debug() {
	Serial.print("Reader1: "); Serial.println(reader1.check());
	Serial.print("Reader2: "); Serial.println(reader2.check());
	Serial.print("Reader3: "); Serial.println(reader3.check());
	Serial.print("Reader4: "); Serial.println(reader4.check());
}
