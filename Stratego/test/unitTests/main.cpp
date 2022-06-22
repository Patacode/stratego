#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <config.h>

using namespace stratego;

int main(int argc, char** argv){
    Config::setDynamicResources(argv[0]);
    int result = Catch::Session().run(argc, argv);
    return result;
}
