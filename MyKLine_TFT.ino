#define K_line_RX 16
#define K_line_TX 17
#define K_Serial Serial2
//
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "Free_Fonts.h"
#define SPRW1 200   //RPM
#define SPRH1 40
#define SPRW2 40    //THROTTLE
#define SPRH2 150
#define SPRW3 80    //TEMP
#define SPRH3 40
#define SPRW4 200   //VELOCITY
#define SPRH4 40
#define WORD_DELAY 5
#define REQUEST_DELAY 55
char throttle_[5];
char rpm_[5];
char temp1_[5];
char velocity_[5];

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
TFT_eSprite spr1 = TFT_eSprite(&tft);
TFT_eSprite spr2 = TFT_eSprite(&tft);
TFT_eSprite spr3 = TFT_eSprite(&tft);
TFT_eSprite spr4 = TFT_eSprite(&tft);
//
bool use_ISO14230_fast_init = false;
bool ISO14230_fast_init;
bool ISO9141_5_init;
int THROTTLE = 66;
int RPM = 666;
int VELOCITY = 6;
int TEMP1 = 0;
String s;
char byfer[30];

//  OBDii PID link: https://zh.wikipedia.org/zh-hans/OBD-II_PID
byte init_obd[] = {0xC1, 0x33, 0xF1, 0x81, 0x66};        // ISO14230 - fast init  C1 33 F1 81 66
byte rpm_obd[] = {0xC2, 0x33, 0xF1, 0x01, 0x0C, 0xF3};   // 引擎转速  C2 33 F1 01 0C F3
byte temp1_obd[] = {0xC2, 0x33, 0xF1, 0x01, 0x05, 0xEC}; // 发动机冷媒温度 C2 33 F1 01 05 EC
byte velocity_obd[] = {0xC2, 0x33, 0xF1, 0x01, 0x0D, 0xF4};    // 车速 C2 33 F1 01 0D F4
byte throttle_obd[] = {0xC2, 0x33, 0xF1, 0x01, 0x11, 0xF8}; // 油门位置 C2 33 F1 01 11 F8
//byte gear_obd[] = {0xC2, 0x33, 0xF1, 0x01, 0xA4, 0x8B}; // 变速箱挡位
// byte   temp2_obd[] = {0xC2,0x33,0xF1,0x01,0x0F,0xF6}; // 进气温度 C2 33 F1 01 0F F6

void setup()
{
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    spr1.createSprite(SPRW1, SPRH1);
    spr1.fillSprite(TFT_BLACK);
    spr2.createSprite(SPRW2, SPRH2);
    spr2.fillSprite(TFT_ORANGE);
    spr3.createSprite(SPRW3, SPRH3);
    spr3.fillSprite(TFT_BLACK);
    spr4.createSprite(SPRW4, SPRH4);
    spr4.fillSprite(TFT_BLACK);
    spr1.setTextDatum(MC_DATUM);
    spr2.setTextDatum(MC_DATUM);
    spr3.setTextDatum(MC_DATUM);
    spr4.setTextDatum(MC_DATUM);
    Serial.begin(115200);
    pinMode(K_line_RX, INPUT_PULLUP);
    pinMode(K_line_TX, OUTPUT);
    
    if (use_ISO14230_fast_init)
    {
        ISO14230_fast_init = true;
        ISO9141_5_init = false;
    }
    else
    {
        ISO14230_fast_init = false;
        ISO9141_5_init = true;
    }

}

void loop()
{
    spr1.fillSprite(TFT_BLACK);
    spr2.fillSprite(TFT_ORANGE); // 0x0000,rgb565,橙0xF3C0,赤橙0xEAA1
    spr3.fillSprite(TFT_BLACK); // 0x0000,rgb565,橙0xF3C0,赤橙0xEAA1
    spr4.fillSprite(TFT_BLACK); // 0x0000,rgb565,橙0xF3C0,赤橙0xEAA1
    read_K();
}

void read_K()
{
    if (ISO14230_fast_init == true)
    {
        K_Serial.end();
        char byfer1[30];
        digitalWrite(K_line_TX, HIGH), delay(300);
        digitalWrite(K_line_TX, LOW), delay(25);
        digitalWrite(K_line_TX, HIGH), delay(25);
        K_Serial.begin(10400); // ------------_- ISO 14230-4 KWP 10.4 Kbaud
        for (int i = 0; i < 5; i++)
        {
            K_Serial.write(init_obd[i]);
            delay(10);
        } // 初始化
        Serial.println("SEND: C1 33 F1 81 66");
        //delay(50);
        while(!K_Serial.available());
        int n = K_Serial.available();
        if (n > 0)
        {
            for (int i = 0; i < n; i++)
            {
                byfer1[i] = K_Serial.read();
                Serial.print(byfer1[i], HEX);
            }
        }
        Serial.println(n);
        ISO14230_fast_init = false;
    }
    if (ISO9141_5_init == true)
    {
        K_Serial.end();
        char byfer2[30];
        digitalWrite(K_line_TX, HIGH), delay(300);
        digitalWrite(K_line_TX, LOW), delay(200);
        digitalWrite(K_line_TX, HIGH), delay(400);
        digitalWrite(K_line_TX, LOW), delay(400);
        digitalWrite(K_line_TX, HIGH), delay(400);
        digitalWrite(K_line_TX, LOW), delay(400);
        digitalWrite(K_line_TX, HIGH), delay(200);
        // K_Serial.begin(5);
        // K_Serial.write(0x33);
        // K_Serial.end();
        // digitalWrite(K_line_TX, HIGH);
        K_Serial.begin(10400);
        delay(104); //103-105ms
        //or while(!K_Serial.available());
        int n = K_Serial.available();
        if (n > 0)
        {
            for (int i = 0; i < n; i++)
            {
                byfer2[i] = K_Serial.read();
                Serial.print(byfer2[i], HEX);
            }
        }
        Serial.println();
        Serial.print("size:");
        Serial.println(n);
        delay(25);
        K_Serial.write(0x70);
        delay(50);
        n = K_Serial.available();
        if (n > 0)
        {
            for (int i = 0; i < n; i++)
            {
                byfer2[i] = K_Serial.read();
                Serial.print(byfer2[i], HEX);
            }
        }
        Serial.println();
        Serial.print("size:");
        Serial.println(n);
        ISO9141_5_init = false;
    }
    Serial.flush();
    //get throttle
    for (int i = 0; i < 6; i++)
    {
        K_Serial.write(throttle_obd[i]);
        delay(WORD_DELAY); // 字节间间隔5-20ms
    }
    delay(REQUEST_DELAY);
    int ndata = K_Serial.available();
    if (ndata > 0)
    {
        for (int i = 0; i < ndata; i++)
        {
            byfer[i] = K_Serial.read();
        }
    }
    // String byte8 = String(byfer[8],DEC);   // С1 (HEX) = 193 (DEC)
    if (ndata == 13 && byfer[10] == 0X11 )
    {
        //THROTTLE = byfer[11]*100/255;
        THROTTLE = byfer[11]*100/180-14;
        for (int i = 0; i < 6; i++)
        {
            K_Serial.write(rpm_obd[i]);
            delay(WORD_DELAY);
        }
        delay(REQUEST_DELAY);
    }
    //get rpm
    ndata = K_Serial.available();
    if (ndata > 0)
    {
        for (int i = 0; i < ndata; i++)
        {
            byfer[i] = K_Serial.read();
            // Serial.print(byfer[i],HEX);
        }
    }

    if (ndata == 14 && byfer[10] == 0x0C)
    {
        RPM = (byfer[11]*256 + byfer[12])/4;
        for (int i = 0; i < 6; i++)
        {
            K_Serial.write(temp1_obd[i]);
            delay(WORD_DELAY);
        }
        delay(REQUEST_DELAY);
    }
    //get temp
    ndata = K_Serial.available();
    if (ndata > 0)
    {
        Serial.print("stamp");
        for (int i = 0; i < ndata; i++)
        {
            byfer[i] = K_Serial.read();
            //Serial.print(byfer[i],HEX);
            //Serial.print(" ");
        }
        //Serial.println("");
    }

    if (ndata == 13 && byfer[10] == 0x05)
    {
        TEMP1 = byfer[11] - 40;
        for (int i = 0; i < 6; i++)
        {
            K_Serial.write(velocity_obd[i]);
            delay(WORD_DELAY);
        }
        delay(REQUEST_DELAY);
    }
    
    //get velocity
    ndata = K_Serial.available();
    if (ndata > 0)
    {
        for (int i = 0; i < ndata; i++)
        {
            byfer[i] = K_Serial.read();
        }
    }

    if (ndata == 13 && byfer[10] == 0x0D)
    {
        VELOCITY = byfer[11];
    }
    
    sprintf(throttle_, "%d%%", THROTTLE);
    sprintf(rpm_, "%d", RPM);
    sprintf(temp1_, "%d\'c", TEMP1);
    sprintf(velocity_, "%d ", VELOCITY);
    //SHOW RPM
    if (RPM < 750)
        spr1.setTextColor(TFT_WHITE);
    else if (RPM >= 750 && RPM < 2000)
        spr1.setTextColor(TFT_GREEN);
    else if (RPM >= 2000 && RPM < 5000)
        spr1.setTextColor(0xF3C0);
    else
        spr1.setTextColor(TFT_RED);
    spr1.setTextDatum(ML_DATUM);
    spr1.setFreeFont(FF20); // 44
    spr1.drawString(rpm_, 70, spr1.height()/2, GFXFF);
    spr1.setFreeFont(FF18);
    spr1.setTextColor(TFT_WHITE);
    spr1.drawString("RPM:", 0, spr1.height()/2, GFXFF);
    spr1.pushSprite(tft.width() / 2 - spr1.width()*0.56, tft.height() / 2 - spr1.height()*1.3);
    //SHOW VELOCITY
    if (VELOCITY >= 120)
        spr4.setTextColor(TFT_RED);
    spr4.setTextDatum(ML_DATUM);
    spr4.setFreeFont(FF20); // 44
    spr4.setTextColor(TFT_GREENYELLOW);
    spr4.drawString(velocity_, 70, spr4.height()/2, GFXFF);
    spr4.setFreeFont(FF18);
    spr4.setTextColor(TFT_WHITE);
    spr4.drawString("VEL:", 0, spr4.height()/2, GFXFF);
    spr4.pushSprite(tft.width() / 2 - spr4.width()*0.56, tft.height() / 2 + spr4.height()*0.4);
    //SHOW THROTTLE
    spr2.drawRect(0, 0, spr2.width(), spr2.height(), TFT_WHITE);
    spr2.fillRect(0, 0, spr2.width(), spr2.height() - spr2.height() * THROTTLE / 100, TFT_WHITE);
    spr2.setTextColor(TFT_BLACK);
    spr2.setFreeFont(FF17);
    spr2.drawString(throttle_, spr2.width()/2, 20, GFXFF);
    spr2.pushSprite(260, tft.height()/4);
    //SHOW TEMP1
    spr3.setFreeFont(FF19); // 44
    spr3.setTextColor(TFT_ORANGE);
    spr3.drawString(temp1_, spr3.width()/2, spr3.height()/2, GFXFF);
    spr3.pushSprite(0, 0);
    
}
