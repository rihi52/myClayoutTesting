# Roadmap

## Todo

* Access saved encounters
* Add players to database
* Player notes in database
* Edit players and creatures
* Remove players and creatures
* Functional scrollbars
* Keyboard navigation and shortcuts
* Shift around initiaive in combat
  * This is possible by going back to add, you can change the initiative of existing combatants
  * Drag and drop would be ideal and will be investigated
* Editable name in combat screen, but not affect statblock of db
* Database for saving encounters

  ## Completed

* Add participants mid comabt
* Arrow keys or spacebar to progress through initiative order
  * Highlight the current turn
  * Still have others selectable to adjust HP or bring up statblocks
* Add custom enemies/statblocks
* Add math to show ability modifiers


## Repetition
Repetitive code found in creature_db.c
1. Repeated validation condition
The same SDL_strcmp("0", ...) && SDL_strcmp("NULL", ...) pattern appears many times.
It is used for:
speed fields (StatSpeedWalk, StatSpeedBurrow, StatSpeedClimb, StatSpeedFly, StatSpeedSwim)
StatSavingThrows
StatSkills
StatSenses
StatLanguages
special abilities 1–3
attacks 2–6
legendary actions 1–3
lair reactions 1–3
regional effects 1–3 and StatEndRegionalEffect
2. Repeated ability score UI blocks in FillStats()
Six identical CLAY_AUTO_ID({StatPageAbilityDivider, ...}) blocks for ability labels:
STR, DEX, CON, INT, WIS, CHA
Six identical CLAY_AUTO_ID({StatPageAbilityDivider, ...}) blocks for ability values:
StatStr, StatDex, StatCon, StatInt, StatWis, StatCha
3. Repeated special ability display blocks
Three nearly identical blocks for:
StatSpecialAbilityOne / StatSpecialAbilityOneDesc
StatSpecialAbilityTwo / StatSpecialAbilityTwoDesc
StatSpecialAbilityThree / StatSpecialAbilityThreeDesc
4. Repeated attack display blocks
One always-shown block for StatAttack1 / StatAttack1Desc
Five repeated conditional blocks for StatAttack2–StatAttack6 with corresponding desc fields
5. Repeated legendary action blocks
Three nearly identical conditional blocks for:
StatActionLeg1 / StatActionLeg1Desc
StatActionLeg2 / StatActionLeg2Desc
StatActionLeg3 / StatActionLeg3Desc
6. Repeated lair reaction blocks
Three nearly identical conditional blocks for:
StatActionLair1 / StatActionLair1Desc
StatActionLair2 / StatActionLair2Desc
StatActionLair3 / StatActionLair3Desc
7. Repeated regional effect blocks
Four nearly identical conditional blocks for:
StatRegionalEffect
StatRegionalEffect1
StatRegionalEffect2
StatRegionalEffect3
plus StatEndRegionalEffect
8. Repeated input form fields in NewStatblockPage()
Six identical label blocks for ability names: STR, DEX, CON, INT, WIS, CHA
Six identical textbox blocks for ability inputs: STRTextBox, DEXTextBox, CONTextBox, INTTextBox, WISTextBox, CHATextBox
9. Repeated input sections for special abilities
Repeated blocks for:
SpecialAbility1NameTextBox / SpecialAbilityOneTextBox
SpecialAbility2NameTextBox / SpecialAbilityTwoTextBox
SpecialAbility3NameTextBox / SpecialAbilityThreeTextBox
10. Repeated input sections for attacks
Repeated name/description textbox pairs for:
StatAttack1TextBox / StatAttack1DescTextBox
StatAttack2TextBox / StatAttack2DescTextBox
StatAttack3TextBox / StatAttack3DescTextBox
StatAttack4TextBox / StatAttack4DescTextBox
StatAttack5TextBox / StatAttack5DescTextBox
StatAttack6TextBox / StatAttack6DescTextBox
11. Repeated input sections for legendary actions
Repeated blocks for:
StatActionLegTextBox / StatActionLegDescTextBox
StatActionLeg2TextBox / StatActionLegDesc2TextBox
StatActionLeg3TextBox / StatActionLegDesc3TextBox
12. Repeated UI text and focus logic
Many identical Clay_OnHover(FocusWindowCallback, gAppState);
Many identical FocusAndWriteTextBox(...) calls with just different IDs and textbox pointers