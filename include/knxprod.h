#pragma once

#define paramDelay(time) (uint32_t)( \
            (time & 0xC000) == 0xC000 ? (time & 0x3FFF) * 100 : \
            (time & 0xC000) == 0x0000 ? (time & 0x3FFF) * 1000 : \
            (time & 0xC000) == 0x4000 ? (time & 0x3FFF) * 60000 : \
            (time & 0xC000) == 0x8000 ? ((time & 0x3FFF) > 1000 ? 3600000 : \
                                         (time & 0x3FFF) * 3600000 ) : 0 )
//--------------------Allgemein---------------------------
#define MAIN_OpenKnxId 0xAF
#define MAIN_ApplicationNumber 0x02
#define MAIN_ApplicationVersion 0x02
#define MAIN_OrderNumber "VIS-UP-RoundMini" //may not work with multiple devices on same hardware or app on different hardware
#define MAIN_ParameterSize 80
#define MAIN_MaxKoNumber 4



//---------------------Modules----------------------------

//-----Module specific starts
#define PAGE_ParamBlockOffset 0
#define PAGE_ParamBlockSize 16
#define PAGE_KoOffset 0
#define PAGE_KoBlockSize 0

//-----Module: page
#define PAGE_type		0x0000
#define PAGE_type_Shift	5
#define PAGE_type_Mask	0x0007
// Offset: 0, Size: 3 Bit, Text: Funktion
#define ParamPAGE_typeIndex(X) ((uint)((knx.paramByte((PAGE_ParamBlockOffset + PAGE_ParamBlockSize * X + 0)) >> PAGE_type_Shift) & PAGE_type_Mask))
// Offset: 0, Size: 3 Bit, Text: Funktion
#define ParamPAGE_type ((uint)((knx.paramByte((PAGE_ParamBlockOffset + PAGE_ParamBlockSize * channelIndex() + 0)) >> PAGE_type_Shift) & PAGE_type_Mask))
#define PAGE_display		0x0001
// Offset: 1, Size: 120 Bit (15 Byte), Text: Anzeigename
#define ParamPAGE_displayIndex(X) knx.paramData((PAGE_ParamBlockOffset + PAGE_ParamBlockSize * X + 1))
// Offset: 1, Size: 120 Bit (15 Byte), Text: Anzeigename
#define ParamPAGE_display knx.paramData((PAGE_ParamBlockOffset + PAGE_ParamBlockSize * channelIndex() + 1))

