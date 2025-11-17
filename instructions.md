# For native builds

```bash
mkdir build && cd build
cmake ..
make
./clay_sdl3_demo
```

Every 104px scrolled the containers created needs to increment by 1
At start create 0-9
+104px
create 10-19

```c
for (int i = 0; i < MAX_VISIBLE_HEADERS; i++) {
    MakeCreatureHeader(i);
}

// create the stat block element at the same time as the element that holds the headers, but don't fill it with stats
// Fill it with empty string variables that get filled when the appropriate header button is selected
// Then cleared out when closed, or when a new header is selected before being rewritten


```

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=debug ..
make
./clay_sdl3_demo
```

```sql
CREATE TABLE monsters ( id INTEGER PRIMARY KEY, name TEXT, cr TEXT, type TEXT, size TEXT, armor_class INTEGER, hitpoints_avg INTEGER, hit_dice TEXT, hitpoints_roll TEXT, speed_type TEXT, speed_walk INTEGER, speed_fly INTEGER, speed_swim INTEGER, speed_climb INTEGER, speed_burrow INTEGER, alignment TEXT, legendary TEXT, str INTEGER, dex INTEGER, con INTEGER, int INTEGER, wis INTEGER, cha INTEGER, prof_bonus INTEGER, throw_str INTEGER, throw_dex INTEGER, throw_con INTEGER, throw_int INTEGER, throw_wis INTEGER, throw_cha INTEGER, skills TEXT, languages TEXT, senses TEXT, range_darkvision INTEGER, range_tremorsense INTEGER, range_blindsight INTEGER, range_truesight INTEGER, special_ability_one TEXT, special_ability_one_desc TEXT, special_ability_two TEXT, special_ability_two_desc TEXT, special_ability_three TEXT, special_ability_three_desc TEXT, special_ability_four TEXT, special_ability_four_desc TEXT, attack_1 TEXT, attack_1_desc TEXT, attack_2 TEXT, attack_2_desc TEXT, attack_3 TEXT, attack_3_desc TEXT, attack_4 TEXT, attack_4_desc TEXT, attack_5 TEXT, attack_5_desc TEXT, attack_6 TEXT, attack_6_desc TEXT, action_leg TEXT, action_leg1 TEXT, action_leg1_desc TEXT, action_leg2 TEXT, action_leg2_desc TEXT, action_leg3 TEXT, action_leg3_desc TEXT, action_lair TEXT, action_lair1 TEXT, action_lair1_desc TEXT, action_lair2 TEXT, action_lair2_desc TEXT, action_lair3 TEXT, action_lair3_desc TEXT, regionaleffect TEXT, regionaleffect1 TEXT, regionaleffect2 TEXT, regionaleffect3 TEXT, endregionaleffect TEXT, environment TEXT, ba1 TEXT, ba1_desc TEXT, ba2 TEXT, ba2_desc TEXT, ba3 TEXT, ba3_desc TEXT, ba4 TEXT, ba4_desc TEXT, reaction1 TEXT, reaction1_desc TEXT, reaction2 TEXT, reaction2_desc TEXT, reaction3 TEXT, reaction3_desc TEXT, vill_action TEXT, vill_action1 TEXT, vill_action1_desc TEXT, vill_action2 TEXT, vill_action2_desc TEXT, vill_action3 TEXT, vill_action3_desc TEXT, utility_spells TEXT, utility_spells_list TEXT, feature1 TEXT, feature1_desc TEXT, feature2 TEXT, feature2_desc TEXT, feature3 TEXT, feature3_desc TEXT, feature4 TEXT, feature4_desc TEXT, feature5 TEXT, feature5_desc TEXT );

CREATE TABLE monsterstemp ( name TEXT, cr TEXT, type TEXT, size TEXT, armor_class INTEGER, hitpoints_avg INTEGER, hit_dice TEXT, hitpoints_roll TEXT, speed_type TEXT, speed_walk INTEGER, speed_fly INTEGER, speed_swim INTEGER, speed_climb INTEGER, speed_burrow INTEGER, alignment TEXT, legendary TEXT, str INTEGER, dex INTEGER, con INTEGER, int INTEGER, wis INTEGER, cha INTEGER, prof_bonus INTEGER, throw_str INTEGER, throw_dex INTEGER, throw_con INTEGER, throw_int INTEGER, throw_wis INTEGER, throw_cha INTEGER, skills TEXT, languages TEXT, senses TEXT, range_darkvision INTEGER, range_tremorsense INTEGER, range_blindsight INTEGER, range_truesight INTEGER, special_ability_one TEXT, special_ability_one_desc TEXT, special_ability_two TEXT, special_ability_two_desc TEXT, special_ability_three TEXT, special_ability_three_desc TEXT, special_ability_four TEXT, special_ability_four_desc TEXT, attack_1 TEXT, attack_1_desc TEXT, attack_2 TEXT, attack_2_desc TEXT, attack_3 TEXT, attack_3_desc TEXT, attack_4 TEXT, attack_4_desc TEXT, attack_5 TEXT, attack_5_desc TEXT, attack_6 TEXT, attack_6_desc TEXT, action_leg TEXT, action_leg1 TEXT, action_leg1_desc TEXT, action_leg2 TEXT, action_leg2_desc TEXT, action_leg3 TEXT, action_leg3_desc TEXT, action_lair TEXT, action_lair1 TEXT, action_lair1_desc TEXT, action_lair2 TEXT, action_lair2_desc TEXT, action_lair3 TEXT, action_lair3_desc TEXT, regionaleffect TEXT, regionaleffect1 TEXT, regionaleffect2 TEXT, regionaleffect3 TEXT, endregionaleffect TEXT, environment TEXT, ba1 TEXT, ba1_desc TEXT, ba2 TEXT, ba2_desc TEXT, ba3 TEXT, ba3_desc TEXT, ba4 TEXT, ba4_desc TEXT, reaction1 TEXT, reaction1_desc TEXT, reaction2 TEXT, reaction2_desc TEXT, reaction3 TEXT, reaction3_desc TEXT, vill_action TEXT, vill_action1 TEXT, vill_action1_desc TEXT, vill_action2 TEXT, vill_action2_desc TEXT, vill_action3 TEXT, vill_action3_desc TEXT, utility_spells TEXT, utility_spells_list TEXT, feature1 TEXT, feature1_desc TEXT, feature2 TEXT, feature2_desc TEXT, feature3 TEXT, feature3_desc TEXT, feature4 TEXT, feature4_desc TEXT, feature5 TEXT, feature5_desc TEXT );

INSERT INTO monsters (
    name, cr, type, size, armor_class, hitpoints_avg, hit_dice, hitpoints_roll,
    speed_type, speed_walk, speed_fly, speed_swim, speed_climb, speed_burrow,
    alignment, legendary, str, dex, con, int, wis, cha, prof_bonus,
    throw_str, throw_dex, throw_con, throw_int, throw_wis, throw_cha,
    skills, languages, senses, range_darkvision, range_tremorsense, range_blindsight,
    range_truesight, special_ability_one, special_ability_one_desc,
    special_ability_two, special_ability_two_desc, special_ability_three,
    special_ability_three_desc, special_ability_four, special_ability_four_desc,
    attack_1, attack_1_desc, attack_2, attack_2_desc, attack_3, attack_3_desc,
    attack_4, attack_4_desc, attack_5, attack_5_desc, attack_6, attack_6_desc, action_leg,
    action_leg1, action_leg1_desc, action_leg2, action_leg2_desc, action_leg3,
    action_leg3_desc, action_lair, action_lair1, action_lair1_desc,
    action_lair2, action_lair2_desc, action_lair3, action_lair3_desc,
    regionaleffect, regionaleffect1, regionaleffect2, regionaleffect3,
    endregionaleffect, environment, ba1, ba1_desc, ba2, ba2_desc, ba3, ba3_desc,
    ba4, ba4_desc, reaction1, reaction1_desc, reaction2, reaction2_desc,
    reaction3, reaction3_desc, vill_action, vill_action1, vill_action1_desc,
    vill_action2, vill_action2_desc, vill_action3, vill_action3_desc,
    utility_spells, utility_spells_list, feature1, feature1_desc, feature2,
    feature2_desc, feature3, feature3_desc, feature4, feature4_desc,
    feature5, feature5_desc
)
SELECT *
FROM monsterstemp;
```
