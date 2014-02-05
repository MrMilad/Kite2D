/* 
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or 
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KINPUTSTRUCTS_H
#define KINPUTSTRUCTS_H

#include "Kite/system/ksystemdef.h"

namespace Kite{
namespace Internal{
    struct KEnumInputDevice{
        bool mouse;
        bool keyboard;
        bool joystick;
        U8 joystickCount;
        KEnumInputDevice():
            mouse(false),
            keyboard(false),
            joystick(false),
            joystickCount(0)
        {}
    };
}

    // keyboard state
//    struct KKeyboardInput{
//        const U8 *keys; // pointer to array[256] of keys
//        bool rshift;
//        bool lshift;
//        bool rctrl;
//        bool lctrl;
//        bool ralt;
//        bool lalt;
//        bool isChanged;
//    };

    // standard mouse state (8 button)
//    struct KMouseInput{
//        U32 xPos;
//        U32 yPos;
//        I32 xWinPos;
//        I32 yWinPos;
//        I32 xDelta;
//        I32 yDelta;
//        I32 zDelta; // z-axis, typically a wheel. If the mouse does not have a z-axis, the value is 0.
//        bool rightButton;
//        bool leftButton;
//        bool middleButton;
//        bool isChanged;
//    };

    // complex mouse state (8 button)
//    struct KMouseInput2{
//        I32 xPosition;
//        I32 yPosition;
//        I32 xDelta;
//        I32 yDelta;
//        I32 zDelta; // z-axis, typically a wheel. If the mouse does not have a z-axis, the value is 0.
//        bool rightButton;
//        bool leftButton;
//        bool middleButton;
//        bool button3;
//        bool button4;
//        bool button5;
//        bool button6;
//        bool button7;
//        bool isChanged;
//    };

//    // complex joystick state
//    struct KJoystickInput2{
//        I64 xAxisAnalog; // x-axis, usually the left-right movement of a stick.
//        I64 yAxisAnalog; // y-axis, usually the forward-backward movement of a stick.
//        I64 zAxisAnalog; // z-axis, often the throttle control. If the joystick does not have this axis, the value is 0.
//        I64 rotXAxis; // x-axis rotation. If the joystick does not have this axis, the value is 0.
//        I64 rotYAxis; // Y-axis rotation. If the joystick does not have this axis, the value is 0.
//        I64 rotZAxis; // z-axis rotation (often called the rudder). If the joystick does not have this axis, the value is 0.
//        const I64 *slider; // pointer to array[2]. two additional axes, formerly called the u-axis and v-axis.
//        const DWORD *pov; /* pointer to array[4]. Direction controllers, such as point-of-view hats.
//                        the position is indicated in hundredths of a degree clockwise from north (away from the user).
//                        the center position is normally reported as �1.
//                        For indicators that have only five positions,
//                        the value for a controller is �1, 0, 9,000, 18,000, or 27,000.*/
//        const U8 *buttons; // pointer to array[128]. array of pressed buttons.
//        I64 xAxisVelocity; // x-axis velocity.
//        I64 yAxisVelocity; // y-axis velocity.
//        I64 zAxisVelocity; // z-axis velocity.
//        I64 xAxisAngVelocity; // x-axis angular velocity.
//        I64 yAxisAngVelocity; // y-axis angular velocity.
//        I64 zAxisAngVelocity; // z-axis angular velocity.
//        const I64 *velocitySlider; // pointer to array[2].extra axis velocities.
//        I64 xAxisAcc; // x-axis acceleration.
//        I64 yAxisAcc; // y-axis acceleration.
//        I64 zAxisAcc; // z-axis acceleration.
//        I64 xAxisAngAcc; // x-axis angular acceleration.
//        I64 yAxisAngAcc; // y-axis angular acceleration.
//        I64 zAxisAngAcc; // y-axis angular acceleration.
//        const I64 *accSlider; // pointer to array[2].Extra axis accelerations.
//        I64 xAxisForce; // x-axis force.
//        I64 yAxisForce; // y-axis force.
//        I64 zAxisForce; // z-axis force.
//        I64 xAxistorquee; // x-axis torque.
//        I64 yAxistorquee; // y-axis torque.
//        I64 zAxistorquee; // z-axis torque.
//        const I64 *forceSlider; // pointer to array[2].Extra axis forces.
//        bool isChanged;
//    };

    // standard joystick state
    struct KJoystickInput{
        I64 xAxisAnalog;
        I64 yAxisAnalog;
        I64 zAxisAnalog;
        I64 rotXAxis;
        I64 rotYAxis;
        I64 rotZAxis;
        const IL32 *slider; // pointer to array[2]. two additional axes, formerly called the u-axis and v-axis.
        const DWORD *pov; // pointer to array[4].
        const U8 *buttons; // pointer to array[32].
        bool isChanged;
    };

//    struct KInputDeviceCaps{
//        bool xAxis; // the horizontal axis.(represent the left-right motion of a mouse or joystick)
//        bool yAxis; // the vertical axis.(represent the forward-backward motion of a mouse or joystick)
//        bool zAxis; // the z-axis.(represent the rotation of a mouse wheel or the movement of a joystick throttle control)
//        bool rotXAxis; // rotation around the x-axis.
//        bool rotYAxis; // rotation around the y-axis.
//        bool rotZAxis; // rotation around the z-axis.(often a rudder control)
//        bool slider; // a slider axis.
//        U8 sliderCount;
//        bool button; // a mouse button.
//        U8 buttonCount;
//        bool key; // a keyboard key.
//        U8 keyCount;
//        bool pov; // a point-of-view indicator.
//        U8 povCount;
//        bool unknown; // Unknown.
//    };

//    struct KInputDevice{
//        const KInputDeviceTypes type;
//        bool installed;
//        const U8 id;
//        const char *name;

//        KInputDevice(const KInputDeviceTypes mTypes = KINPUT_DEVICE_UNKNOWN, bool mInstalled = false,
//                     const U8 mId = 0, const char *mName = 0):
//            type(mTypes), installed(mInstalled),
//            id(mId), name(mName)
//        {}
//    };


//    struct KInputState{
//        bool exclusive, complexInput;

//        KInputState(bool Exclusive = false, bool ComplexInput = false):
//            exclusive(Exclusive), complexInput(ComplexInput)
//        {}
//    };

//    typedef KInputState KMouseState;
//    typedef KInputState KJoystickState;

}
#endif // KINPUTSTRUCTS_H
