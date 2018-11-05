#ifndef Reader_H
#define Reader_H

#include <Arduino.h>
#include <MFRC522.h>

class Reader {
	private:
		MFRC522 _nfc;
		int _rst;
		String _okWord;
		byte NFCKeyAByteArray[6] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7};
		MFRC522::MIFARE_Key key;

	public:
		Reader(MFRC522 nfc, int rst, String okWord);
		void init();
		bool check();
		bool checkMultiple();
};

#endif
