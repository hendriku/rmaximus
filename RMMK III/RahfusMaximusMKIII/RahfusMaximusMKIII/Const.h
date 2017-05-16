#pragma once
#include <limits>

// General
#define MAX_VALUE 1000000
#define MIN_VALUE -1000000
#define MAX_CALCULATION_TIME 1900
#define GAME_ENDING_TURN 60
#define BASE_DEPTH 3
#define SPEEDTEST false
#define INTEGRITY_CHECK false

// Field
#define XPLAYER 1
#define YPLAYER -1
#define EMPTY 0
#define SWAMP -2

// Board
#define ARRAYDIM 30
#define DIM 24
#define MAXDIM 36
#define MARGIN (int) 3
#define BRIDGED 10

// Evaluation
#define MULT_EVAL 10
#define GRAPH_SZ 200
#define BLOCKED_FIELD_VAL 99
#define UNUSED -1
#define NO_FATHER -2
#define MALUS_1 21
#define MALUS_2 13
#define BLOCKED_FIELD_VAL_MULT 990

// Move
#define SHIFT 5

// CritPos
#define UP false
#define DOWN true

// CheckPattern
#define FREE_STR "FREE"
#define OWN_STR "OWN"
#define OPP_STR "OPP"
#define SET_STR "SET"
#define BPOSS_STR "BPOSS"
#define BEXIST_STR "BEXIST"
#define STRONG_STR "STRONG"
#define NSTRONG_STR "NSTRONG"
#define FREE_NR 0
#define OWN_NR 1
#define OPP_NR 2
#define SET_NR 3
#define BPOSS_NR 4
#define BEXIST_NR 5
#define STRONG_NR 6
#define NSTRONG_NR 7
#define FILENAME "patterns.properties"
#define OFFENSIVE "OFF"
#define DEFENSIVE "DEF"
#define INTITAL_CAPACITY 10000
#define MILLI_PER_SEC 1000
#define GAMEOVER 1000

