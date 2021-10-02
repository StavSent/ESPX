# ESPX

Simulates MAC addresses' collection. 

## Installation

Compile addresses.c and simulator.c

```bash
gcc addresses.c -o addresses.out
gcc -pthrad simulator.c covidTrace.c -o simulator.c
```

## Usage

First run addresses.out with desired number of MAC addresses'. Then ./simulator.out

```bash 
./addresses.out 100
./simulator.out 100
```





