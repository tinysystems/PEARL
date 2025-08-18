# MACOP execution on MAX32666

## Introduction

The project is to compile and run the MACOP execution experiments on the MAX32666 dual-core planform. The execution adapts to the strength of ambient power.

## Dependencies

- [MAX SDK (2022)](https://analogdevicesinc.github.io/msdk/USERGUIDE/)
- [MAX32666FTHR Evaluation Kit](https://www.analog.com/en/resources/evaluation-hardware-and-software/evaluation-boards-kits/max32666fthr.html#eb-overview)

## Content of the Project

```
.
├── README.md       : This file
├── .settings       : Eclips settings
├── .vscode         : VSCode settings
├── config.h        : Configuration file
├── pearllib.c      : Pearl library functions
├── main.c          : Main application file
```

The main application file is divided into three blocks: ```DUAL_CORE_EXPERIMENT, SINGLE_CORE_EXPERIMENT, ADAPTIVE_EXPERIMENT```
The last one uses the Pearl library macros. By default in MAX32666, all the computations happening in ```main()``` belong to ```Core0```, while the computations code for ```Core1``` is given in ```main_core1()```. 

## How to run?

```
- Import the project to the MAX SDK
- Connect the evaluation kit
- Compile and run
```

## Output

MACOP and Power failure count in single- and dual-core modes separately.
