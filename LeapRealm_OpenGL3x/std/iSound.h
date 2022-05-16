#pragma once

#include "iStd.h"

enum iSound
{
	snd_eff_chest_open,
	snd_eff_chest_close,
	snd_eff_cloth_equip,
	snd_eff_cloth_unequip,
	snd_eff_item_grab,
	snd_eff_item_drop,
	snd_eff_mouse_click,
	snd_eff_mouse_hover,
	snd_eff_shield_equip,
	snd_eff_shield_unequip,
	snd_eff_sword_equip,
	snd_eff_sword_unequip,
	snd_eff_max,

	snd_bgm_main = snd_eff_max,
	snd_max,
};

extern AudioInfo ai[snd_max];
