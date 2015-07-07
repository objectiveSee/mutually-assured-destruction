# mutually-assured-destruction
Playa Del Fuego / Transformus 2015

# Fire Patterns

A fire pattern is specially coded array of numbers that define how long the fire exhaust is open and closed for. You create an array of numbers that alternate how long the valve is closed, then open. For example, to fire three 300ms bursts, with 1 second in between them, the pattern would look like:

```c
const unsigned char BURST[] PROGMEM = {0,30,100,30,100,30,100,30,STOP};
```

In the example above, a C array is created with values between 0 and 254. The value 255 is reserved for the STOP character, which must be at the end of the sequence. The unit of time is deci-seconds (ie. 1 = 10ms, 10=100ms, 100=1000ms).

Fire Patterns can then be added to the source code of the app and assigned to a button on the IR remote control. Patterns are declared in program memory in `fire-patterns.h` (https://github.com/objectiveSee/mutually-assured-destruction/blob/master/fire-patterns.h) and assigned to buttons in the .ino file (https://github.com/objectiveSee/mutually-assured-destruction/blob/master/mutually-assured-destruction.ino)


## Testing Patterns

`pattern-test.js`: A Node.js script that lets you simulate a pattern in the console.

`midifire.coffee`: A Coffeescript that takes a MIDI file as input, extracts two channels, and encodes those channels as fire patterns. 
