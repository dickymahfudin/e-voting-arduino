#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <ESP8266WiFi.h>
#include "api.h"

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

const int buttonPin1 = 14;
const int buttonPin2 = 12;
const int buttonPin3 = 13;

int button1 = 0;
int button2 = 0;
int button3 = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);

String ssid = "yuhu";
String password = "00000000";

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  lcd.init();
  nfc.begin();

  lcd.backlight();

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    lcdPrint("Connecting...", 0, 0);
  }
  lcd.clear();

  lcdPrint("Aru Pratomo Gunawan", 0, 0);
  lcdPrint("065117183", 6, 1);
  lcdPrint("", 1, 2);
  lcdPrint("Prototipe E-Voting", 1, 3);

  delay(5000);
  lcd.clear();
}

void loop()
{
  stateButton();
  lcdPrint("Pilih Menu", 0, 0);
  lcdPrint("1. Cek E-KTP", 0, 1);
  lcdPrint("2. Masuk Sistem", 0, 2);

  if (button1 == HIGH)
  {
    lcd.clear();
    registerVote();
  }
  else if (button2 == HIGH)
  {
    lcd.clear();
    runSystem();
  }
}

String readNfcTag()
{
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    String tagId = tag.getUidString();
    tagId.replace(" ", "");
    return tagId;
  }
  return "";
}

void registerVote()
{
  lcd.clear();

  lcdPrint("Menu Cek E-KTP", 3, 0);
  lcdPrint("Tempelkan E-KTP", 2, 2);
  while (1)
  {
    String tagId = readNfcTag();
    if (tagId != "")
    {
      lcd.clear();
      lcdPrint("Proses Mencari", 0, 0);
      lcdPrint("E-KTP Anda", 0, 1);
      getUser(tagId);
      if (user[1] != "null")
      {
        lcd.clear();
        lcdPrint("E-KTP Sudah", 0, 0);
        lcdPrint("Terdaftar", 0, 1);
        lcdPrint("Atas Nama:", 0, 2);
        lcdPrint(user[1], 0, 3);
        delay(4000);
        break;
      }
      else
      {
        lcd.clear();
        lcdPrint("E-KTP Belum", 0, 0);
        lcdPrint("Terdaftar", 0, 1);
        lcdPrint("Proses Mendaftarkan", 0, 2);
        lcdPrint("E-KTP Anda", 0, 3);
        registerUid(tagId);
        delay(4000);
        break;
      }
    }
  }
  lcd.clear();
}

void runSystem()
{
  lcd.clear();

  while (1)
  {
    lcdPrint("Tempelkan E-KTP Anda", 0, 0);
    lcdPrint("Untuk Memilih", 3, 1);
    String tagId = readNfcTag();
    if (tagId != "")
    {
      lcd.clear();
      lcdPrint("Proses Mencari", 0, 0);
      lcdPrint("E-KTP Anda", 0, 1);
      getUser(tagId);
      if (user[0] == "belum")
      {
        lcd.clear();
        lcdPrint(user[1], 0, 0);
        lcdPrint("Tentukan Pilihan", 1, 2);
        lcdPrint("Anda", 7, 3);
        delay(2000);
        lcd.clear();
        lcdPrint("Silahkan Memilih ", 0, 0);
        lcdPrint("1. Anies & Muhaimin", 0, 1);
        lcdPrint("2. Prabowo & Gibran", 0, 2);
        lcdPrint("3. Ganjar & Mahfud", 0, 3);

        while (1)
        {
          stateButton();
          if (button1 == HIGH)
          {
            lcdDone("satu", tagId);
            break;
          }
          else if (button2 == HIGH)
          {
            lcdDone("dua", tagId);
            break;
          }
          else if (button3 == HIGH)
          {
            lcdDone("tiga", tagId);
            break;
          }
          delay(300);
        }
      }
      else if (user[0] == "sudah")
      {
        lcd.clear();
        lcdPrint("Sudah Memilih", 0, 0);
        lcdPrint("Tidak Bisa Memilih", 0, 1);
        delay(4000);
        lcd.clear();
      }
      else
      {
        lcd.clear();
        lcdPrint("E-KTP Tidak", 0, 0);
        lcdPrint("Terdaftar", 0, 1);
        delay(4000);
        lcd.clear();
      }
    }
  }
}

void lcdPrint(String text, int x, int y)
{
  lcd.setCursor(x, y);
  lcd.print(text);
}

void lcdDone(String selection, String uid)
{
  lcd.clear();
  lcdPrint("Proses ...", 4, 0);
  delay(1000);
  lcd.clear();

  createResult(selection, uid);

  lcdPrint("Terimakasih", 4, 0);
  lcdPrint("Sudah Memilih", 4, 1);
  delay(4000);
  lcd.clear();
}

void stateButton()
{
  button1 = digitalRead(buttonPin1);
  button2 = digitalRead(buttonPin2);
  button3 = digitalRead(buttonPin3);
}
