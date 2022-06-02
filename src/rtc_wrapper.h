//
// Created by webrtc on 6/2/22.
//

#ifndef SIMPLE_WEBRTC_CPP_LINUX_RTC_WRAPPER_H
#define SIMPLE_WEBRTC_CPP_LINUX_RTC_WRAPPER_H

#include "rtc_connection.h"
#include "rtc_ice.h"

class Wrapper {
public:
    const std::string name;
    std::unique_ptr<rtc::Thread> network_thread;
    std::unique_ptr<rtc::Thread> worker_thread;
    std::unique_ptr<rtc::Thread> signaling_thread;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory;
    webrtc::PeerConnectionInterface::RTCConfiguration configuration;
    Connection connection;

    Wrapper(const std::string name_) : name(name_), connection(name_) {
    }

    void on_sdp(std::function<void(const std::string &)> f) {
        connection.on_sdp = f;
    }

    void on_accept_ice(std::function<void()> f) {
        connection.on_accept_ice = f;
    }

    void on_ice(std::function<void(const Ice &)> f) {
        connection.on_ice = f;
    }

    void on_success(std::function<void()> f) {
        connection.on_success = f;
    }

    void on_message(std::function<void(const std::string &)> f) {
        connection.on_message = f;
    }

    void init() {
        std::cout << name << ":" << std::this_thread::get_id() << ":"
                  << "Main thread has initiated..." << std::endl;

        // Using Google's STUN server.
        webrtc::PeerConnectionInterface::IceServer ice_server;
        ice_server.uri = "stun:stun.l.google.com:19302";
        configuration.servers.push_back(ice_server);

        network_thread = rtc::Thread::CreateWithSocketServer();
        network_thread->Start();
        worker_thread = rtc::Thread::Create();
        worker_thread->Start();
        signaling_thread = rtc::Thread::Create();
        signaling_thread->Start();
        webrtc::PeerConnectionFactoryDependencies dependencies;
        dependencies.network_thread   = network_thread.get();
        dependencies.worker_thread    = worker_thread.get();
        dependencies.signaling_thread = signaling_thread.get();
        peer_connection_factory       = webrtc::CreateModularPeerConnectionFactory(std::move(dependencies));

        if (peer_connection_factory.get() == nullptr) {
            std::cout << name << ":" << std::this_thread::get_id() << ":"
                      << "Error on CreateModularPeerConnectionFactory." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void create_offer_sdp() {
        std::cout << name << ":" << std::this_thread::get_id() << ":"
                  << "create_offer_sdp" << std::endl;

        connection.peer_connection =
                peer_connection_factory->CreatePeerConnection(configuration, nullptr, nullptr, &connection.pco);

        webrtc::DataChannelInit config;

        // Configuring DataChannel.
        connection.data_channel = connection.peer_connection->CreateDataChannel("data_channel", &config);
        connection.data_channel->RegisterObserver(&connection.dco);

        if (connection.peer_connection.get() == nullptr) {
            peer_connection_factory = nullptr;
            std::cout << name << ":" << std::this_thread::get_id() << ":"
                      << "Error on CreatePeerConnection." << std::endl;
            exit(EXIT_FAILURE);
        }
        connection.peer_connection->CreateOffer(connection.csdo, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
    }

    void create_answer_sdp(const std::string &parameter) {
        std::cout << name << ":" << std::this_thread::get_id() << ":"
                  << "create_answer_sdp" << std::endl;

        connection.peer_connection =
                peer_connection_factory->CreatePeerConnection(configuration, nullptr, nullptr, &connection.pco);

        if (connection.peer_connection.get() == nullptr) {
            peer_connection_factory = nullptr;
            std::cout << name << ":" << std::this_thread::get_id() << ":"
                      << "Error on CreatePeerConnection." << std::endl;
            exit(EXIT_FAILURE);
        }
        webrtc::SdpParseError error;
        webrtc::SessionDescriptionInterface *session_description(
                webrtc::CreateSessionDescription("offer", parameter, &error));
        if (session_description == nullptr) {
            std::cout << name << ":" << std::this_thread::get_id() << ":"
                      << "Error on CreateSessionDescription." << std::endl
                      << error.line << std::endl
                      << error.description << std::endl;
            std::cout << name << ":" << std::this_thread::get_id() << ":"
                      << "Offer SDP:begin" << std::endl
                      << parameter << std::endl
                      << "Offer SDP:end" << std::endl;
            exit(EXIT_FAILURE);
        }
        connection.peer_connection->SetRemoteDescription(connection.ssdo, session_description);
        connection.peer_connection->CreateAnswer(connection.csdo, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
    }

    void push_reply_sdp(const std::string &parameter) {
        std::cout << name << ":" << std::this_thread::get_id() << ":"
                  << "push_reply_sdp" << std::endl;

        webrtc::SdpParseError error;
        webrtc::SessionDescriptionInterface *session_description(
                webrtc::CreateSessionDescription("answer", parameter, &error));
        if (session_description == nullptr) {
            std::cout << name << ":" << std::this_thread::get_id() << ":"
                      << "Error on CreateSessionDescription." << std::endl
                      << error.line << std::endl
                      << error.description << std::endl;
            std::cout << name << ":" << std::this_thread::get_id() << ":"
                      << "Answer SDP:begin" << std::endl
                      << parameter << std::endl
                      << "Answer SDP:end" << std::endl;
            exit(EXIT_FAILURE);
        }
        connection.peer_connection->SetRemoteDescription(connection.ssdo, session_description);
    }

    void push_ice(const Ice &ice_it) {
        std::cout << name << ":" << std::this_thread::get_id() << ":"
                  << "push_ice" << std::endl;

        webrtc::SdpParseError err_sdp;
        webrtc::IceCandidateInterface *ice =
                CreateIceCandidate(ice_it.sdp_mid, ice_it.sdp_mline_index, ice_it.candidate, &err_sdp);
        if (!err_sdp.line.empty() && !err_sdp.description.empty()) {
            std::cout << name << ":" << std::this_thread::get_id() << ":"
                      << "Error on CreateIceCandidate" << std::endl
                      << err_sdp.line << std::endl
                      << err_sdp.description << std::endl;
            exit(EXIT_FAILURE);
        }
        connection.peer_connection->AddIceCandidate(ice);
    }

    void send(const std::string &parameter) {
        std::cout << name << ":" << std::this_thread::get_id() << ":"
                  << "send" << std::endl;

        webrtc::DataBuffer buffer(rtc::CopyOnWriteBuffer(parameter.c_str(), parameter.size()), true);
        std::cout << name << ":" << std::this_thread::get_id() << ":"
                  << "Send(" << connection.data_channel->state() << ")" << std::endl;
        connection.data_channel->Send(buffer);
    }

    void quit() {
        std::cout << name << ":" << std::this_thread::get_id() << ":"
                  << "quit" << std::endl;

        // Close with the thread running.
        connection.peer_connection->Close();
        connection.peer_connection = nullptr;
        connection.data_channel    = nullptr;
        peer_connection_factory    = nullptr;

        network_thread->Stop();
        worker_thread->Stop();
        signaling_thread->Stop();
    }
};



#endif //SIMPLE_WEBRTC_CPP_LINUX_RTC_WRAPPER_H
