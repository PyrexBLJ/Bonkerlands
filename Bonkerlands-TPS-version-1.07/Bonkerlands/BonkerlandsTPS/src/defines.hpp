#pragma once

#define AWillowHUD_DrawAlwaysOnTopItems_VTABLE 0x01739890
#define UObject_ProcessEvent_VTABLE 0x016EE1C0
#define UObject_CallFunction_a 0xB47B0
#define AWillowHUD_DrawAlwaysOnTopItems_a 0x68D984
#define UObject_ProcessEvent_a 0xB7484
#define CellPadGetData_a 0x011B71DC
#define ParseLine_a 0x0005D068
#define ExecuteCommand_a 0x27BA14
#define appBufferToString_a 0x5CC24
#define _wcsrtombs_a 0x0BFE220
#define Unhash_name_a 0xC50578
#define FStringDestructor_a 0xC19EBC
#define appLoadFileFromString_a 0x005E5F4

#define FOutput_Logf_Ptr 0x1780904

#define AWillowPlayerController_NoFailSetPawnLocation_a 55542
#define AWillowPlayerController_AddGoldenKeysFromSource_a 63137
#define AWillowPlayerController_SetBadassPoints_a 62433
#define UGameViewportClient_GetPlayerOwner_a 20783
#define APawn_GetPawnViewLocation_a 24328
#define APlayerController_ConsoleCommand_a 5567
#define Font_UI_Fonts_font_willowbody_18pt 106196
#define Canvas_DrawBox_a 15290
#define Canvas_DrawRect_a 15286
#define Canvas_DrawText_a 8158
#define Canvas_SetDrawColor_a 8157
#define Canvas_SetPos_a 8161

#define GObjects			0x017D3700;
#define GNames				0x017CBDAC;

#define BONKERLANDS_DIR "/dev_hdd0/tmp/Bonkerlands/BLTPS/"

//trophy init nop 0x053FDA0

struct FString;
struct FNameEntryHash;	

static symbol<int(void*, void*, void*, void*)> ProcessEvent{ UObject_ProcessEvent_a };
static symbol<void(void*)> AWillowHUD_DrawAlwaysOnTopItems_o{ AWillowHUD_DrawAlwaysOnTopItems_a };

static symbol<int(FString*, char*, size_t)> appBufferToString{ appBufferToString_a };
static symbol<void(char*, wchar_t**, int, mbstate_t*)> _wcsrtombs{ _wcsrtombs_a };
static symbol<int(FNameEntryHash* entry, char* name)> Unhash_name{ Unhash_name_a };

