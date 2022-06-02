
#include <condition_variable>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

// Header files related with WebRTC.
#include <api/create_peerconnection_factory.h>
#include <rtc_base/ssl_adapter.h>
#include <rtc_base/thread.h>
#include <system_wrappers/include/field_trial.h>

#include "rtc_connection.h"
#include "rtc_wrapper.h"

#include "picojson/picojson.h"


int main(int argc, char *argv[]) {
    webrtc::field_trial::InitFieldTrialsFromString("");
    rtc::InitializeSSL();

    Wrapper webrtc("");
    std::list<Ice> ice_list;

    std::string line;
    std::string command;
    std::string parameter;
    std::string sdp_type;
    bool is_cmd_mode = true;

    webrtc.on_ice([&](const Ice &ice) { ice_list.push_back(ice); });
    webrtc.on_message([&](const std::string &message) { std::cout << message << std::endl; });
    webrtc.on_sdp([&](const std::string &sdp) {
        std::cout << sdp_type << " sdp:begin" << std::endl << sdp << sdp_type << " sdp:end" << std::endl;
    });

    webrtc.init();
    std::cout << "> ";
    while (std::getline(std::cin, line)) {
        if (is_cmd_mode) {
            if (line == "") {
                continue;

            } else if (line == "sdp1") {
                sdp_type = "Offer";
                webrtc.create_offer_sdp();
                std::cout << "> ";

            } else if (line == "sdp2") {
                command     = "sdp2";
                is_cmd_mode = false;

            } else if (line == "sdp3") {
                command     = "sdp3";
                is_cmd_mode = false;

            } else if (line == "ice1") {
                picojson::array ice_a;
                for (auto &ice : ice_list) {
                    picojson::object ice_o;
                    ice_o.insert(std::make_pair("candidate", ice.candidate));
                    ice_o.insert(std::make_pair("sdp_mid", ice.sdp_mid));
                    ice_o.insert(std::make_pair("sdp_mline_index", static_cast<double>(ice.sdp_mline_index)));
                    ice_a.push_back(picojson::value(ice_o));
                }
                std::cout << picojson::value(ice_a).serialize(true) << std::endl;
                ice_list.clear();

            } else if (line == "ice2") {
                command     = "ice2";
                is_cmd_mode = false;

            } else if (line == "send") {
                command     = "send";
                is_cmd_mode = false;

            } else if (line == "quit") {
                webrtc.quit();
                break;

            } else {
                std::cout << "> " << line << std::endl;
            }
        } else {
            if (line == ";") {
                if (command == "sdp2") {
                    sdp_type = "Answer";
                    webrtc.create_answer_sdp(parameter);

                } else if (command == "sdp3") {
                    webrtc.push_reply_sdp(parameter);

                } else if (command == "ice2") {
                    picojson::value v;
                    std::string err = picojson::parse(v, parameter);
                    if (!err.empty()) {
                        std::cout << "Error on parse json : " << err << std::endl;
                        parameter = "";
                        continue;
                    }
                    for (auto &it : v.get<picojson::array>()) {
                        picojson::object &ice_o = it.get<picojson::object>();
                        Ice ice;
                        ice.candidate       = ice_o.at("candidate").get<std::string>();
                        ice.sdp_mid         = ice_o.at("sdp_mid").get<std::string>();
                        ice.sdp_mline_index = static_cast<int>(ice_o.at("sdp_mline_index").get<double>());
                        webrtc.push_ice(ice);
                    }
                } else if (command == "send") {
                    webrtc.send(parameter);
                }

                parameter   = "";
                is_cmd_mode = true;
            } else {
                parameter += line + "\n";
            }
        }
    }

    rtc::CleanupSSL();

    return 0;
}
