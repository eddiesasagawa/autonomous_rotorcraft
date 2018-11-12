#include "arc_host_interface.hh"
#include <string>

namespace arc { namespace common {

// ArcHostInterface& ArcHostInterface::GetInstance() {
//     throw bad_function_call("Cannot call base class GetInstance");
//     return ArcHostInterface();
// }

ArcHostInterface::ArcHostInterface() 
    :   context_(1),
        is_initialized_(false) {
    /* Do other initialization tasks here */
}

}} // arc::common
