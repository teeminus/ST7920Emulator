/**
 * ST7920Emulator
 * Copyright (C) 2021 Teeminus [https://github.com/teeminus/ST7920Emulator]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>

class St7920Emulator {
    public:
        St7920Emulator(void (*fClearDisplay)(void), void (*fDrawByte)(uint8_t, uint8_t, uint8_t));
        void parseSerialData(uint8_t ui8Data);
        void reset(bool bClearDisplay);

    private:
        enum St7920SyncByteType {
            SBT_None,
            SBT_Command,
            SBT_Data
        };
        enum St7920CommandType {
            CT_None,
            CT_DdramAddress,
            CT_CgramAddress,
            CT_FunctionSet,
            CT_CursorControl,
            CT_DisplayControl,
            CT_EntryMode,
            CT_Home,
            CT_Clear,
            CT_GdramAddress,
            CT_IramAddress,
            CT_Reverse,
            CT_ScrollSelect,
            CT_StandBy
        };
        enum St7920DataTarget {
            DT_None,
            DT_CGRAM,
            DT_DDRAM,
            DT_GDRAM,
        };

        static const uint8_t pByteFlip[256];
        static const uint8_t pFont816[4080];

        void (*fClearDisplay_)(void);
        void (*fDrawByte_)(uint8_t, uint8_t, uint8_t);

        St7920CommandType lastCommand_;
        St7920DataTarget dataTarget_;
        St7920SyncByteType syncByteType_;

        uint8_t ui8DataNibbleIdx_;
        uint8_t ui8DataByte_;

        uint8_t ui8EnableVerticalScroll_;
        uint8_t ui8ExtendedMode_;
        uint8_t ui8GraphicMode_;
        uint8_t ui8AddressX_;
        uint8_t ui8AddressY_;

        uint8_t pCgRam_[64][2];
        uint8_t pDdRam_[4][32];
        uint8_t pGdRam_[64][16];

        void parseCommandByte();
        void parseDataByte();
        uint8_t parseSyncByte(uint8_t ui8Data);
        void showByte(uint8_t x, uint8_t y);
};