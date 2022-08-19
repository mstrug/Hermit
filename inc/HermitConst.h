/*
 ============================================================================
  Name        : HermitConst.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITCONST_H_
#define HERMITCONST_H_

#include <e32def.h>
#include "HermitCompilationSettings.h"


_LIT( KHermitVersion, "1.2.0" );


/**
 * SGE file
 */
_LIT( KHermitSGEFileName, "Hermit_360x640.sge" );
const TInt KHermitSGEFileId = 1;


/**
 * Level files
 */
_LIT( KHermitLevelGroup1FileName, "lvg_1.dat" );
_LIT( KHermitLevelGroup2FileName, "lvg_2.dat" );
_LIT( KHermitLevelGroup3FileName, "lvg_3.dat" );
_LIT( KHermitLevelGroup4FileName, "lvg_4.dat" );
_LIT( KHermitLevelGroup5FileName, "lvg_5.dat" );
_LIT( KHermitLevelGroup6FileName, "lvg_6.dat" );
_LIT( KHermitLevelGroup7FileName, "lvg_7.dat" );
_LIT( KHermitLevelGroup8FileName, "lvg_8.dat" );
_LIT( KHermitLevelGroup9FileName, "lvg_9.dat" );
_LIT( KHermitLevelGroup10FileName, "lvg_10.dat" );
_LIT( KHermitLevelGroup11FileName, "lvg_11.dat" );



/**
 * Textures Files
 */
_LIT( KHermitTexFieldEmptyName, "field_empty.png" );
_LIT( KHermitTexFieldEmptyNameS, "field_empty_s.png" );
_LIT( KHermitTexFieldItemName, "field_item.png" );
_LIT( KHermitTexFieldItemNameS, "field_item_s.png" );
_LIT( KHermitTexFieldNoneName, "field_none.png" );
_LIT( KHermitTexFieldSelectedName, "field_selected.png" );
_LIT( KHermitTexFieldSelectedNameS, "field_selected_s.png" );
_LIT( KHermitTexFieldHintName, "field_hint.png" );
_LIT( KHermitTexFieldHintNameS, "field_hint_s.png" );
_LIT( KHermitTexFieldEndName, "field_end.png" );
_LIT( KHermitTexFieldEndNameS, "field_end_s.png" );
_LIT( KHermitTexFieldEndIndName, "field_endind.png" );
_LIT( KHermitTexFieldEndIndNameS, "field_endind_s.png" );

_LIT( KHermitTexMenuBtnGameName, "btn_game.png" );
_LIT( KHermitTexMenuBtnSettingsName, "btn_settings.png" );
_LIT( KHermitTexMenuBtnTutorialName, "btn_tutorial.png" );
_LIT( KHermitTexMenuBtnExitName, "btn_exit.png" );
_LIT( KHermitTexMenuBtnAboutName, "btn_about.png" );
_LIT( KHermitTexMenuTitleName, "img_title.png" );
_LIT( KHermitTexMenuTitleNameS, "img_title_s.png" );
_LIT( KHermitTexMenuTitleNameSS, "img_title_ss.png" );
_LIT( KHermitTexMenuTitleNameSSS, "img_title_sss.png" );

_LIT( KHermitTexMenuBtnSelectedName, "btn_selected.png" );
_LIT( KHermitTexMenuBtnBackName, "btn_back.png" );
_LIT( KHermitTexMenuBtnYesName, "btn_yes.png" );
//_LIT( KHermitTexMenuBtnNoName, "btn_exit.png" );
_LIT( KHermitTexMenuBtnPressAnimName, "anim_btnpress.png" );

_LIT( KHermitTexMenuBtnMinusName, "btn_minus.png" );
_LIT( KHermitTexMenuBtnPlusName, "btn_plus.png" );
_LIT( KHermitTexMenuBtnResetName, "btn_reset.png" );
_LIT( KHermitTexMenuSettingsSoundName, "set_sound.png" );
_LIT( KHermitTexMenuSettingsMusicName, "set_music.png" );
_LIT( KHermitTexMenuSettingsLevelName, "set_level.png" );
_LIT( KHermitTexMenuSettingsTitleName, "set_title.png" );
_LIT( KHermitTexMenuSettingsColorsName, "set_colors.png" );
_LIT( KHermitTexMenuSettingsColorsSelName, "set_colors_sel.png" );
_LIT( KHermitTexMenuSettingsSelName, "set_sel.png" );

_LIT( KHermitTexMenuBtnPlayName, "btn_play.png" );
_LIT( KHermitTexMenuBtnRightName, "btn_right.png" );
_LIT( KHermitTexMenuGameTitleName, "game_title.png" );

_LIT( KHermitTexMenuBtnUndoName, "btn_undo.png" );

_LIT( KHermitTexMenuTutorialTitleName, "tutorial_title.png" );
_LIT( KHermitTexMenuTutorialIndicator1Name, "tutorial_ind_1.png" );
_LIT( KHermitTexMenuTutorialIndicator2Name, "tutorial_ind_2.png" );
_LIT( KHermitTexMenuTutorialText1Name, "tutorial_txt_1.png" );
_LIT( KHermitTexMenuTutorialText2Name, "tutorial_txt_2.png" );
_LIT( KHermitTexMenuTutorialText3Name, "tutorial_txt_3.png" );

_LIT( KHermitTexMenuAboutLogoName, "about_logo.png" );
_LIT( KHermitTexMenuAboutTextName, "about_text.png" );

_LIT( KHermitTexMenuAckTitleName, "ack_title.png" );

_LIT( KHermitTexFont1Name, "font_1.png" );


/**
 * Textures IDs
 */
const TInt KHermitTexFieldEmptyId = 1;
const TInt KHermitTexFieldItemId = 2;
const TInt KHermitTexFieldNoneId = 3;
const TInt KHermitTexFieldSelectedId = 4;
const TInt KHermitTexFieldHintId = 5;
const TInt KHermitTexFieldEndId = 20;
const TInt KHermitTexFieldEndIndId = 21;

const TInt KHermitTexMenuBtnGameId = 0x101;
const TInt KHermitTexMenuBtnTutorialId = 0x102;
const TInt KHermitTexMenuBtnSettingsId = 0x103;
const TInt KHermitTexMenuBtnExitId = 0x104;
const TInt KHermitTexMenuBtnAboutId = 0x105;
const TInt KHermitTexImageTitleId = 0x106;

const TInt KHermitTexMenuBtnSelectedId = 0x40;
const TInt KHermitTexMenuBtnBackId = 0x41;
const TInt KHermitTexMenuBtnYesId = 0x42;
//const TInt KHermitTexMenuBtnNoId = 0x43;
const TInt KHermitTexMenuBtnNoId = KHermitTexMenuBtnExitId;
const TInt KHermitTexMenuBtnPressAnimId = 0x44;

const TInt KHermitTexMenuBtnMinusId = 0x201;
const TInt KHermitTexMenuBtnPlusId = 0x202;
const TInt KHermitTexMenuBtnResetId = 0x203;
const TInt KHermitTexSettingsSoundId = 0x213;
const TInt KHermitTexSettingsMusicId = 0x214;
const TInt KHermitTexSettingsLevelId = 0x215;
const TInt KHermitTexSettingsTitleId = 0x216;
const TInt KHermitTexSettingsColorsId = 0x217;
const TInt KHermitTexSettingsColorsSelId = 0x218;
const TInt KHermitTexSettingsSelId = 0x219;

const TInt KHermitTexMenuBtnPlayId = 0x301;
const TInt KHermitTexMenuBtnRightId = 0x302;
const TInt KHermitTexGameTitleId = 0x303;

const TInt KHermitTexMenuBtnUndoId = 0x401;

const TInt KHermitTexTutorialTitleId = 0x501;
const TInt KHermitTexTutorialIndicator1Id = 0x502;
const TInt KHermitTexTutorialIndicator2Id = 0x503;
const TInt KHermitTexTutorialText1Id = 0x504;
const TInt KHermitTexTutorialText2Id = 0x505;
const TInt KHermitTexTutorialText3Id = 0x506;

const TInt KHermitTexAboutLogoId = 0x601;
const TInt KHermitTexAboutTextId = 0x602;

const TInt KHermitTexAckTitleId = 0x701;

const TInt KHermitTexFont1Id = 0x801;






/**
 * Audio Files
 */
#ifdef AUDIO_FILES_PUBLIC
_LIT( KHermitMusic1FileName, "/Media/Sounds/Music/hermit_music.mp3" );
_LIT( KHermitSoundButtonPressFileName, "/Media/Sounds/Music/Hermit/button.wav" );
_LIT( KHermitSoundButton2PressFileName, "/Media/Sounds/Music/Hermit/button2.wav" );
_LIT( KHermitSoundEndGameWinFileName, "/Media/Sounds/Music/Hermit/end_win.wav" );
_LIT( KHermitSoundEndGameLoseFileName, "/Media/Sounds/Music/Hermit/end_lose.wav" );
_LIT( KHermitSoundFieldPressFileName, "/Media/Sounds/Music/Hermit/field.wav" );
_LIT( KHermitSoundMenuInFileName, "/Media/Sounds/Music/Hermit/menu_in.wav" );
_LIT( KHermitSoundMenuOutFileName, "/Media/Sounds/Music/Hermit/menu_out.wav" );
#else
_LIT( KHermitMusic1FileName, "hermit_music.mp3" );
_LIT( KHermitSoundButtonPressFileName, "button.wav" );
_LIT( KHermitSoundButton2PressFileName, "button2.wav" );
_LIT( KHermitSoundEndGameWinFileName, "end_win.wav" );
_LIT( KHermitSoundEndGameLoseFileName, "end_lose.wav" );
_LIT( KHermitSoundFieldPressFileName, "field.wav" );
_LIT( KHermitSoundMenuInFileName, "menu_in.wav" );
_LIT( KHermitSoundMenuOutFileName, "menu_out.wav" );
#endif

const TUint KHermitMusic1Id = 0x101;
const TUint KHermitSoundButtonPressId = 0x201;
const TUint KHermitSoundButton2PressId = 0x202;
const TUint KHermitSoundEndGameWinId = 0x203;
const TUint KHermitSoundEndGameLoseId = 0x204;
const TUint KHermitSoundFieldPressId = 0x205;
const TUint KHermitSoundMenuInId = 0x206;
const TUint KHermitSoundMenuOutId = 0x207;



#endif // HERMITCONST_H_
