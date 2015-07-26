#include <MemoryUsage.h>

void setup() 
{
    Serial.begin(115200);
    FREERAM_PRINT;

    byte *p = new byte[3000];    
    
    FREERAM_PRINT;
}

void loop() 
{

}
