# simple-webrtc-cpp-linux

### All in All WebRtc: A Simple WebRtc Program for beginners to implement console based applications in Linux OS/Box

-------


**Getting Started**
------
High performance audio apps typically require more functionality than the simple ability to play or record sound. They demand responsive realtime system behavior. Webrtc provides browsers and *mobile applications* with Real-Time Communications (RTC) capabilities via simple APIs. 

For *Understanding WebRtc Audio Processing and It's different modules*, you should have to look into this reference:

- [Android-Audio-Processing-Using-WebRTC](https://github.com/mail2chromium/Android-Audio-Processing-Using-WebRTC)


For *Compilation and Building the WebRTC Library for Android*, you should have to look into this refernce:

- [Compile_WebRTC_Library_For_Android](https://github.com/mail2chromium/Compile_WebRTC_Library_For_Android)


For *real-time Communication and AudioProcessing* in Android, I will recommend you to must visit these refernces:

- [Android_Realtime_Communication_Using_WebRTC](https://github.com/mail2chromium/Android_Realtime_Communication_Using_WebRTC)
- [Android-Native-Development-For-WebRTC](https://github.com/mail2chromium/Android-Native-Development-For-WebRTC)


-------

The basic motivation behind this project is to run a webrtc based simple application on latest WebRtc Stack. I have compiled fresh WebRtc Library for `Linux OS`.
I have made it very simple for *webrtc newbies* and for *webrtc geeks* to understand and build large projects using the development setup.

**Prerequisites:**

This tutorial assumes that you are on a UNIX-based system, I am on Ubuntu and have tested versions *18.04 LTS* and *20.04 LTS*.

> Note: I recommend WebRtc Linux development on Ubuntu 18.04 LTS


**Basic Usage:**

You can run this program in different IDEs and build tools such as;

- Visual Studio 2019-2022
- Clion Latest version
- Using Cmake Build Tools

**Workaround:**

In order to use this program in your development setup, you have to proceed the following steps;

1. Setup WebRtc Compilation Environment for Linux.

Please Visit: [WebRtc Compilation Setup for Core Linux Development](https://medium.com/@BeingOttoman/simplest-webrtc-debugging-development-environment-for-linux-3d4e4aed54f3)


2. Clone this repo:

```
  git clone https://github.com/mail2chromium/simple-webrtc-cpp-linux.git

```

3. Open Project in your desired IDE;
    - Visual Studio 2019-2022
    - Clion Latest version
    - Using Cmake Build Tools


**Results:**

I have run this program in Clion to properly debug the WebRtc Native Source Code as follows;

1. Debugger Output Screen

![image](https://user-images.githubusercontent.com/42235538/171619877-b57a8e6d-bb9f-42b5-8b68-e8bbb487f102.png)

2. Console Output Screen

![image](https://user-images.githubusercontent.com/42235538/171619997-43e15e19-20cc-436a-9808-8346ed111e81.png)

When you run this program, you will come up with the following list of commands to generate SDPs, and ICE Candidates

```
*** Welcome to WebRtc Debugging Development Environment ***
    > Please follow these commands to use this program < 
    1. sdp1 -> To Generate SDPs for Peer-A 
    2. sdp2 -> To Generate SDPs for Peer-B 
    3. ice1 -> To Generate Ice Candidates for Peer-A 
    4. ice2 -> To Generate Ice Candidates for Peer-B 
    5. ;    -> To terminate RTC Thread Session. 

```

![image](https://user-images.githubusercontent.com/42235538/171621651-f09dd00b-ab14-4f4c-8c2e-672df6f523aa.png)

When you type `sdp1` command, the result will be like this;

```

:140071081162560:Main thread has initiated...
> sdp1
:140071081162560:create_offer_sdp
:140071046182656:PeerConnectionObserver::RenegotiationNeeded
> :140071046182656:CreateSessionDescriptionObserver::OnSuccess
:140071046182656:PeerConnectionObserver::SignalingChange(1)
Offer sdp:begin
v=0
o=- 1381929178758113972 2 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE data
a=msid-semantic: WMS
m=application 9 UDP/DTLS/SCTP webrtc-datachannel
c=IN IP4 0.0.0.0
a=ice-ufrag:Zw7U
a=ice-pwd:fhNYQwc7IWGYYUR2Mrbsi9Qa
a=ice-options:trickle
a=fingerprint:sha-256 10:0B:68:35:3F:20:65:48:0D:67:7E:DF:3F:2A:4F:ED:A1:CE:79:CF:D8:B1:AE:D0:57:59:BB:C6:4A:DD:91:8C
a=setup:actpass
a=mid:data
a=sctp-port:5000
a=max-message-size:262144
Offer sdp:end
:140071046182656:SetSessionDescriptionObserver::OnSuccess
:140071046182656:PeerConnectionObserver::IceGatheringChange(1)
:140071046182656:PeerConnectionObserver::IceCandidate
:140071046182656:PeerConnectionObserver::IceCandidate
:140071046182656:PeerConnectionObserver::IceCandidate
:140071046182656:PeerConnectionObserver::IceGatheringChange(2)

```

When you type `ice1` command, output will be like this;

```

> ice1
[
  {
    "candidate": "candidate:1021689336 1 udp 2122260223 192.168.1.160 44302 typ host generation 0 ufrag Zw7U network-id 3 network-cost 50",
    "sdp_mid": "data",
    "sdp_mline_index": 0
  },
  {
    "candidate": "candidate:1919303944 1 tcp 1518280447 192.168.1.160 53703 typ host tcptype passive generation 0 ufrag Zw7U network-id 3 network-cost 50",
    "sdp_mid": "data",
    "sdp_mline_index": 0
  },
  {
    "candidate": "candidate:3182279500 1 udp 1686052607 110.93.193.154 44302 typ srflx raddr 192.168.1.160 rport 44302 generation 0 ufrag Zw7U network-id 3 network-cost 50",
    "sdp_mid": "data",
    "sdp_mline_index": 0
  }
]

```

**Conculsion:**

I have tried my best to cut this program as simple as possible to give newbies a good example to Kick start webrtc debugging across core linux development. Thanks for your support.
