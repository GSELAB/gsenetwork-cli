# gsenetwork-cli

Welcome to the c++ implementation of GSENetwork protocol. GSENetwork is a decentralized trust network for sharing economies that will disrupt the traditional model, eliminating the need for centralized intermediaries, through a three-pillar solution - DService, Incentive Model and Trust Network. GSENetwork envisions the future of the sharing economy as a global decentralized network which will seamlessly connect the interactions of every user, asset or service in the sharing economies anywhere in the world.

## Introduction
  * core: core common code.
  * cmd: executable program entry
  * client: http client
  * parse: command line parser

## Setup
```
$ mkdir -p ~/CLI
$ cd ~/CLI
$ git clone https://github.com/GSELAB/gsenetwork-cli.git
$ cd gsenetwork-cli
```

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Run GSE-CLI
```
# Normal GSE Node.
$ cd cmd
$ ./gsecli
```