#pragma once

#define AWillowHUD_DrawAlwaysOnTopItems_VTABLE 0x17967F8
#define UObject_ProcessEvent_VTABLE 0x1749418
#define UObject_CallFunction_a 0xBDDB0
#define AWillowHUD_DrawAlwaysOnTopItems_a 0x6D0F0C
#define UObject_ProcessEvent_a 0xBE568
#define CellPadGetData_a 0x12314CC
#define ParseLine_a 0x5FDB0
#define ExecuteCommand_a 0x2B607C
#define appBufferToString_a 0x5F96C 
#define _wcsrtombs_a 0x0CA98B8
#define Unhash_name_a 0xCEAFFC
#define Sys_Milliseconds_a 0x115091C
#define appLoadFileFromString_a 0x0061360

#define FOutput_Logf_Ptr 0x17DB3D4

#define AWillowPlayerController_NoFailSetPawnLocation_a 57055
#define AWillowPlayerController_AddGoldenKeysFromSource_a 59478
#define AWillowPlayerController_SetBadassPoints_a 58864
#define UGameViewportClient_GetPlayerOwner_a 20845
#define APawn_GetPawnViewLocation_a 24356
#define APlayerController_ConsoleCommand_a 5500
#define Font_UI_Fonts_font_willowbody_18pt 97850
#define Canvas_DrawBox_a 15375
#define Canvas_DrawRect_a 15371
#define Canvas_DrawText_a 7932
#define Canvas_SetDrawColor_a 7931
#define Canvas_SetPos_a 7935
#define Canvas_StrLen_a 15307

#define GObjects			0x182D0B0;
#define GNames				0x18255BC;

#define BONKERLANDS_DIR "/dev_hdd0/tmp/Bonkerlands/BL2/"

//trophy system init nop 0x5A0EB4

struct FString;
struct FNameEntryHash;	

static symbol<int(void*, void*, void*, void*)> ProcessEvent{ UObject_ProcessEvent_a };
static symbol<void(void*)> AWillowHUD_DrawAlwaysOnTopItems_o{ AWillowHUD_DrawAlwaysOnTopItems_a };

static symbol<unsigned int()> Sys_Milliseconds{ Sys_Milliseconds_a };

static symbol<int(FString*, char*, size_t)> appBufferToString{ appBufferToString_a };
static symbol<void(char*, wchar_t**, int, mbstate_t*)> _wcsrtombs{ _wcsrtombs_a };
static symbol<int(FNameEntryHash* entry, char* name)> Unhash_name{ Unhash_name_a };

static int levelXP[72] = { 0, 358, 1241, 2850, 5376, 8997, 13886, 20208, 28126, 37798, 49377, 63016, 78861, 97061, 117757, 141092, 167206, 196238, 228322, 263595, 302190, 344238, 389873, 439222, 492414, 549578, 610840, 676325, 746158, 820463, 899363, 982980, 1071435, 1164850, 1263343, 1367034, 1476041, 1590483, 1710476, 1836137, 1967582, 2104926, 2248285, 2397772, 2553501, 2715586, 2884139, 3059273, 3241098, 3429728, 3625271, 3827840, 4037543, 4254491, 4478792, 4710556, 4949890, 5196902, 5451701, 5714393, 5985086, 6263885, 6550897, 6846227, 7149982, 7462266, 7783184, 8112840, 8451340, 8798786, 9155282, 9520932 };