//
// Created by webrtc on 6/2/22.
//

#ifndef SIMPLE_WEBRTC_CPP_LINUX_RTC_CONNECTION_H
#define SIMPLE_WEBRTC_CPP_LINUX_RTC_CONNECTION_H

#include "rtc_ice.h"

class Connection {
public:
    const std::string name;

    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection;
    rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel;

    std::function<void(const std::string &)> on_sdp;
    std::function<void()> on_accept_ice;
    std::function<void(const Ice &)> on_ice;
    std::function<void()> on_success;
    std::function<void(const std::string &)> on_message;

    // When the status of the DataChannel changes, determine if the connection is complete.
    void on_state_change() {
        std::cout << "state: " << data_channel->state() << std::endl;
        if (data_channel->state() == webrtc::DataChannelInterface::kOpen && on_success) {
            on_success();
        }
    }

    // After the SDP is successfully created, it is set as a LocalDescription and displayed as a string to be passed to
    // the other party.
    void on_success_csd(webrtc::SessionDescriptionInterface *desc) {
        peer_connection->SetLocalDescription(ssdo, desc);

        std::string sdp;
        desc->ToString(&sdp);
        on_sdp(sdp);
    }

    // Convert the got ICE.
    void on_ice_candidate(const webrtc::IceCandidateInterface *candidate) {
        Ice ice;
        candidate->ToString(&ice.candidate);
        ice.sdp_mid         = candidate->sdp_mid();
        ice.sdp_mline_index = candidate->sdp_mline_index();
        on_ice(ice);
    }

    class PCO : public webrtc::PeerConnectionObserver {
    private:
        Connection &parent;

    public:
        PCO(Connection &parent) : parent(parent) {
        }

        void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "PeerConnectionObserver::SignalingChange(" << new_state << ")" << std::endl;
        };

        void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "PeerConnectionObserver::AddStream" << std::endl;
        };

        void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "PeerConnectionObserver::RemoveStream" << std::endl;
        };

        void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "PeerConnectionObserver::DataChannel(" << data_channel << ", " << parent.data_channel.get() << ")"
                      << std::endl;
            // The request recipient gets a DataChannel instance in the onDataChannel event.
            parent.data_channel = data_channel;
            parent.data_channel->RegisterObserver(&parent.dco);
        };

        void OnRenegotiationNeeded() override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "PeerConnectionObserver::RenegotiationNeeded" << std::endl;
        };

        void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "PeerConnectionObserver::IceConnectionChange(" << new_state << ")" << std::endl;
        };

        void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "PeerConnectionObserver::IceGatheringChange(" << new_state << ")" << std::endl;
        };

        void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "PeerConnectionObserver::IceCandidate" << std::endl;
            parent.on_ice_candidate(candidate);
        };
    };

    class DCO : public webrtc::DataChannelObserver {
    private:
        Connection &parent;

    public:
        DCO(Connection &parent) : parent(parent) {
        }

        void OnStateChange() override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "DataChannelObserver::StateChange" << std::endl;
            parent.on_state_change();
        };

        // Message receipt.
        void OnMessage(const webrtc::DataBuffer &buffer) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "DataChannelObserver::Message" << std::endl;
            if (parent.on_message) {
                parent.on_message(std::string(buffer.data.data<char>(), buffer.data.size()));
            }
        };

        void OnBufferedAmountChange(uint64_t previous_amount) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "DataChannelObserver::BufferedAmountChange(" << previous_amount << ")" << std::endl;
        };
    };

    class CSDO : public webrtc::CreateSessionDescriptionObserver {
    private:
        Connection &parent;

    public:
        CSDO(Connection &parent) : parent(parent) {
        }

        void OnSuccess(webrtc::SessionDescriptionInterface *desc) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "CreateSessionDescriptionObserver::OnSuccess" << std::endl;
            parent.on_success_csd(desc);
        };

        void OnFailure(webrtc::RTCError error) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "CreateSessionDescriptionObserver::OnFailure" << std::endl
                      << error.message() << std::endl;
        };
    };

    class SSDO : public webrtc::SetSessionDescriptionObserver {
    private:
        Connection &parent;

    public:
        SSDO(Connection &parent) : parent(parent) {
        }

        void OnSuccess() override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "SetSessionDescriptionObserver::OnSuccess" << std::endl;
            if (parent.on_accept_ice) {
                parent.on_accept_ice();
            }
        };

        void OnFailure(webrtc::RTCError error) override {
            std::cout << parent.name << ":" << std::this_thread::get_id() << ":"
                      << "SetSessionDescriptionObserver::OnFailure" << std::endl
                      << error.message() << std::endl;
        };
    };

    PCO pco;
    DCO dco;
    rtc::scoped_refptr<CSDO> csdo;
    rtc::scoped_refptr<SSDO> ssdo;

    Connection(const std::string &name_) :
            name(name_),
            pco(*this),
            dco(*this),
            csdo(new rtc::RefCountedObject<CSDO>(*this)),
            ssdo(new rtc::RefCountedObject<SSDO>(*this)) {
    }
};




#endif //SIMPLE_WEBRTC_CPP_LINUX_RTC_CONNECTION_H
