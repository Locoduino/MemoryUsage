#include <MemoryUsage.h>

STACK_DECLARE;

// Dummy structure sample, with a complex content...
struct rhaaa
{
    int ival[50];
    double dval[10];
    char text[80];
};

void subFull(rhaaa aSample);
void subPointer(rhaaa *apSample);
void subSmartPointer(rhaaa &aSample);
void subConstSmartPointer(const rhaaa &aSample);

void setup() 
{
    Serial.begin(115200);
    
    // An instance of the sample is declared, and the string is filled with
    // some string to see how to access to it inside functions !
    rhaaa sample; 
    strcpy(sample.text, "Test string");
    
    {
    // First attempt, just pass the structure. The full content is duplicated on the stack.
    // If the sub function modifies the content, the original structure from the caller
    // function will not be affected.
    STACK_START;
    STACK_PRINT(F("Stack Size start:"));
    subFull(sample);
    }
    
    {
    // Here we pass a pointer to the original structure instance.
    // Only this pointer is added to the stack.
    // The content is fully modifiable by the sub function.
    // This is the best way to let a sub funtion modify an argument.
    STACK_START;
    STACK_PRINT(F("Stack Size start:"));
    subPointer(&sample);
    }
    
    {
    // Here also, this is a pointer which is passed, but the sub function see its argument
    // as a normal data, not a pointer. Be careful here because the sub function can modify
    // the structure content and because this is not a pointer syntax, you can believe that
    // you only modify a copy !
    STACK_START;
    STACK_PRINT(F("Stack Size start:"));
    subSmartPointer(sample);
    }
    
    {
    // You have here the best way to pass a structure if you dont want to modify it.
    // Only a pointer is added to the stack, and any try to modify the struture content
    // will be detected as an error by the compiler.
    STACK_START;
    STACK_PRINT(F("Stack Size start:"));
    subConstSmartPointer(sample);
    }
}

void subFull(rhaaa aSample)
{
    Serial.println(aSample.text);
    STACK_PRINT(F("Stack Size subFull:"));
}

void subPointer(rhaaa *apSample)
{
    Serial.println(apSample->text);
    STACK_PRINT(F("Stack Size subPointer:"));
}

void subSmartPointer(rhaaa &aSample)
{
    Serial.println(aSample.text);
    STACK_PRINT(F("Stack Size subSmartPointer:"));
}

void subConstSmartPointer(const rhaaa &aSample)
{
    Serial.println(aSample.text);
    STACK_PRINT(F("Stack Size subConstSmartPointer:"));
}

void loop() 
{

}
