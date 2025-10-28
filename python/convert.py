import json
import csv

# === CONFIG ===
INPUT_JSON = "monsters.json" # Input JSON file path
OUTPUT_CSV = "monsters.csv" # Output CSV file name

# === CSV HEADER ===
headers = [
    "id","name","cr","type","size","armor_class","hitpoints_avg","hit_dice","hitpoints_roll",
    "speed_type","speed_walk","speed_fly","speed_swim","speed_climb","speed_burrow",
    "alignment","legendary","str","dex","con","int","wis","cha","prof_bonus",
    "throw_str","throw_dex","throw_con","throw_int","throw_wis","throw_cha",
    "skills","languages","senses","range_darkvision","range_tremorsense","range_blindsight","range_truesight",
    "special_ability_one","special_ability_one_desc","special_ability_two","special_ability_two_desc",
    "special_ability_three","special_ability_three_desc","special_ability_four","special_ability_four_desc",
    "attack_1","attack_1_desc","attack_2","attack_2_desc","attack_3","attack_3_desc","attack_4","attack_4_desc","attack_5_type","attack_5_desc",
    "action_leg","action_leg1","action_leg1_desc","action_leg2","action_leg2_desc","action_leg3","action_leg3_desc",
    "action_lair","action_lair1","action_lair1_desc","action_lair2","action_lair2_desc","action_lair3","action_lair3_desc",
    "regionaleffect","regionaleffect1","regionaleffect2","regionaleffect3","endregionaleffect",
    "environment","ba1","ba1_desc","ba2","ba2_desc","ba3","ba3_desc","ba4","ba4_desc",
    "reaction1","reaction1_desc","reaction2","reaction2_desc","reaction3","reaction3_desc",
    "vill_action","vill_action1","vill_action1_desc","vill_action2","vill_action2_desc","vill_action3","vill_action3_desc",
    "utility_spells","utility_spells_list",
    "feature1","feature1_desc","feature2","feature2_desc","feature3","feature3_desc","feature4","feature4_desc","feature5","feature5_desc"
]

# === HELPER FUNCTIONS ===

def get_value(data, key, default=0):
    """Safely get value from dict, or return default."""
    return data.get(key, default) if isinstance(data, dict) else default

def extract_first_n(entries, n):
    """Extract up to n (name, desc) pairs from a list of dicts."""
    pairs = []
    for i in range(n):
        if isinstance(entries, list) and i < len(entries):
            name = entries[i].get("name", 0)
            desc = entries[i].get("desc", 0)
        else:
            name, desc = 0, 0
        pairs.append((name, desc))
    return pairs

def extract_speed(speed_dict):
    """Normalize speed data (convert all to integers)."""
    out = {"type": 0, "walk": 0, "fly": 0, "swim": 0, "climb": 0, "burrow": 0}
    if not isinstance(speed_dict, dict):
        return out
    if speed_dict:
        out["type"] = ",".join(sorted(speed_dict.keys()))

    def parse_speed(val):
        if val is None:
            return 0
        if isinstance(val, int):
            return val
        if isinstance(val, str):
            digits = ''.join(ch for ch in val if ch.isdigit())
            return int(digits) if digits else 0
        return 0

    out["walk"] = parse_speed(speed_dict.get("walk", 0))
    out["fly"] = parse_speed(speed_dict.get("fly", 0))
    out["swim"] = parse_speed(speed_dict.get("swim", 0))
    out["climb"] = parse_speed(speed_dict.get("climb", 0))
    out["burrow"] = parse_speed(speed_dict.get("burrow", 0))
    return out

def extract_range(senses, key):
    """Extract numeric range from senses data."""
    val = get_value(senses, key, 0)
    if isinstance(val, int):
        return val
    if isinstance(val, str):
        digits = ''.join(ch for ch in val if ch.isdigit())
        return int(digits) if digits else 0
    return 0

def parse_saving_throws(mon):
    """Parse saving throws, handling multiple schema variations."""
    throws = {"str":0,"dex":0,"con":0,"int":0,"wis":0,"cha":0}
    saving_throws = mon.get("saving_throws") or mon.get("saves")

    # Case 1: direct saving throw dictionary
    if isinstance(saving_throws, dict) and saving_throws:
        mapping = {
            "str": "str", "strength": "str",
            "dex": "dex", "dexterity": "dex",
            "con": "con", "constitution": "con",
            "int": "int", "intelligence": "int",
            "wis": "wis", "wisdom": "wis",
            "cha": "cha", "charisma": "cha"
        }
        for k, v in saving_throws.items():
            key_lower = k.lower()
            if key_lower in mapping:
                sval = ''.join(ch for ch in str(v) if ch in "-0123456789")
                throws[mapping[key_lower]] = int(sval) if sval else 0
        return throws

    # Case 2: proficiencies list
    profs = mon.get("proficiencies", [])
    if isinstance(profs, list):
        for p in profs:
            val = p.get("value", None)
            prof = p.get("proficiency", {})
            pname = prof.get("name", "") or prof.get("index", "")
            if isinstance(pname, str):
                name_lower = pname.lower()
                if "saving throw" in name_lower:
                    for ab in ["str","dex","con","int","wis","cha"]:
                        if ab in name_lower:
                            throws[ab] = int(val) if val is not None else 0
    return throws

# === MAIN CONVERSION ===

with open(INPUT_JSON, "r", encoding="utf-8") as f:
    monsters = json.load(f)

with open(OUTPUT_CSV, "w", newline='', encoding="utf-8") as csvfile:
    writer = csv.DictWriter(csvfile, fieldnames=headers)
    writer.writeheader()

    for i, mon in enumerate(monsters, start=1):
        row = {h: 0 for h in headers}

        # Basic info
        row["id"] = i
        row["name"] = mon.get("name", 0)
        row["cr"] = str(mon.get("challenge_rating", "0"))
        row["type"] = mon.get("type", 0)
        row["size"] = mon.get("size", 0)
        row["alignment"] = mon.get("alignment", 0)

        # Armor class
        ac = mon.get("armor_class")
        if isinstance(ac, list) and ac:
            ac_item = ac[0]
            row["armor_class"] = ac_item.get("value") or ac_item.get("ac") or 0
        elif isinstance(ac, dict):
            row["armor_class"] = ac.get("ac", 0)
        else:
            row["armor_class"] = ac or 0

        # Hit points
        hp = mon.get("hit_points")
        if isinstance(hp, int):
            row["hitpoints_avg"] = hp
        elif isinstance(hp, dict):
            row["hitpoints_avg"] = hp.get("average", hp.get("value", 0))
        else:
            row["hitpoints_avg"] = hp or 0
        row["hit_dice"] = mon.get("hit_dice", 0)
        row["hitpoints_roll"] = mon.get("hit_points_roll", 0)

        # Speed
        speeds = extract_speed(mon.get("speed", {}))
        for k, v in speeds.items():
            row[f"speed_{k}"] = v

        # === Ability Scores ===
        ability_map = {
            "str": ["str", "strength"],
            "dex": ["dex", "dexterity"],
            "con": ["con", "constitution"],
            "int": ["int", "intelligence"],
            "wis": ["wis", "wisdom"],
            "cha": ["cha", "charisma"]
        }
        for key, aliases in ability_map.items():
            val = 0
            for alias in aliases:
                if alias in mon:
                    val = mon[alias]
                    break
            if not val and isinstance(mon.get("stats"), list):
                stats_list = mon["stats"]
                idx = list(ability_map.keys()).index(key)
                if len(stats_list) > idx:
                    val = stats_list[idx]
            row[key] = int(val or 0)

        # Proficiency bonus
        row["prof_bonus"] = mon.get("proficiency_bonus", mon.get("proficiency", 0)) or 0

        # Saving throws
        throws = parse_saving_throws(mon)
        for ab in ["str","dex","con","int","wis","cha"]:
            row[f"throw_{ab}"] = throws.get(ab, 0)

        # Skills
        skills_obj = mon.get("skills", {})
        if isinstance(skills_obj, dict) and skills_obj:
            row["skills"] = ", ".join(f"{k.upper()} +{v}" for k, v in skills_obj.items())
        else:
            row["skills"] = 0

        # Languages
        langs = mon.get("languages", 0)
        row["languages"] = ", ".join(langs) if isinstance(langs, list) else langs or 0

        # Senses
        senses = mon.get("senses", {})
        if isinstance(senses, dict) and senses:
            row["senses"] = ", ".join(f"{k} {v}" for k,v in senses.items())
            for sense in ["darkvision","tremorsense","blindsight","truesight"]:
                row[f"range_{sense}"] = extract_range(senses, sense)

        # Environment
        env = mon.get("environments", mon.get("environment", mon.get("environment_list", [])))
        row["environment"] = ", ".join(env) if isinstance(env, list) else env or 0

        # Special abilities
        special_abilities = extract_first_n(mon.get("special_abilities", []), 4)
        for idx, (name, desc) in enumerate(special_abilities, start=1):
            suffix = ["one","two","three","four"][idx-1]
            row[f"special_ability_{suffix}"] = name
            row[f"special_ability_{suffix}_desc"] = desc

        # Actions / attacks
        attacks = extract_first_n(mon.get("actions", []), 5)
        for idx, (name, desc) in enumerate(attacks, start=1):
            if idx == 5:
                row["attack_5_type"] = name
                row["attack_5_desc"] = desc
            else:
                row[f"attack_{idx}"] = name
                row[f"attack_{idx}_desc"] = desc

        writer.writerow(row)

print(f"✅ Conversion complete! Wrote {len(monsters)} entries to {OUTPUT_CSV}")