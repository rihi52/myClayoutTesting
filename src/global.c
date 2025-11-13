#include "clay.h"
#include "global.h"

int WindowState = 0;

char TextBuffer[MAX_TEXT] = {0};

const int FONT_ID_BODY_16 = 0;
const int FONT_ID_BODY_32 = 0;

int WindowWidth = 0;
int WindowHeight = 0;
uint16_t TotalCreatures = 0;

float ScrollOffset = 0;

Clay_String StatName = {0};
Clay_String StatSize = {0};
Clay_String StatType = {0};
Clay_String StatAC = {0};
Clay_String StatHP = {0};
Clay_String StatSpeed = {0};
Clay_String StatStr = {0};
Clay_String StatDex = {0};
Clay_String StatCon = {0};
Clay_String StatInt = {0};
Clay_String StatWis = {0};
Clay_String StatCha = {0};

Clay_String StatSaves = {0};
Clay_String StatSkills = {0};
Clay_String StatSenses = {0};
Clay_String StatLanguages = {0};
Clay_String StatCR = {0};

Clay_String StatRacial1 = {0};
Clay_String StatRacial2 = {0};
Clay_String StatRacial3 = {0};

Clay_String StatAction1 = {0};
Clay_String StatAction2 = {0};
Clay_String StatAction3 = {0};
Clay_String StatAction4 = {0};
