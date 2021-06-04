#include <MemoryUsage.h>

// Simple example to report memory sizes

void reportAllocation(int numBytes) {

    Serial.print(F("Allocating for "));
    Serial.print( numBytes );
    Serial.print(F(" bytes; "));
    
    byte *p = new byte[numBytes];

    if (p) {
        Serial.println(F("...success."));
    } else {
        Serial.println(F("...allocation FAILED"));
    }

    MEMORY_PRINT_HEAPSIZE
    FREERAM_PRINT

    Serial.println(F("\ndeleting byte array with delete"));
    delete p;   // don't want a memory leak!
    p = 0;      // don't want a dangling/obsolete pointer

    MEMORY_PRINT_HEAPSIZE
    FREERAM_PRINT
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println(F( "Running " __FILE__ ", Built " __DATE__));

    Serial.println(F("\nStarting conditions"));
    MEMORY_PRINT_TOTALSIZE
    MEMORY_PRINT_HEAPSIZE
    FREERAM_PRINT

    Serial.println(F("\nallocate a byte array with new; too big to fit in RAM?"));
    reportAllocation(3000);

    Serial.println(F("\nallocate smaller byte array with new (it should fit)"));
    reportAllocation(300);

    Serial.println(F("\nFinal conditions"));
    MEMORY_PRINT_HEAPSIZE
    FREERAM_PRINT
}

void loop() {
    // User reads output from setup().
}
