#include "Kite/system/ksystemdef.h"
#include "Kite/window/kwindowutil.h"
#include <algorithm>
#include <windows.h>

namespace Kite{
    const KEnumDisplayList *getFullscreenStates(){
        DEVMODE dm;
        static KEnumDisplayList m_disList;
        int index = 0;
        int aspRat;

        // initialize the DEVMODE structure
        ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);

        // clear old list
        m_disList.clear();

        while (EnumDisplaySettingsW(NULL, index++, &dm) != 0){
            KEnumDisplay mode;
            mode.width = dm.dmPelsWidth;
            mode.height = dm.dmPelsHeight;

            // retrieve aspect ratio
            aspRat = (int)(((float)dm.dmPelsWidth / (float)dm.dmPelsHeight) * 10);
            if (aspRat == 13){
                mode.aspectRatio = KAR_4X3;
            }else if (aspRat == 16){
                mode.aspectRatio = KAR_16X10;
            }else if (aspRat == 17){
                mode.aspectRatio = KAR_16X9;
            }else{
                mode.aspectRatio = KAR_UNKNOWN;
            }

            // retrieve color depth
            switch(dm.dmBitsPerPel){
            case 4:
                mode.colorDepth = KCD_4;
                break;
            case 8:
                mode.colorDepth = KCD_8;
                break;
            case 16:
                mode.colorDepth = KCD_16;
                break;
            case 24:
                mode.colorDepth = KCD_24;
                break;
            case 32:
                mode.colorDepth = KCD_32;
                break;
            default:
                mode.colorDepth = KCD_OUT;
                break;
            }
            m_disList.push_back(mode);
        }
        return &m_disList;
    }

    const KEnumDisplay *getDesktopState(){
        DEVMODE dm;
        static KEnumDisplay m_desktop;
        int aspRat;

        // initialize the DEVMODE structure
        ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);

        EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

        m_desktop.width = dm.dmPelsWidth;
        m_desktop.height = dm.dmPelsHeight;

        // retrieve aspect ratio
        aspRat = (int)(((float)dm.dmPelsWidth / (float)dm.dmPelsHeight) * 10);
        if (aspRat == 13){
            m_desktop.aspectRatio = KAR_4X3;
        }else if (aspRat == 16){
            m_desktop.aspectRatio = KAR_16X10;
        }else if (aspRat == 17){
            m_desktop.aspectRatio = KAR_16X9;
        }else{
            m_desktop.aspectRatio = KAR_UNKNOWN;
        }

        // retrieve color depth
        switch(dm.dmBitsPerPel){
        case 4:
            m_desktop.colorDepth = KCD_4;
            break;
        case 8:
            m_desktop.colorDepth = KCD_8;
            break;
        case 16:
            m_desktop.colorDepth = KCD_16;
            break;
        case 24:
            m_desktop.colorDepth = KCD_24;
            break;
        case 32:
            m_desktop.colorDepth = KCD_32;
            break;
        default:
            m_desktop.colorDepth = KCD_OUT;
            break;
        }
        return &m_desktop;
    }

    bool isValidDisplay(const KEnumDisplayList &DisplayList, const KEnumDisplay &Display){
        if (std::find(DisplayList.begin(), DisplayList.end(), Display) != DisplayList.end())
            return true;
        return false;
    }

    void convertEnumToWin(const KEnumDisplay &EnumDisplay, KWindowState &WindowState){
        WindowState.width = EnumDisplay.width;
        WindowState.height = EnumDisplay.height;
        WindowState.colorDepth = EnumDisplay.colorDepth;
    }

}
