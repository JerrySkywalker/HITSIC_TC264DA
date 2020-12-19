/**
 * \file Ifx_LutAtan2F32.c
 * \brief ATAN2 lookup functions with fixed-point angle data type
 *
 *
 * \version disabled
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees. If and as long as no such terms
 * of use are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */
#include "Ifx_LutAtan2F32.h"

#include <math.h>

void Ifx_LutAtan2F32_init(void)
{
#if IFX_LUT_TABLE_CONST == 0
    {   /* generate arctan table */
        sint32  k;
        float32 Step, value;
        Step = (double)IFX_LUT_ATAN_LIMIT_LOW / IFX_LUT_ATAN_SIZE_LOW;

        for (k = 0; k < IFX_LUTATAN2F32_SIZE; k++)
        {
            Ifx_g_LutAtan2F32_FxpAngle_table[k] = IFX_LUT_F32_TO_FXPANGLE(atanf(k / Step));
        }
    }
#endif
}


IFX_INLINE Ifx_Lut_FxpAngle Ifx_LutAtan2F32_fxpAnglePrivate(float32 valf)
{
    uint32 vali;
    valf = IFX_LUTATAN2F32_SIZE * valf;
    vali = (uint32)valf;

    if ((valf - (float32)vali) > 0.5)
    {   /* Round to nearest */
        vali++;
    }

    return Ifx_g_LutAtan2F32_FxpAngle_table[vali];
}


IFX_INLINE float32 Ifx_LutAtan2F32_float32Private(float32 val)
{   /* FIXME round to nearest ?? */
    return Ifx_g_LutAtan2F32_table[(int)(val * IFX_LUTATAN2F32_SIZE)];
}


Ifx_Lut_FxpAngle Ifx_LutAtan2F32_fxpAngle(float32 x, float32 y)
{
    Ifx_Lut_FxpAngle angle;
    float32          fx = (float32)x;
    float32          fy = (float32)y;

    if (y < 0)
    {
        if (x < 0)
        {
            if (-y < -x)
            {   // 180 .. 225 deg
                angle = -IFX_LUT_ANGLE_PI + Ifx_LutAtan2F32_fxpAnglePrivate(-fy / -fx);
            }
            else
            {   // 225 .. 270 deg
                angle = -(IFX_LUT_ANGLE_PI / 2) - Ifx_LutAtan2F32_fxpAnglePrivate(-fx / -fy);
            }
        }
        else // x >= 0
        {
            if (-y < x)
            { // 315 .. 360 deg
                angle = -Ifx_LutAtan2F32_fxpAnglePrivate(-fy / fx); // ok
            }
            else
            {                                                         // 225 .. 270 deg
                angle = -(IFX_LUT_ANGLE_PI / 2) + Ifx_LutAtan2F32_fxpAnglePrivate(fx / -fy);
            }
        }
    }
    else // y >= 0
    {
        if (x < 0)
        {
            if (y < -x)
            {                                                                                 // 135 .. 180 deg
                angle = IFX_LUT_ANGLE_PI - Ifx_LutAtan2F32_fxpAnglePrivate(fy / -fx);       // ok
            }
            else
            {                                                                                 // 90 .. 135 deg
                angle = (IFX_LUT_ANGLE_PI / 2) + Ifx_LutAtan2F32_fxpAnglePrivate(-fx / fy); // ok
            }
        }
        else // x >= 0
        {
            if (y < x)
            {                                                                                // 0 .. 45 deg
                angle = Ifx_LutAtan2F32_fxpAnglePrivate(fy / fx);                          // ok
            }
            else
            {                                                                                // 45 .. 90 deg
                angle = (IFX_LUT_ANGLE_PI / 2) - Ifx_LutAtan2F32_fxpAnglePrivate(fx / fy); // ok
            }
        }
    }

    return angle & (IFX_LUT_ANGLE_RESOLUTION - 1);
}


float32 Ifx_LutAtan2F32_float32(float32 y, float32 x)
{
    float32 angle;

    if (y < 0)
    {
        if (x < 0)
        {
            if (-y < -x)
            {   // 180 .. 225 deg
                angle = -IFX_PI + Ifx_LutAtan2F32_float32Private(-y / -x);
            }
            else
            {   // 225 .. 270 deg
                angle = -(IFX_PI / 2) - Ifx_LutAtan2F32_float32Private(-x / -y);
            }
        }
        else // x >= 0
        {
            if (-y < x)
            { // 315 .. 360 deg
                angle = -Ifx_LutAtan2F32_float32Private(-y / x); // ok
            }
            else
            {                                                      // 225 .. 270 deg
                angle = -(IFX_PI / 2) + Ifx_LutAtan2F32_float32Private(x / -y);
            }
        }
    }
    else // y >= 0
    {
        if (x < 0)
        {
            if (y < -x)
            {                                                                    // 135 .. 180 deg
                angle = IFX_PI - Ifx_LutAtan2F32_float32Private(y / -x);       // ok
            }
            else
            {                                                                    // 90 .. 135 deg
                angle = (IFX_PI / 2) + Ifx_LutAtan2F32_float32Private(-x / y); // ok
            }
        }
        else // x >= 0
        {
            if (y < x)
            {                                                                   // 0 .. 45 deg
                angle = Ifx_LutAtan2F32_float32Private(y / x);                // ok
            }
            else
            {                                                                   // 45 .. 90 deg
                angle = (IFX_PI / 2) - Ifx_LutAtan2F32_float32Private(x / y); // ok
            }
        }
    }

    return angle;
}
