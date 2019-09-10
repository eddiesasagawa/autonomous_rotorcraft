#ifndef ARC_MEASUREMENTS_HH
#define ARC_MEASUREMENTS_HH

#include "Eigen/Dense"

namespace arc {
  namespace arc {
    namespace bsp {

/**
 * Defines the IMU measurements for carrying around
 */
class ImuMeasurements {
 public:
  double t_valid;
  float temperature;
  Eigen::Vector3f f_im_m;
  Eigen::Vector3f omega_im_m;
};

    } // arc::arc::bsp
  }
}
#endif
