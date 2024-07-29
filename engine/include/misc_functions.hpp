#ifndef Q_MISC_FUNCTIONS
#define Q_MISC_FUNCTIONS

#include <string>
#include <iomanip>
#include <sstream>

namespace qrk::misc {
template<typename num_t>
std::string to_string_precision(num_t value, int precision) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}
}

#endif