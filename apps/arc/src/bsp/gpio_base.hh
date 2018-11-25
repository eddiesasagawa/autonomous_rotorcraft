#ifndef ARC_GPIO_BASE_HH
#define ARC_GPIO_BASE_HH

#include <stdint.hpp>
#include "arc_errors.hh"

namespace arc { namespace arc { namespace bsp {

class GpioBase {
    public:
        GpioBase();
        ~GpioBase();

    protected:
        common::shared_log_ptr_t logger_;
        static const std::string kLogName;
        
        static int16_t ref_count_;
};

}}} 

#endif //ARC_GPIO_BASE_HH