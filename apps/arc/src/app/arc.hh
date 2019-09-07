#ifndef ARC_ARC_HH
#define ARC_ARC_HH

#include<memory>

#include "arc2host_interface.hh"
#include "gnc_autopilot.hh"
#include "sensor.hh"
#include "arc_errors.hh"

namespace arc { namespace arc {

class Arc {
 public:
  /**
   * Constructor
   */
  Arc(
    const char* const my_ip_addr,      /**< [in] IP address that this side runs from */
    const char* const my_port,         /**< [in] Port that this side runs from */
    const char* const dest_ip_addr,     /**< [in] IP address of destination */
    const char* const dest_port        /**< [in] Port of destination */
  );

  /**
   * Destructor
   */
  ~Arc();

  /**
   * Main loop that runs everything
   */
  void Spin();

 private:
  Arc2HostInterface& a2hi_;
  common::shared_log_ptr_t logger_;

  std::unique_ptr<AutopilotBase> autopilot_;

  bsp::ST_6DOFImu_LSM6DS33 imu_;

  static const std::string kLogName;
};

}} // arc::arc

#endif //ARC_ARC_HH