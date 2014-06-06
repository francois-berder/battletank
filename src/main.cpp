#include <stdexcept>

#include "Options.hpp"
#include "Logger.hpp"


int main(int argc, char **argv)
{
    Options opt;
    try
    {
        opt.parse(argc, argv);
    }
    catch(std::exception &e)
    {
        LOG << "ERROR ! " << e.what() << "\n";
    }
    
    return 0;
}
