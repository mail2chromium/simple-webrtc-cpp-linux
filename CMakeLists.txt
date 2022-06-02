cmake_minimum_required(VERSION 3.20)
project(Simple_WebRtc_Cpp_Linux)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include_directories(

        #################################################################
        #                   >      Project Files
        "webrtc"
        "picojson"

        #################################################################
        #                   >    WebRtc Project Files

        "/home/webrtc/lib_webrtc/src"
        "/home/webrtc/lib_webrtc/src/test"
        "/home/webrtc/lib_webrtc/src/build/out/Debug/obj"
        "/home/webrtc/lib_webrtc/src/third_party/abseil-cpp"
        "/home/webrtc/lib_webrtc/src/third_party/googletest/src/googletest/include"
        "/home/webrtc/lib_webrtc/src/third_party/libyuv/include"
        "/home/webrtc/lib_webrtc/src/third_party/libyuv/source"
        "/home/webrtc/lib_webrtc/src/third_party/jsoncpp/source/include"
        "/home/webrtc/lib_webrtc/src/base"
        "/home/webrtc/lib_webrtc/src/media/base"
        "/home/webrtc/lib_webrtc/src/media/engine"
        "/home/webrtc/lib_webrtc/src/pc"

        #################################################################
        #                   >    External Modules

        "/usr/include/websocketpp"
        "/usr/include/nlohmann"
        "/usr/include/boost"
)

add_definitions(
        -DWEBRTC_POSIX
        -D_ITERATOR_DEBUG_LEVEL=0
        -DUSE_AURA=1
        -D_HAS_EXCEPTIONS=0
        -D__STD_C
        -D_CRT_RAND_S
        -D_CRT_SECURE_NO_DEPRECATE
        -D_SCL_SECURE_NO_DEPRECATE
        -D_ATL_NO_OPENGL
        -DCERT_CHAIN_PARA_HAS_EXTRA_FIELDS
        -DPSAPI_VERSION=2
        -D_SECURE_ATL
        -D__WRL_NO_DEFAULT_LIB__
        -DNOMINMAX
        -D_UNICODE
        -DUNICODE
        -DNTDDI_VERSION=NTDDI_WIN10_RS2
        -DNDEBUG
        -DNVALGRIND
        -DDYNAMIC_ANNOTATIONS_ENABLED=0
        -DWEBRTC_ENABLE_PROTOBUF=0
        -DWEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE
        -DRTC_ENABLE_VP9
        -DHAVE_SCTP
        -DWEBRTC_LIBRARY_IMPL
        -DWEBRTC_NON_STATIC_TRACE_EVENT_HANDLERS=0
        -DABSL_ALLOCATOR_NOTHROW=1
        -DHAVE_SCTP
)

add_executable(Simple_WebRtc_Cpp_Linux

        main.cpp
        webrtc/rtc_connection.h

        webrtc/rtc_wrapper.h webrtc/rtc_ice.h)

target_link_libraries(Simple_WebRtc_Cpp_Linux

        PUBLIC "/home/webrtc/lib_webrtc/src/out/Debug/obj/libwebrtc.a"
        -lpthread
        -lboost_system
        -ldl
)