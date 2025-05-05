#include <iostream>
#include "TransportManager.hpp"
#include "x86_bsp/ArgParser.hpp"
#include "x86_bsp/3rdparty/nlohmann/json.hpp"
#include <thread>
#include <chrono>

using json = nlohmann::json;

namespace sensor_transports
{
namespace transporter
{

int run_transport_main_server(const std::string& json_file)
{
    json json_data = json::parse(std::ifstream(json_file));
    auto rig_object = json_data["rig"];
    auto sensors_array = rig_object["sensors"];

    TransportManager transport_manager(sensors_array);

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        transport_manager.runLoop();
    }

    return 0;
}

} // namespace transporter
} // namespace sensor_transports

int main(int argc, char *argv[])
{
    ArgParser parser("TransportMainServer");
    parser.addOption("--rig", "json_file.json", "path to the json file");
    parser.parseArgs(argc, argv);

    std::string json_file;
    parser.getOptionVal("--rig", json_file);

    try
    {
        sensor_transports::transporter::run_transport_main_server(json_file);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}