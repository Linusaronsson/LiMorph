#include "pch.h"

#include "Logging.h"

namespace morph {
void Logging::Print(const std::string& msg) {
    std::ofstream outfile;
    outfile.open("D:\\Desktop\\debugfile.txt", std::ios_base::app);
    outfile << msg;
}
} // namespace morph