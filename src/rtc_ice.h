//
// Created by webrtc on 6/2/22.
//

#ifndef SIMPLE_WEBRTC_CPP_LINUX_RTC_ICE_H
#define SIMPLE_WEBRTC_CPP_LINUX_RTC_ICE_H

struct Ice {
    std::string candidate;
    std::string sdp_mid;
    int sdp_mline_index;
};

#endif //SIMPLE_WEBRTC_CPP_LINUX_RTC_ICE_H
