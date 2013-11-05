InfoThink IT-100MU Contactless Reader SDK V1.3版 目錄說明

-doc
 - IT100MU_WINNFC_SDK_v1_0.pdf: IT-100MU 讀卡機 非接觸介面開發 API 說明文件
 - M001053_MF1ICS50_rev5_3.pdf: MIFARE卡規格文件
 - ISOIEC 14443-4_ed20.pdf: ISO/IEC 14443-4規格文件
-driver
 目前 driver 可以支援 WIN2000 以上 32位元 CPU的 Window 作業系統.

-example
 - vbWinNfc.zip: VB 整合範例 (VS2008)
 - WinNfcTest.zip: VC範例 (VS2008), 裡面有三個測試範例:
   - test_LEDAndBuzzer(): IT-100MU讀卡機 LED燈和 Buzzer測試範例
   - Test_Mifare(): Mifare卡測試範例, SDK附三張
   - Test_ISO1443(): ISO1443卡片測試範例, 可使用 ETC卡來測試
 - NfcCtrl.zip: ActiveX 範例 (VS2008), ZIP檔解開後 cab目錄下有網頁範例 Mifare.htm

-library
 -vb: Virual Basic 模組檔, 內容為 WinNfc.dll 的宣告方式
 -vc: Virual C/C++ header file(*.h); Dynamic link library(WinNfc.dll)
 