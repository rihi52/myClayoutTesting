#!/usr/bin/env python3
import json
import csv
import os

# ----------------------------------------------------------------------
#  EXACT column order from your INSERT statement
# ----------------------------------------------------------------------
# ----------------------------------------------------------------------
#  EXACT column order – NOW WITH attack_6
# ----------------------------------------------------------------------
HEADERS = [
    'name', 'cr', 'type', 'size', 'armor_class', 'hitpoints_avg', 'hit_dice', 'hitpoints_roll',
    'speed_type', 'speed_walk', 'speed_fly', 'speed_swim', 'speed_climb', 'speed_burrow',
    'alignment', 'legendary', 'str', 'dex', 'con', 'int', 'wis', 'cha', 'prof_bonus',
    'throw_str', 'throw_dex', 'throw_con', 'throw_int', 'throw_wis', 'throw_cha',
    'skills', 'languages', 'senses', 'range_darkvision', 'range_tremorsense', 'range_blindsight',
    'range_truesight', 'special_ability_one', 'special_ability_one_desc',
    'special_ability_two', 'special_ability_two_desc', 'special_ability_three',
    'special_ability_three_desc', 'special_ability_four', 'special_ability_four_desc',
    'attack_1', 'attack_1_desc', 'attack_2', 'attack_2_desc', 'attack_3', 'attack_3_desc',
    'attack_4', 'attack_4_desc', 'attack_5', 'attack_5_desc',
    'attack_6', 'attack_6_desc',          # <-- ADDED
    'action_leg', 'action_leg1', 'action_leg1_desc', 'action_leg2', 'action_leg2_desc',
    'action_leg3', 'action_leg3_desc', 'action_lair', 'action_lair1', 'action_lair1_desc',
    'action_lair2', 'action_lair2_desc', 'action_lair3', 'action_lair3_desc',
    'regionaleffect', 'regionaleffect1', 'regionaleffect2', 'regionaleffect3',
    'endregionaleffect', 'environment', 'ba1', 'ba1_desc', 'ba2', 'ba2_desc', 'ba3', 'ba3_desc',
    'ba4', 'ba4_desc', 'reaction1', 'reaction1_desc', 'reaction2', 'reaction2_desc',
    'reaction3', 'reaction3_desc', 'vill_action', 'vill_action1', 'vill_action1_desc',
    'vill_action2', 'vill_action2_desc', 'vill_action3', 'vill_action3_desc',
    'utility_spells', 'utility_spells_list', 'feature1', 'feature1_desc', 'feature2',
    'feature2_desc', 'feature3', 'feature3_desc', 'feature4', 'feature4_desc',
    'feature5', 'feature5_desc'
]

# ----------------------------------------------------------------------
#  Fill a block of columns (special abilities, attacks, legendary, etc.)
# ----------------------------------------------------------------------
# ----------------------------------------------------------------------
#  fill_action_fields – now supports up to 6 attacks
# ----------------------------------------------------------------------
def fill_action_fields(actions, base_name, max_items, is_attack=False):
    fields = {}
    for i in range(1, max_items + 1):
        if base_name == 'special_ability_one':
            name_key = f"special_ability_{'one' if i==1 else 'two' if i==2 else 'three' if i==3 else 'four'}"
        elif base_name == 'attack_1':
            name_key = f"attack_{i}"                     # 1 through 6
        elif base_name == 'action_leg1':
            name_key = f"action_leg{i}"
        else:
            name_key = f"{base_name}_{i}"
        fields[name_key] = ''
        fields[f"{name_key}_desc"] = ''

    for idx, act in enumerate(actions[:max_items]):
        name = act.get('name', '')
        desc = act.get('desc', '')

        if base_name == 'special_ability_one':
            col = ['one','two','three','four'][idx]
            name_key = f"special_ability_{col}"
        elif base_name == 'attack_1':
            name_key = f"attack_{idx+1}"                 # attack_1 → attack_6
        elif base_name == 'action_leg1':
            name_key = f"action_leg{idx+1}"
        else:
            name_key = f"{base_name}_{idx+1}"

        fields[name_key] = name
        fields[f"{name_key}_desc"] = desc
    return fields

# ----------------------------------------------------------------------
#  Process a single monster (unchanged except for the attack-5 fix)
# ----------------------------------------------------------------------
def process_monster(m):
    row = {h: '' for h in HEADERS}
    numeric_zero = {h: 0 for h in HEADERS if h.startswith(('speed_','range_','throw_','prof_bonus','cr','hitpoints_avg',
                                                          'action_leg','action_lair','regionaleffect','utility_spells'))}
    row.update(numeric_zero)

    # ---- basics -------------------------------------------------------
    row['name'] = m.get('name','')
    row['cr']   = m.get('challenge_rating',0)
    row['type'] = m.get('type','')
    row['size'] = m.get('size','')
    row['armor_class'] = m['armor_class'][0]['value'] if m.get('armor_class') else 0
    row['hitpoints_avg'] = m.get('hit_points',0)
    row['hit_dice']      = m.get('hit_dice','')
    row['hitpoints_roll']= m.get('hit_points_roll','')
    row['alignment']     = m.get('alignment','')
    row['str'] = m.get('strength',0)
    row['dex'] = m.get('dexterity',0)
    row['con'] = m.get('constitution',0)
    row['int'] = m.get('intelligence',0)
    row['wis'] = m.get('wisdom',0)
    row['cha'] = m.get('charisma',0)
    row['prof_bonus'] = m.get('proficiency_bonus',0)
    row['languages']  = m.get('languages','')

    # ---- speed --------------------------------------------------------
    st, w, f, s, c, b = parse_speed(m.get('speed',{}))
    row['speed_type']   = st
    row['speed_walk']   = w
    row['speed_fly']    = f
    row['speed_swim']  = s
    row['speed_climb']  = c
    row['speed_burrow'] = b

    # ---- senses -------------------------------------------------------
    sens_str, dv, tv, bv, trv = parse_senses(m.get('senses',{}))
    row['senses'] = sens_str
    row['range_darkvision']   = dv
    row['range_tremorsense']  = tv
    row['range_blindsight']   = bv
    row['range_truesight']    = trv

    # ---- saving throws AND skills ------------------------------------
    row.update(parse_proficiencies(m.get('proficiencies', [])))
    row['skills'] = parse_skills(m.get('proficiencies', []))
    # ---- special abilities (max 4) ------------------------------------
    specials = m.get('special_abilities',[])
    row.update(fill_action_fields(specials, 'special_ability_one', max_items=4, is_attack=False))

        # ---- ACTIONS – Multiattack first (now supports 6) -----------------
    actions = m.get('actions',[])
    multi = None
    other = []
    for a in actions:
        if a.get('name') == 'Multiattack':
            multi = a
        else:
            other.append(a)

    if multi:
        row['attack_1'] = multi.get('name','')
        row['attack_1_desc'] = multi.get('desc','')
        row.update(fill_action_fields(other, 'attack_1', max_items=5, is_attack=True))  # 5 more = 6 total
    else:
        row.update(fill_action_fields(actions, 'attack_1', max_items=6, is_attack=True))  # 6 total

    # ---- LEGENDARY ACTIONS --------------------------------------------
    leg = m.get('legendary_actions',[])
    row['action_leg'] = 1 if leg else 0
    row.update(fill_action_fields(leg, 'action_leg1', max_items=3, is_attack=False))

    # ---- placeholders -------------------------------------------------
    row['action_lair'] = 0
    row['regionaleffect'] = 0
    row['endregionaleffect'] = ''
    row['environment'] = ''

    # ---- spellcasting -------------------------------------------------
    for ab in specials:
        if ab.get('name') == 'Spellcasting':
            row['utility_spells'] = 1
            row['utility_spells_list'] = ab.get('desc','')
            break

    return row

# ----------------------------------------------------------------------
#  Helper functions (unchanged)
# ----------------------------------------------------------------------
def parse_speed(speed_dict):
    speed_types = []
    speeds = {'walk': 0, 'fly': 0, 'swim': 0, 'climb': 0, 'burrow': 0}
    for key, value in speed_dict.items():
        if not value or value == '0 ft.':
            continue
        try:
            val = int(value.split()[0])
            if key in speeds:
                speeds[key] = val
                speed_types.append(key)
        except Exception:
            pass
    return ','.join(speed_types), speeds['walk'], speeds['fly'], speeds['swim'], speeds['climb'], speeds['burrow']

def parse_senses(senses_dict):
    parts = []
    dark = trem = blind = true = 0
    for k, v in senses_dict.items():
        if k == 'passive_perception':
            parts.append(f"Passive Perception {v}")
        else:
            parts.append(f"{k} {v}")
            if k == 'darkvision':
                dark = int(v.split()[0])
            elif k == 'tremorsense':
                trem = int(v.split()[0])
            elif k == 'blindsight':
                blind = int(v.split()[0])
            elif k == 'truesight':
                true = int(v.split()[0])
    return ', '.join(parts), dark, trem, blind, true

def parse_proficiencies(proficiencies):
    throws = {'throw_str':0,'throw_dex':0,'throw_con':0,'throw_int':0,'throw_wis':0,'throw_cha':0}
    for p in proficiencies:
        idx = p['proficiency']['index']
        val = p['value']
        if idx.startswith('saving-throw-'):
            key = 'throw_' + idx.split('-')[-1][:3]
            throws[key] = val
    return throws

# ----------------------------------------------------------------------
#  NEW: parse the "proficiencies" array for SKILL entries
# ----------------------------------------------------------------------
def parse_skills(proficiencies):
    """Return a comma-separated string like: History +12, Perception +10"""
    skill_parts = []
    for p in proficiencies:
        idx = p['proficiency']['index']
        if idx.startswith('skill-'):
            skill_name = idx.split('-', 1)[1].capitalize()   # "history" -> "History"
            skill_val  = p['value']
            # Optional: add proficiency bonus sign only if >0
            sign = '+' if skill_val >= 0 else ''
            skill_parts.append(f"{skill_name} {sign}{skill_val}")
    return ', '.join(skill_parts)

# ----------------------------------------------------------------------
#  Main
# ----------------------------------------------------------------------
def main(in_json='monsters.json', out_csv='monsters.csv'):
    if not os.path.exists(in_json):
        print(f"Error: {in_json} not found.")
        return

    with open(in_json, 'r', encoding='utf-8') as f:
        data = json.load(f)

    rows = [process_monster(m) for m in data]

    with open(out_csv, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=HEADERS)
        writer.writeheader()
        writer.writerows(rows)

    print(f"Success: {len(rows)} monsters -> {out_csv}")

if __name__ == '__main__':
    main()