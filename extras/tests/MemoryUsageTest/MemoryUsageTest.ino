// MemoryUsageTest.ino
//
// Memory Usage unit tests.  Expected to run on Arduino UNO (ATMega328)
// (tests may fail on other boards)
//
// REQUIRES: MemoryUsage, AUnit libraries.
//
#include <MemoryUsage.h>

#include <AUnit.h>
using namespace MU;

STACK_DECLARE

///////////////////////////////////////////////////////////////
test(heapStart) {
    int heapStart = getHeapStart();
    int dataStart = getDataStart();
    int memoryEnd = getMemoryEnd();
    int approxGlobalSize = 350;
    assertMore(heapStart, dataStart + approxGlobalSize);   // heap after globals
    assertLess(heapStart, memoryEnd);
    //assertEqual(908,heapStart);    // too specific! why this value?
}

///////////////////////////////////////////////////////////////
test(heapEnd) {
    int heapEnd = getHeapEnd();
    int heapStart = getHeapStart();
    int memoryEnd = getMemoryEnd();

    assertMoreOrEqual(heapEnd, heapStart);
    assertLess(heapEnd, memoryEnd);

    int reasonableStack = 100;
    assertLess(heapEnd, (memoryEnd - reasonableStack));
    //assertEqual(912,he);    // too specific! why this?

    {
        String foo(F("this is something to put on the heap"));
        int heapWithString = getHeapEnd();
        assertMore(heapWithString, heapEnd);
        assertEqual(39, heapWithString - heapEnd);  // too specific?
        // foo goes out of scope (and cleaned up)
    }
    int heapAfterStringGone = getHeapEnd();
    assertEqual(heapEnd, heapAfterStringGone);
}

///////////////////////////////////////////////////////////////
test(heapSize) {
    int heapSize = getHeapSize();
    assertEqual(0, heapSize);

    {
        String foo(F("this is something to put on the heap"));
        int heapWithString = getHeapSize();
        assertMore(heapWithString, heapSize);
        assertEqual(39, heapWithString - heapSize);  // too specific?
        // foo goes out of scope (and cleaned up)
    }
    int finalHeapSize = getHeapSize();
    assertEqual(heapSize, finalHeapSize);
}

///////////////////////////////////////////////////////////////
test(stackSize) {
    int ss = getStackSize();
    assertEqual(12, ss);    // too specific?
}

///////////////////////////////////////////////////////////////
void  __attribute__ ((noinline)) STACK_COMPUTE_function(void)
{
    // treat this as a real function to ensure stack is up-to-date
    STACK_COMPUTE;
}

///////////////////////////////////////////////////////////////
void  __attribute__ ((noinline)) BIG_STACK_COMPUTE_function(void)
{
    // noinline:
    // treat this is a real function to exersise return stack
    // do not optimize the function call/return away!

    // volatile:
    // make this variable ready to change from outside this call chain
    // do not optimize the data away because it doesn't seem to be used!
    //
    char volatile foo[] = "something on the stack";
    String moreStuff = (char *) foo; // on the heap too

    STACK_COMPUTE_function();
}

///////////////////////////////////////////////////////////////
test(stackMaxSize) {
    int stackSize1 = getMaxStackSize();
    assertEqual(2, stackSize1);

    STACK_COMPUTE_function(); // first call to STACK_COMPUTE in this sketch
    int stackSize2 = getMaxStackSize();
    assertMore(stackSize2, stackSize1);

    int stackSize3 = 0;

    BIG_STACK_COMPUTE_function();
    stackSize3 = getMaxStackSize();
    assertMore(stackSize3, stackSize2);
    assertEqual(stackSize2 + 37, stackSize3); // too specific?

    STACK_COMPUTE_function();
    int stackSize4 = getMaxStackSize();
    assertEqual(stackSize3, stackSize4);    // no shrinking
}

///////////////////////////////////////////////////////////////
int __attribute__ ((noinline)) exerciseStack(void) {
    return getStackStart();
}

///////////////////////////////////////////////////////////////
int __attribute__ ((noinline)) useMoreFreeRAM(void) {
    const int numFoos = 100;
    int volatile foo[numFoos] = {0}; // use stack -- don't optimize away
    for (int i = 0; i < numFoos; i++) {
        foo[i] = getFreeRam();
    }
    foo[numFoos - 1] = getFreeRam();
    return foo[numFoos - 1];
}

///////////////////////////////////////////////////////////////
test(freeRam) {
    int frBefore = getFreeRam();
    assertMore(frBefore, 100);
    assertLess(frBefore, (int) getRamSize());

    //int currentStack = exerciseStack();
    exerciseStack();
    frBefore = getFreeRam();
    assertMore(frBefore, 100);
    assertLess(frBefore, (int) getRamSize());

    int frAfter = useMoreFreeRAM();

    assertLess(frAfter, frBefore);
    int deltaBytes = frAfter - frBefore;
    assertEqual(deltaBytes, -208); // too specific?
}

///////////////////////////////////////////////////////////////
test(ramSize) {
    int ramSize = getRamSize();
    assertEqual(2048, ramSize); // for ATmega328
}

///////////////////////////////////////////////////////////////
test(dataStart) {
    int ds = getDataStart();
    assertEqual(0x100, ds); // for ATmega328
}

///////////////////////////////////////////////////////////////
test(memoryEnd) {
    int me = getMemoryEnd();
    
    // NOTE memory end is the last byte of RAM
    // -- not the byte after
    int expectedLength = 2048-1; // 2047
    
    assertEqual(0x100 + expectedLength, me); // for ATmega328
}

///////////////////////////////////////////////////////////////
int __attribute__ ((noinline)) exerciseStackBigger(void) {
    const int numFoos = 100;
    int volatile foo[numFoos] = {0};
    for (int i = 0; i < numFoos; i++) {
        foo[i] = exerciseStack();
    }
    foo[0] = exerciseStack();
    return foo[0];
}

///////////////////////////////////////////////////////////////
test(stackStart) {
    int stackStart = getStackStart();
    int ds = getDataStart();
    int me = getMemoryEnd();
    assertMore(stackStart, ds + 350);   // should be after globals
    assertLess(stackStart, me);
    assertMore(stackStart, me - 50); // stack should not be very big here

    int moreStack = exerciseStackBigger(); // stack grows DOWN

    assertLess(moreStack, stackStart);
    assertMore(moreStack, stackStart - 230);

    assertEqual(moreStack, stackStart - 210); // too specific?
}

///////////////////////////////////////////////////////////////
test(stackWithTop) {
    char top = '!'; // top of stack
    int stackStart = (int) &top;
    int moreStack;  // why aren't these ints taking up room? register vars?
    {
        char top2 = '!';
        moreStack = (int) &top2;
    }
    int stackChange = stackStart - moreStack; // stack grows DOWN
    assertEqual((int) sizeof(char), stackChange);
}

///////////////////////////////////////////////////////////////
test(stack_SP_vsTop) {
    int stackStart;
    int stackStartSP;
    char top = '!'; // top of stack (used)
    stackStart = (int) (&top);
    stackStartSP = getStackStart(); // next (unused) byte

    assertEqual(stackStartSP + 1, stackStart);
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
    delay(1000); // wait for stability on some boards to prevent garbage Serial
    Serial.begin(115200);
    while (!Serial); // for the Arduino Leonardo/Micro only

    Serial.println(F( "Running " __FILE__ ", Built " __DATE__));
    Serial.println(F("This test should produce the following:"));
    Serial.println(
        F("12 passed, 0 failed, 0 skipped, 0 timed out, out of 12 test(s).")
    );
    Serial.println(F("----"));
}

void loop() {
    aunit::TestRunner::run();
}
