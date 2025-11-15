#include "db_query.h"
#include "global.h"

/*========================================================================*
 *  SECTION - External variables that cannot be defined in header files   *
 *========================================================================*
 */
sqlite3 * pGuidnbatterDB;

CreatureHeader DBPageHeaders[MAX_DB_COUNT] = {0};

/*========================================================================*
 *  SECTION - Global function definitions                                 *
 *========================================================================*
 */
void DatabaseOpen(void) {

    int rc = sqlite3_open("../guidnbatter.db", &pGuidnbatterDB);
    if (rc)
    {
        /* Exit if the database can't be opened */
        SDL_Log("Can't open database: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }
    else
    {
        SDL_Log("Opened database successfully");
    }

    // for (int i = 0; i < MAX_DB_COUNT; i++) {
    //     DBPageHeaders[i] = {0};
    // }
    return;
}

void DatabaseClose(void) {
    sqlite3_close(pGuidnbatterDB);
}

Clay_String MakeClayString(const char * string) {
    char * copy = SDL_malloc(SDL_strlen(string)+1);

    SDL_strlcpy(copy, string, SDL_strlen(string)+1);

    Clay_String str= {
        .isStaticallyAllocated = false,
        .length = SDL_strlen(copy),
        .chars = copy
    };

    return str;
}

void ClearClayString(Clay_String *s) {
    if (!s->isStaticallyAllocated && s->chars != NULL) {
        SDL_free((void*)s->chars);
    }
    s->chars = NULL;
    s->length = 0;
    s->isStaticallyAllocated = false;
}

Clay_String MakeClayIntString(int value) {
    char buffer[32];
    SDL_snprintf(buffer, sizeof(buffer), "%d", value);
    return MakeClayString(buffer);
}

int LoadCreatureHeaderAlphabetical(int MonsterId) {

    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT name, cr, size, type FROM monsters WHERE id = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, MonsterId + 1);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const char *Name    = sqlite3_column_text(stmt, 0);
        const char *Cr      = sqlite3_column_text(stmt, 1);
        const char *Size    = sqlite3_column_text(stmt, 2);
        const char *Type    = sqlite3_column_text(stmt, 3);
        //const char *Source  = sqlite3_column_text(stmt, 4);

        DBPageHeaders[MonsterId].CreatureName = MakeClayString(Name);
        DBPageHeaders[MonsterId].CreatureCR = MakeClayString(Cr);
        DBPageHeaders[MonsterId].CreatureSize = MakeClayString(Size);
        DBPageHeaders[MonsterId].CreatureType = MakeClayString(Type);
        
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
    if (NULL == DBPageHeaders[MonsterId].CreatureName.chars) return 1;
    return 0;
}

void LookUpCreatureStats(int MonsterId) {

    // Clear all Clay_Strings before writing new data
    ClearClayString(&StatId);
    ClearClayString(&StatName);
    ClearClayString(&StatCr);
    ClearClayString(&StatType);
    ClearClayString(&StatSize);

    ClearClayString(&StatArmorClass);
    ClearClayString(&StatHitpointsAvg);
    ClearClayString(&StatHitDice);
    ClearClayString(&StatHitpointsRoll);

    ClearClayString(&StatSpeedType);
    ClearClayString(&StatSpeedWalk);
    ClearClayString(&StatSpeedFly);
    ClearClayString(&StatSpeedSwim);
    ClearClayString(&StatSpeedClimb);
    ClearClayString(&StatSpeedBurrow);

    ClearClayString(&StatAlignment);
    ClearClayString(&StatLegendary);

    ClearClayString(&StatStr);
    ClearClayString(&StatDex);
    ClearClayString(&StatCon);
    ClearClayString(&StatInt);
    ClearClayString(&StatWis);
    ClearClayString(&StatCha);

    ClearClayString(&StatProfBonus);

    ClearClayString(&StatThrowStr);
    ClearClayString(&StatThrowDex);
    ClearClayString(&StatThrowCon);
    ClearClayString(&StatThrowInt);
    ClearClayString(&StatThrowWis);
    ClearClayString(&StatThrowCha);

    ClearClayString(&StatSavingThrows);

    ClearClayString(&StatSkills);
    ClearClayString(&StatLanguages);
    ClearClayString(&StatSenses);

    ClearClayString(&StatRangeDarkvision);
    ClearClayString(&StatRangeTremorsense);
    ClearClayString(&StatRangeBlindsight);
    ClearClayString(&StatRangeTruesight);

    ClearClayString(&StatSpecialAbilityOne);
    ClearClayString(&StatSpecialAbilityOneDesc);
    ClearClayString(&StatSpecialAbilityTwo);
    ClearClayString(&StatSpecialAbilityTwoDesc);
    ClearClayString(&StatSpecialAbilityThree);
    ClearClayString(&StatSpecialAbilityThreeDesc);
    ClearClayString(&StatSpecialAbilityFour);
    ClearClayString(&StatSpecialAbilityFourDesc);

    ClearClayString(&StatAttack1);
    ClearClayString(&StatAttack1Desc);
    ClearClayString(&StatAttack2);
    ClearClayString(&StatAttack2Desc);
    ClearClayString(&StatAttack3);
    ClearClayString(&StatAttack3Desc);
    ClearClayString(&StatAttack4);
    ClearClayString(&StatAttack4Desc);

    ClearClayString(&StatAttack5Type);
    ClearClayString(&StatAttack5Desc);

    ClearClayString(&StatActionLeg);
    ClearClayString(&StatActionLeg1);
    ClearClayString(&StatActionLeg1Desc);
    ClearClayString(&StatActionLeg2);
    ClearClayString(&StatActionLeg2Desc);
    ClearClayString(&StatActionLeg3);
    ClearClayString(&StatActionLeg3Desc);

    ClearClayString(&StatActionLair);
    ClearClayString(&StatActionLair1);
    ClearClayString(&StatActionLair1Desc);
    ClearClayString(&StatActionLair2);
    ClearClayString(&StatActionLair2Desc);
    ClearClayString(&StatActionLair3);
    ClearClayString(&StatActionLair3Desc);

    ClearClayString(&StatRegionalEffect);
    ClearClayString(&StatRegionalEffect1);
    ClearClayString(&StatRegionalEffect2);
    ClearClayString(&StatRegionalEffect3);
    ClearClayString(&StatEndRegionalEffect);

    ClearClayString(&StatEnvironment);

    ClearClayString(&StatBa1);
    ClearClayString(&StatBa1Desc);
    ClearClayString(&StatBa2);
    ClearClayString(&StatBa2Desc);
    ClearClayString(&StatBa3);
    ClearClayString(&StatBa3Desc);
    ClearClayString(&StatBa4);
    ClearClayString(&StatBa4Desc);

    ClearClayString(&StatReaction1);
    ClearClayString(&StatReaction1Desc);
    ClearClayString(&StatReaction2);
    ClearClayString(&StatReaction2Desc);
    ClearClayString(&StatReaction3);
    ClearClayString(&StatReaction3Desc);

    ClearClayString(&StatVillAction);
    ClearClayString(&StatVillAction1);
    ClearClayString(&StatVillAction1Desc);
    ClearClayString(&StatVillAction2);
    ClearClayString(&StatVillAction2Desc);
    ClearClayString(&StatVillAction3);
    ClearClayString(&StatVillAction3Desc);

    ClearClayString(&StatUtilitySpells);
    ClearClayString(&StatUtilitySpellsList);

    ClearClayString(&StatFeature1);
    ClearClayString(&StatFeature1Desc);
    ClearClayString(&StatFeature2);
    ClearClayString(&StatFeature2Desc);
    ClearClayString(&StatFeature3);
    ClearClayString(&StatFeature3Desc);
    ClearClayString(&StatFeature4);
    ClearClayString(&StatFeature4Desc);
    ClearClayString(&StatFeature5);
    ClearClayString(&StatFeature5Desc);
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT * FROM monsters WHERE id = ?";

    int rc = sqlite3_prepare_v2(pGuidnbatterDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        SDL_Log("Failed to prepare statement: %s", sqlite3_errmsg(pGuidnbatterDB));
        return;
    }

    sqlite3_bind_int(stmt, 1, MonsterId + 1);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        // int Id                           = sqlite3_column_int(stmt, 0);
        // const char *Name                 = (const char *)sqlite3_column_text(stmt, 1);
        // const char *Cr                   = (const char *)sqlite3_column_text(stmt, 2);
        // const char *Type                 = (const char *)sqlite3_column_text(stmt, 3);
        // const char *Size                 = (const char *)sqlite3_column_text(stmt, 4);

        // int Armor_Class                  = sqlite3_column_int(stmt, 5);
        // int Hitpoints_Avg                = sqlite3_column_int(stmt, 6);
        // const char *Hit_Dice             = (const char *)sqlite3_column_text(stmt, 7);
        // const char *Hitpoints_Roll       = (const char *)sqlite3_column_text(stmt, 8);

        // const char *Speed_Type           = (const char *)sqlite3_column_text(stmt, 9);
        // int Speed_Walk                   = sqlite3_column_int(stmt, 10);
        // int Speed_Fly                    = sqlite3_column_int(stmt, 11);
        // int Speed_Swim                   = sqlite3_column_int(stmt, 12);
        // int Speed_Climb                  = sqlite3_column_int(stmt, 13);
        // int Speed_Burrow                 = sqlite3_column_int(stmt, 14);

        // const char *Alignment            = (const char *)sqlite3_column_text(stmt, 15);
        // const char *Legendary            = (const char *)sqlite3_column_text(stmt, 16);

        // int Str                          = sqlite3_column_int(stmt, 17);
        // int Dex                          = sqlite3_column_int(stmt, 18);
        // int Con                          = sqlite3_column_int(stmt, 19);
        // int Int                          = sqlite3_column_int(stmt, 20);
        // int Wis                          = sqlite3_column_int(stmt, 21);
        // int Cha                          = sqlite3_column_int(stmt, 22);

        // int Prof_Bonus                   = sqlite3_column_int(stmt, 23);

        // int Throw_Str                    = sqlite3_column_int(stmt, 24);
        // int Throw_Dex                    = sqlite3_column_int(stmt, 25);
        // int Throw_Con                    = sqlite3_column_int(stmt, 26);
        // int Throw_Int                    = sqlite3_column_int(stmt, 27);
        // int Throw_Wis                    = sqlite3_column_int(stmt, 28);
        // int Throw_Cha                    = sqlite3_column_int(stmt, 29);

        // const char *Skills               = (const char *)sqlite3_column_text(stmt, 30);
        // const char *Languages            = (const char *)sqlite3_column_text(stmt, 31);
        // const char *Senses               = (const char *)sqlite3_column_text(stmt, 32);

        // int Range_Darkvision             = sqlite3_column_int(stmt, 33);
        // int Range_Tremorsense            = sqlite3_column_int(stmt, 34);
        // int Range_Blindsight             = sqlite3_column_int(stmt, 35);
        // int Range_Truesight             = sqlite3_column_int(stmt, 36);

        // const char *Special_Ability_One        = (const char *)sqlite3_column_text(stmt, 37);
        // const char *Special_Ability_One_Desc   = (const char *)sqlite3_column_text(stmt, 38);
        // const char *Special_Ability_Two        = (const char *)sqlite3_column_text(stmt, 39);
        // const char *Special_Ability_Two_Desc   = (const char *)sqlite3_column_text(stmt, 40);
        // const char *Special_Ability_Three      = (const char *)sqlite3_column_text(stmt, 41);
        // const char *Special_Ability_Three_Desc = (const char *)sqlite3_column_text(stmt, 42);
        // const char *Special_Ability_Four       = (const char *)sqlite3_column_text(stmt, 43);
        // const char *Special_Ability_Four_Desc  = (const char *)sqlite3_column_text(stmt, 44);

        // const char *Attack_1         = (const char *)sqlite3_column_text(stmt, 45);
        // const char *Attack_1_Desc    = (const char *)sqlite3_column_text(stmt, 46);
        // const char *Attack_2         = (const char *)sqlite3_column_text(stmt, 47);
        // const char *Attack_2_Desc    = (const char *)sqlite3_column_text(stmt, 48);
        // const char *Attack_3         = (const char *)sqlite3_column_text(stmt, 49);
        // const char *Attack_3_Desc    = (const char *)sqlite3_column_text(stmt, 50);
        // const char *Attack_4         = (const char *)sqlite3_column_text(stmt, 51);
        // const char *Attack_4_Desc    = (const char *)sqlite3_column_text(stmt, 52);

        // const char *Attack_5_Type    = (const char *)sqlite3_column_text(stmt, 53);
        // const char *Attack_5_Desc    = (const char *)sqlite3_column_text(stmt, 54);

        // const char *Action_Leg             = (const char *)sqlite3_column_text(stmt, 55);
        // const char *Action_Leg1            = (const char *)sqlite3_column_text(stmt, 56);
        // const char *Action_Leg1_Desc       = (const char *)sqlite3_column_text(stmt, 57);
        // const char *Action_Leg2            = (const char *)sqlite3_column_text(stmt, 58);
        // const char *Action_Leg2_Desc       = (const char *)sqlite3_column_text(stmt, 59);
        // const char *Action_Leg3            = (const char *)sqlite3_column_text(stmt, 60);
        // const char *Action_Leg3_Desc       = (const char *)sqlite3_column_text(stmt, 61);

        // const char *Action_Lair            = (const char *)sqlite3_column_text(stmt, 62);
        // const char *Action_Lair1           = (const char *)sqlite3_column_text(stmt, 63);
        // const char *Action_Lair1_Desc      = (const char *)sqlite3_column_text(stmt, 64);
        // const char *Action_Lair2           = (const char *)sqlite3_column_text(stmt, 65);
        // const char *Action_Lair2_Desc      = (const char *)sqlite3_column_text(stmt, 66);
        // const char *Action_Lair3           = (const char *)sqlite3_column_text(stmt, 67);
        // const char *Action_Lair3_Desc      = (const char *)sqlite3_column_text(stmt, 68);

        // const char *RegionalEffect         = (const char *)sqlite3_column_text(stmt, 69);
        // const char *RegionalEffect1        = (const char *)sqlite3_column_text(stmt, 70);
        // const char *RegionalEffect2        = (const char *)sqlite3_column_text(stmt, 71);
        // const char *RegionalEffect3        = (const char *)sqlite3_column_text(stmt, 72);
        // const char *EndRegionalEffect      = (const char *)sqlite3_column_text(stmt, 73);

        // const char *Environment            = (const char *)sqlite3_column_text(stmt, 74);

        // const char *BA1                    = (const char *)sqlite3_column_text(stmt, 75);
        // const char *BA1_Desc               = (const char *)sqlite3_column_text(stmt, 76);
        // const char *BA2                    = (const char *)sqlite3_column_text(stmt, 77);
        // const char *BA2_Desc               = (const char *)sqlite3_column_text(stmt, 78);
        // const char *BA3                    = (const char *)sqlite3_column_text(stmt, 79);
        // const char *BA3_Desc               = (const char *)sqlite3_column_text(stmt, 80);
        // const char *BA4                    = (const char *)sqlite3_column_text(stmt, 81);
        // const char *BA4_Desc               = (const char *)sqlite3_column_text(stmt, 82);

        // const char *Reaction1              = (const char *)sqlite3_column_text(stmt, 83);
        // const char *Reaction1_Desc         = (const char *)sqlite3_column_text(stmt, 84);
        // const char *Reaction2              = (const char *)sqlite3_column_text(stmt, 85);
        // const char *Reaction2_Desc         = (const char *)sqlite3_column_text(stmt, 86);
        // const char *Reaction3              = (const char *)sqlite3_column_text(stmt, 87);
        // const char *Reaction3_Desc         = (const char *)sqlite3_column_text(stmt, 88);

        // const char *Vill_Action            = (const char *)sqlite3_column_text(stmt, 89);
        // const char *Vill_Action1           = (const char *)sqlite3_column_text(stmt, 90);
        // const char *Vill_Action1_Desc      = (const char *)sqlite3_column_text(stmt, 91);
        // const char *Vill_Action2           = (const char *)sqlite3_column_text(stmt, 92);
        // const char *Vill_Action2_Desc      = (const char *)sqlite3_column_text(stmt, 93);
        // const char *Vill_Action3           = (const char *)sqlite3_column_text(stmt, 94);
        // const char *Vill_Action3_Desc      = (const char *)sqlite3_column_text(stmt, 95);

        // const char *Utility_Spells         = (const char *)sqlite3_column_text(stmt, 96);
        // const char *Utility_Spells_List    = (const char *)sqlite3_column_text(stmt, 97);

        // const char *Feature1               = (const char *)sqlite3_column_text(stmt, 98);
        // const char *Feature1_Desc          = (const char *)sqlite3_column_text(stmt, 99);
        // const char *Feature2               = (const char *)sqlite3_column_text(stmt, 100);
        // const char *Feature2_Desc          = (const char *)sqlite3_column_text(stmt, 101);
        // const char *Feature3               = (const char *)sqlite3_column_text(stmt, 102);
        // const char *Feature3_Desc          = (const char *)sqlite3_column_text(stmt, 103);
        // const char *Feature4               = (const char *)sqlite3_column_text(stmt, 104);
        // const char *Feature4_Desc          = (const char *)sqlite3_column_text(stmt, 105);
        // const char *Feature5               = (const char *)sqlite3_column_text(stmt, 106);
        // const char *Feature5_Desc          = (const char *)sqlite3_column_text(stmt, 107);


        // // Process Stats as needed
        // //SDL_Log("Creature Stats: %s", Stats);
        // SDL_Log("Id: %d", Id);
        // SDL_Log("Name: %s", Name);
        // SDL_Log("Cr: %s", Cr);
        // SDL_Log("Type: %s", Type);
        // SDL_Log("Size: %s", Size);

        // SDL_Log("Armor_Class: %d", Armor_Class);
        // SDL_Log("Hitpoints_Avg: %d", Hitpoints_Avg);
        // SDL_Log("Hit_Dice: %s", Hit_Dice);
        // SDL_Log("Hitpoints_Roll: %s", Hitpoints_Roll);

        // SDL_Log("Speed_Type: %s", Speed_Type);
        // SDL_Log("Speed_Walk: %d", Speed_Walk);
        // SDL_Log("Speed_Fly: %d", Speed_Fly);
        // SDL_Log("Speed_Swim: %d", Speed_Swim);
        // SDL_Log("Speed_Climb: %d", Speed_Climb);
        // SDL_Log("Speed_Burrow: %d", Speed_Burrow);

        // SDL_Log("Alignment: %s", Alignment);
        // SDL_Log("Legendary: %s", Legendary);

        // SDL_Log("Str: %d", Str);
        // SDL_Log("Dex: %d", Dex);
        // SDL_Log("Con: %d", Con);
        // SDL_Log("Int: %d", Int);
        // SDL_Log("Wis: %d", Wis);
        // SDL_Log("Cha: %d", Cha);

        // SDL_Log("Prof_Bonus: %d", Prof_Bonus);

        // SDL_Log("Throw_Str: %d", Throw_Str);
        // SDL_Log("Throw_Dex: %d", Throw_Dex);
        // SDL_Log("Throw_Con: %d", Throw_Con);
        // SDL_Log("Throw_Int: %d", Throw_Int);
        // SDL_Log("Throw_Wis: %d", Throw_Wis);
        // SDL_Log("Throw_Cha: %d", Throw_Cha);

        // SDL_Log("Skills: %s", Skills);
        // SDL_Log("Languages: %s", Languages);
        // SDL_Log("Senses: %s", Senses);

        // SDL_Log("Range_Darkvision: %d", Range_Darkvision);
        // SDL_Log("Range_Tremorsense: %d", Range_Tremorsense);
        // SDL_Log("Range_Blindsight: %d", Range_Blindsight);
        // SDL_Log("Range_Truesight: %d", Range_Truesight);

        // SDL_Log("Special_Ability_One: %s", Special_Ability_One);
        // SDL_Log("Special_Ability_One_Desc: %s", Special_Ability_One_Desc);
        // SDL_Log("Special_Ability_Two: %s", Special_Ability_Two);
        // SDL_Log("Special_Ability_Two_Desc: %s", Special_Ability_Two_Desc);
        // SDL_Log("Special_Ability_Three: %s", Special_Ability_Three);
        // SDL_Log("Special_Ability_Three_Desc: %s", Special_Ability_Three_Desc);
        // SDL_Log("Special_Ability_Four: %s", Special_Ability_Four);
        // SDL_Log("Special_Ability_Four_Desc: %s", Special_Ability_Four_Desc);

        // SDL_Log("Attack_1: %s", Attack_1);
        // SDL_Log("Attack_1_Desc: %s", Attack_1_Desc);
        // SDL_Log("Attack_2: %s", Attack_2);
        // SDL_Log("Attack_2_Desc: %s", Attack_2_Desc);
        // SDL_Log("Attack_3: %s", Attack_3);
        // SDL_Log("Attack_3_Desc: %s", Attack_3_Desc);
        // SDL_Log("Attack_4: %s", Attack_4);
        // SDL_Log("Attack_4_Desc: %s", Attack_4_Desc);

        // SDL_Log("Attack_5_Type: %s", Attack_5_Type);
        // SDL_Log("Attack_5_Desc: %s", Attack_5_Desc);

        // SDL_Log("Action_Leg: %s", Action_Leg);
        // SDL_Log("Action_Leg1: %s", Action_Leg1);
        // SDL_Log("Action_Leg1_Desc: %s", Action_Leg1_Desc);
        // SDL_Log("Action_Leg2: %s", Action_Leg2);
        // SDL_Log("Action_Leg2_Desc: %s", Action_Leg2_Desc);
        // SDL_Log("Action_Leg3: %s", Action_Leg3);
        // SDL_Log("Action_Leg3_Desc: %s", Action_Leg3_Desc);

        // SDL_Log("Action_Lair: %s", Action_Lair);
        // SDL_Log("Action_Lair1: %s", Action_Lair1);
        // SDL_Log("Action_Lair1_Desc: %s", Action_Lair1_Desc);
        // SDL_Log("Action_Lair2: %s", Action_Lair2);
        // SDL_Log("Action_Lair2_Desc: %s", Action_Lair2_Desc);
        // SDL_Log("Action_Lair3: %s", Action_Lair3);
        // SDL_Log("Action_Lair3_Desc: %s", Action_Lair3_Desc);

        // SDL_Log("RegionalEffect: %s", RegionalEffect);
        // SDL_Log("RegionalEffect1: %s", RegionalEffect1);
        // SDL_Log("RegionalEffect2: %s", RegionalEffect2);
        // SDL_Log("RegionalEffect3: %s", RegionalEffect3);
        // SDL_Log("EndRegionalEffect: %s", EndRegionalEffect);

        // SDL_Log("Environment: %s", Environment);

        // SDL_Log("BA1: %s", BA1);
        // SDL_Log("BA1_Desc: %s", BA1_Desc);
        // SDL_Log("BA2: %s", BA2);
        // SDL_Log("BA2_Desc: %s", BA2_Desc);
        // SDL_Log("BA3: %s", BA3);
        // SDL_Log("BA3_Desc: %s", BA3_Desc);
        // SDL_Log("BA4: %s", BA4);
        // SDL_Log("BA4_Desc: %s", BA4_Desc);

        // SDL_Log("Reaction1: %s", Reaction1);
        // SDL_Log("Reaction1_Desc: %s", Reaction1_Desc);
        // SDL_Log("Reaction2: %s", Reaction2);
        // SDL_Log("Reaction2_Desc: %s", Reaction2_Desc);
        // SDL_Log("Reaction3: %s", Reaction3);
        // SDL_Log("Reaction3_Desc: %s", Reaction3_Desc);

        // SDL_Log("Vill_Action: %s", Vill_Action);
        // SDL_Log("Vill_Action1: %s", Vill_Action1);
        // SDL_Log("Vill_Action1_Desc: %s", Vill_Action1_Desc);
        // SDL_Log("Vill_Action2: %s", Vill_Action2);
        // SDL_Log("Vill_Action2_Desc: %s", Vill_Action2_Desc);
        // SDL_Log("Vill_Action3: %s", Vill_Action3);
        // SDL_Log("Vill_Action3_Desc: %s", Vill_Action3_Desc);

        // SDL_Log("Utility_Spells: %s", Utility_Spells);
        // SDL_Log("Utility_Spells_List: %s", Utility_Spells_List);

        // SDL_Log("Feature1: %s", Feature1);
        // SDL_Log("Feature1_Desc: %s", Feature1_Desc);
        // SDL_Log("Feature2: %s", Feature2);
        // SDL_Log("Feature2_Desc: %s", Feature2_Desc);
        // SDL_Log("Feature3: %s", Feature3);
        // SDL_Log("Feature3_Desc: %s", Feature3_Desc);
        // SDL_Log("Feature4: %s", Feature4);
        // SDL_Log("Feature4_Desc: %s", Feature4_Desc);
        // SDL_Log("Feature5: %s", Feature5);
        // SDL_Log("Feature5_Desc: %s", Feature5_Desc);

        int col = 0;

        StatId                = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatName              = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatCr                = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatType              = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSize              = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatArmorClass        = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatHitpointsAvg      = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatHitDice           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatHitpointsRoll     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatSpeedType         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpeedWalk         = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatSpeedFly          = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatSpeedSwim         = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatSpeedClimb        = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatSpeedBurrow       = MakeClayIntString(sqlite3_column_int(stmt, col++));

        StatAlignment         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatLegendary         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatStr               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatDex               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatCon               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatInt               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatWis               = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatCha               = MakeClayIntString(sqlite3_column_int(stmt, col++));

        StatProfBonus         = MakeClayIntString(sqlite3_column_int(stmt, col++));

        int throwStr = sqlite3_column_int(stmt, col++);
        int throwDex = sqlite3_column_int(stmt, col++);
        int throwCon = sqlite3_column_int(stmt, col++);
        int throwInt = sqlite3_column_int(stmt, col++);
        int throwWis = sqlite3_column_int(stmt, col++);
        int throwCha = sqlite3_column_int(stmt, col++);

        // Temporary buffer to assemble the text
        char buffer[256] = {0};
        bool first = true;

        #define APPEND_THROW(label, value) \
            if (value != 0) { \
                if (!first) SDL_strlcat(buffer, ", ", sizeof(buffer)); \
                char temp[32]; \
                SDL_snprintf(temp, sizeof(temp), "%s +%d", label, value); \
                SDL_strlcat(buffer, temp, sizeof(buffer)); \
                first = false; \
            }

        // Build final formatted string
        APPEND_THROW("Str", throwStr);
        APPEND_THROW("Dex", throwDex);
        APPEND_THROW("Con", throwCon);
        APPEND_THROW("Int", throwInt);
        APPEND_THROW("Wis", throwWis);
        APPEND_THROW("Cha", throwCha);

        // Prefix the full thing if any throws exist
        char finalBuffer[300] = {0};
        if (!first) {
            SDL_snprintf(finalBuffer, sizeof(finalBuffer), "Saving Throws %s", buffer);
        }

        // Convert to Clay_String
        StatSavingThrows = MakeClayString(finalBuffer);


        StatSkills            = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatLanguages         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSenses            = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatRangeDarkvision   = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatRangeTremorsense  = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatRangeBlindsight   = MakeClayIntString(sqlite3_column_int(stmt, col++));
        StatRangeTruesight    = MakeClayIntString(sqlite3_column_int(stmt, col++));

        StatSpecialAbilityOne       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityOneDesc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityTwo       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityTwoDesc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityThree     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityThreeDesc= MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityFour      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatSpecialAbilityFourDesc = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatAttack1          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack1Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack2          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack2Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack3          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack3Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack4          = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack4Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatAttack5Type      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatAttack5Desc      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatActionLeg        = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg1       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg1Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg2       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg2Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg3       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLeg3Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatActionLair       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair1      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair1Desc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair2      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair2Desc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair3      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatActionLair3Desc  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatRegionalEffect    = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatRegionalEffect1   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatRegionalEffect2   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatRegionalEffect3   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatEndRegionalEffect = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatEnvironment       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatBa1               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa1Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa2               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa2Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa3               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa3Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa4               = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatBa4Desc           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatReaction1         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction1Desc     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction2         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction2Desc     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction3         = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatReaction3Desc     = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatVillAction        = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction1       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction1Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction2       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction2Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction3       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatVillAction3Desc   = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatUtilitySpells      = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatUtilitySpellsList  = MakeClayString((const char*)sqlite3_column_text(stmt, col++));

        StatFeature1           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature1Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature2           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature2Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature3           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature3Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature4           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature4Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature5           = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        StatFeature5Desc       = MakeClayString((const char*)sqlite3_column_text(stmt, col++));
        
    }
    else if (rc != SQLITE_DONE)
    {
        SDL_Log("Failed to execute statement: %s", sqlite3_errmsg(pGuidnbatterDB));
    }

    sqlite3_finalize(stmt);
}