#include "Reader.h"

Reader::Reader(MFRC522 nfc, int rst, String okWord) : _nfc(nfc), _rst(rst), _okWord(okWord) {}

void Reader::init() {
	for (byte i = 0; i < 6; i++) {
		key.keyByte[i] = NFCKeyAByteArray[i];
	}
	delay(20);
}

bool Reader::check() {
	// turn reader on
	digitalWrite(_rst, HIGH);
	_nfc.PCD_Init();
	// causes weird results
	// _nfc.PCD_SetAntennaGain(_nfc.RxGain_max);
	if (!_nfc.PICC_IsNewCardPresent()) return false;
	if (!_nfc.PICC_ReadCardSerial()) { Serial.println("failed to read card!"); return false; }
	byte sector = 1;
	byte trailerBlock = sector * 4 + 3;
	byte blockAddr = 4;
	byte buffer[18];
	byte size = sizeof(buffer);
	MFRC522::StatusCode status;
	status = (MFRC522::StatusCode) _nfc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(_nfc.uid));
	// failed to authenticate
	if (status != MFRC522::STATUS_OK) { Serial.println("failed to auth!"); return false; }
	status = (MFRC522::StatusCode) _nfc.MIFARE_Read(blockAddr, buffer, &size);
	// failed to read
	if (status != MFRC522::STATUS_OK) { Serial.println("failed to read block!"); return false; }
	// halt reader
	_nfc.PICC_HaltA();
	// stop encryption on reader
	_nfc.PCD_StopCrypto1();
	// turn it off
	digitalWrite(_rst, LOW);
	String result = String(String((char)buffer[11]) + String((char)buffer[12]));
	return result == _okWord;
}
