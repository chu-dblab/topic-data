// WinNfcTest.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "WinNfc.h"


void ShowError(NFCCONTEXT ctx, LONG rc)
{
	
	NfcDisconnect(ctx, 0);
	printf("disconnect tsuccessfully\n");

	NfcReleaseContext(ctx);
	printf("release NFC functions successfully\n", rc);

	printf("error = %X\npress any key to exist\n", rc);
	getchar();
}

void ShowData(PBYTE pbData, DWORD dwLen)
{
	for (int i = 0; i < (int)dwLen; i++)
		printf("%02X ", *(pbData + i));

	printf("\n");
}



int Test_ISO1443()
{
	
	NFCCONTEXT phCtx = NULL;
	LONG rc = 0;

	//initialize NFC functions
	rc = NfcEstablishContext(NULL, NULL, NULL, &phCtx);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("initialize NFC functions successfully\n", rc);



	//connect to the NFC reader by the name given
	//ex: rc = NfcConnect(phCtx, "InfoThink IT-100MU 1", 0);
	//if name is NULL, will auto connect first reader founded
	rc = NfcConnect(phCtx, NULL, 0);

	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("auto connect NFC reader successfully\n");

	//Find card and select the first card
	rc = NfcSelectCard(phCtx, NFC_CARDTYPE_ISO14443_4A, NULL, 0);

	if (rc) { 
		ShowError(phCtx, rc);
		return 0;
	}
	printf("select card successfully\n");

	//1.select AID APDU
	BYTE SelectAID[] = {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x03,0x00,0x00,0x00};
	
	BYTE Rec[262] = {0};
	DWORD dwLen = 262;
	
	printf("Select AID \n");
	ShowData(SelectAID, 13);

	//transmit C=TL APDU command
	rc = ISO14443_SendAPDU(phCtx, (LPCBYTE)SelectAID, 13, Rec, &dwLen);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}

	printf("get response data successfully\n");
	ShowData(Rec, dwLen);



	//deselect the card if you don't use it anymore
	rc = NfcDeSelectCard(phCtx);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("DeSelect card successfully\n");

	//disconnect NFC device
	rc = NfcDisconnect(phCtx, 0);

	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("disconnect NFC reader successfully\n");

	//release NFC functions
	NfcReleaseContext(phCtx);
	printf("release NFC functions successfully\n", rc);

	printf("press any key to exist\n", rc);
	getchar();

	return 0;
}


int Test_Mifare(int Block, PBYTE KeyA, int Value)
{
	
	NFCCONTEXT phCtx = NULL;
	LONG rc = 0;

	//initialize NFC functions
	rc = NfcEstablishContext(NULL, NULL, NULL, &phCtx);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("initialize NFC functions successfully\n", rc);



	//connect to the NFC reader by the name given
	//ex: rc = NfcConnect(phCtx, "InfoThink IT-100MU 1", 0);
	//if name is NULL, will auto connect first reader founded
	rc = NfcConnect(phCtx, NULL, 0);

	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("auto connect NFC reader successfully\n");

	//Find card and select the first card
	rc = NfcSelectCard(phCtx, NFC_CARDTYPE_MIFARE, NULL, 0);

	if (rc) { 
		ShowError(phCtx, rc);
		return 0;
	}
	printf("select card successfully\n");


	//get Mifare Card ID
	char szCardID[9] = {0};
	rc = Mifare_GetCardID(phCtx, szCardID);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}

	printf("get MIFARE Card ID: %s \n", szCardID);
	
	//authenticate key A on Block N given
	rc = Mifare_KeyAuthority(phCtx, Block, KEY_TYPE_A, KeyA);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("authenticate key A successfully\n");


	/* 
		Part I
		Read or Write data test
	*/

	//read data from Block N
	BYTE data[16] = {0};
	DWORD dwLen = 16;
	rc = Mifare_ReadBlock(phCtx, Block, data, &dwLen);

	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}

	printf("read data from block %d successfully\n", Block);
	ShowData(data, 16);

	//write data to Block N
	PBYTE pData = (PBYTE)"\x01\x02\x03\x04\x05\x06\x07\x08\x01\x02\x03\x04\x05\x06\x07\x08";
	rc = Mifare_WriteBlock(phCtx, Block, pData, 16);

	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("write data to block %d successfully\n", Block);


	memset(data, 0x00, 16);
	dwLen = 16;
	rc = Mifare_ReadBlock(phCtx, Block, data, &dwLen);

	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}

	printf("read data from block %d successfully\n", Block);
	ShowData(data, 16);


	/* 
		Part II
		Read or Write value test
	*/

	//write value on Block N given
	rc = Mifare_WriteValue(phCtx, Block, Value);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("write value %d successfully\n", Value);


	/* 加值 200 元/點*/
	rc = Mifare_IncreaseValue(phCtx, Block, 200);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}

	rc = Mifare_TransferValue(phCtx, Block);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}

	printf("add 200 to block %d\n", Block);

	DWORD dwBalance = 0;
	rc = Mifare_ReadValue(phCtx, Block, &dwBalance);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("now block %d balance is %d\n", Block, dwBalance);

	
	/* 扣款 100 元/點 */
	rc = Mifare_DecreaseValue(phCtx, Block, 100);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}

	rc = Mifare_TransferValue(phCtx, Block);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("remove 100 from block %d\n", Block);

	dwBalance = 0;
	rc = Mifare_ReadValue(phCtx, Block, &dwBalance);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("now block %d balance is %d\n", Block, dwBalance);



	//deselect the card if you don't use it anymore
	rc = NfcDeSelectCard(phCtx);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("DeSelect card successfully\n");

	//disconnect NFC device
	rc = NfcDisconnect(phCtx, 0);

	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("disconnect NFC reader successfully\n");

	//release NFC functions
	NfcReleaseContext(phCtx);
	printf("release NFC functions successfully\n", rc);

	printf("press any key to exist\n", rc);
	getchar();

	return 0;
}


int test_LEDAndBuzzer()
{
	NFCCONTEXT phCtx = NULL;
	LONG rc = 0;

	//initialize NFC functions
	rc = NfcEstablishContext(0, 0, 0, &phCtx);
	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("initialize NFC functions successfully\n", rc);

	
	//connect to the NFC reader by the name given
	rc = NfcConnect(phCtx, "InfoThink IT-100MU 0", 0);

	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("connect to IT-100MU successfully\n");

	//LED test
	printf("LED test, press any key to start\n");
	getchar();

	printf("LED BLUE ON\n");
	rc = NfcLEDControl(phCtx, LED_BLUE);
	Sleep(500);

	printf("LED RED ON\n");
	rc = NfcLEDControl(phCtx, LED_RED);
	Sleep(500);

	printf("LED ALL ON\n");
	rc = NfcLEDControl(phCtx, LED_ALL);

	Sleep(500);
	printf("LED ALL OFF\n");
	rc = NfcLEDControl(phCtx, LED_OFF);

	//BUZZER test
	printf("BUZZER test, press any key to start\n");
	getchar();

	NfcBuzzerControl(phCtx, BUZZER_ON);
	Sleep(50);
	NfcBuzzerControl(phCtx, BUZZER_OFF);

	Sleep(500);

	NfcBuzzerControl(phCtx, BUZZER_ON);
	Sleep(100);
	NfcBuzzerControl(phCtx, BUZZER_OFF);

	Sleep(500);

	NfcBuzzerControl(phCtx, BUZZER_ON);
	Sleep(200);
	NfcBuzzerControl(phCtx, BUZZER_OFF);

	Sleep(500);

	NfcBuzzerControl(phCtx, BUZZER_ON);
	Sleep(400);
	NfcBuzzerControl(phCtx, BUZZER_OFF);

	Sleep(500);


	//disconnect NFC device
	rc = NfcDisconnect(phCtx, 0);

	if (rc) {
		ShowError(phCtx, rc);
		return 0;
	}
	printf("disconnect to IT-100MU successfully\n");

	//release NFC functions
	NfcReleaseContext(phCtx);
	printf("release NFC functions successfully\n", rc);

	printf("press any key to exist\n", rc);
	getchar();

	return 0;
}




int _tmain(int argc, _TCHAR* argv[])
{

	PBYTE KeyA = (PBYTE)"\xFF\xFF\xFF\xFF\xFF\xFF";

	//Test_Mifare(2, KeyA, 5000);

	Test_ISO1443();

	//test_LEDAndBuzzer();

	return 0;
}
