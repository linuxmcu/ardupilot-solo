#ifndef __AP_AHRS_NAVEKF_H__
#define __AP_AHRS_NAVEKF_H__
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *  NavEKF based AHRS (Attitude Heading Reference System) interface for
 *  ArduPilot
 *
 */

#include <AP_AHRS.h>

#if HAL_CPU_CLASS >= HAL_CPU_CLASS_150
#include <AP_NavEKF.h>

#define AP_AHRS_NAVEKF_AVAILABLE 1

class AP_AHRS_NavEKF : public AP_AHRS_DCM
{
public:
    // Constructor
    AP_AHRS_NavEKF(AP_InertialSensor &ins, GPS *&gps, AP_Baro &baro) :
        AP_AHRS_DCM(ins, gps),
        EKF(this, baro),
        _baro(baro)
        {
        }

    // return the smoothed gyro vector corrected for drift
    const Vector3f get_gyro(void) const;
    const Matrix3f &get_dcm_matrix(void) const;

    // return the current drift correction integrator value
    const Vector3f &get_gyro_drift(void) const;

    void            update(void);
    void            reset(bool recover_eulers = false);

    // reset the current attitude, used on new IMU calibration
    void reset_attitude(const float &roll, const float &pitch, const float &yaw);

    // dead-reckoning support
    bool get_position(struct Location &loc);

    // status reporting of estimated error
    float           get_error_rp(void);
    float           get_error_yaw(void);

    // return a wind estimation vector, in m/s
    Vector3f wind_estimate(void);

    // return an airspeed estimate if available. return true
    // if we have an estimate
    bool airspeed_estimate(float *airspeed_ret);

    // true if compass is being used
    bool use_compass(void);

    const NavEKF &get_NavEKF(void) const { return EKF; }

private:
    NavEKF EKF;
    AP_Baro &_baro;
};
#endif

#endif // __AP_AHRS_NAVEKF_H__
