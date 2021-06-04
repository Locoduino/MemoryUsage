#include <MemoryUsage.h>

void setup() 
{
    Serial.begin(115200);
    Serial.println(F( "Running " __FILE__ ", Built " __DATE__));
    Serial.println(F("Starting state of the memory:"));
    Serial.println();
    
    MEMORY_PRINT_START
    MEMORY_PRINT_HEAPSTART
    MEMORY_PRINT_HEAPEND
    MEMORY_PRINT_STACKSTART
    MEMORY_PRINT_END
    MEMORY_PRINT_HEAPSIZE

    Serial.println();
    Serial.println();
   
    FREERAM_PRINT;
 
    //byte *p = new byte[3000];
    byte *p = new byte[300];  // Uno (ATmega328) only has 2k RAM

    if(!p) {
        Serial.println(F("could not allocate bytes for p[] array!"));
    }
        
    Serial.println();
    Serial.println();
    
    Serial.println(F("Ending state of the memory:"));
    Serial.println();
    
    MEMORY_PRINT_START
    MEMORY_PRINT_HEAPSTART
    MEMORY_PRINT_HEAPEND
    MEMORY_PRINT_STACKSTART
    MEMORY_PRINT_END
    MEMORY_PRINT_HEAPSIZE
    
    Serial.println();
    Serial.println();
    
    FREERAM_PRINT;

    delete p;
    p = 0;
}

void loop() 
{

}