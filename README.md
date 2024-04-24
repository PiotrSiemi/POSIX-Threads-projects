# Ubuntu-system-clock
Simple system clock if you ever need to set the alarm in a Linux terminal!

## Usage
### To compile
Using gcc compiler:
```
gcc -o main  clock.c -pthread
```
### To run:
```
./main
```

## Remarks
This program bases on POSIX Threads standard which is used mainly in Unix family systems.
All comments included in main file.

# Threads are playing rock-paper-scissors
Rivarly of two threads. Depending on which thread won the previous round, it determines 
which thread will start the next one. Simple example of blocking resources using mutexes.

## Usage
### To compile
Using gcc compiler:
```
gcc -o main  game.c -pthread
```
### To run:
```
./main
```

## Remarks
This program bases on POSIX Threads standard which is used mainly in Unix family systems.
All comments included in main file.
