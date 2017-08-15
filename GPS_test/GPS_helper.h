#ifndef _GPS_HELPER_H_
#define _GPS_HELPER_H_

#include <SoftwareSerial.h>

const int sentenceSize = 80;
char sentence[sentenceSize];


class SerialGPS
{
  public:
    //  RX_pin refers to the RX pin on the arduino
    //  connect the TX pin on the GPS to the RX pin on arduino
    //  TX on GPS is unused.
    SerialGPS(int RX_pin, int TX_pin)
    {
        m_gps = new SoftwareSerial(RX_pin, TX_pin);
        m_gps->begin(9600);
        Serial.begin(9600);
    }
    ~SerialGPS()
    {
        delete m_gps;
    }
    void getCoords()
    {
        static int i = 0;
        // only if gps is available, attempt to read
        if (m_gps->available())
        {
            // read each byte
            char ch = m_gps->read();
            if (ch != '\n' && i < sentenceSize)
            {
                // if not a new line or is less than max sentence size, keep reading
                sentence[i] = ch;
                i++;
            }
            else
            {
                // else we have a full sentence. now display it
                sentence[i] = '\0';
                i = 0;
                displayGPS();
            }
        }
    }

  private:
    SoftwareSerial *m_gps;
    void displayGPS()
    {
        char field[20];
        char latiNum[20];
        char latiDir[20];
        char longiNum[20];
        char longiDir[20];
        getField(field, 0);
        // taken from datasheet for Venus GPS receiver, found in DAQ drive
        // Structure:
        // $GPRMC,hhmmss.sss,A,dddmm.mmmm,a,dddmm.mmmm,a,x.x,x.x,ddmmyy,,,a*hh<CR><LF> 
        
        if (strcmp(field, "$GPRMC") == 0)
        {
            Serial.print("Lat: ");
            getField(latiNum, 3); // number
            Serial.print(latiNum);
            getField(latiDir, 4); // N/S
            Serial.print(latiDir);

            Serial.print(" Long: ");
            getField(longiNum, 5); // number
            Serial.print(longiNum);
            getField(longiDir, 6); // E/W
            Serial.println(longiDir);
        }
    }
    void getField(char *buffer, int index)
    {
        int sentencePos = 0;
        int fieldPos = 0;
        int commaCount = 0;
        while (sentencePos < sentenceSize)
        {
            if (sentence[sentencePos] == ',')
            {
                commaCount++;
                sentencePos++;
            }
            if (commaCount == index)
            {
                buffer[fieldPos] = sentence[sentencePos];
                fieldPos++;
            }
            sentencePos++;
        }
        buffer[fieldPos] = '\0';
    }
};

// Helper functions

#endif
