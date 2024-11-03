/// @file
///	@copyright	Copyright 2024 VERTIGO. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <cassert>
#include "c74_min.h"
#include "../shared/signal_routing_objects.h"
#include "c74_min_api.h"
#include "netherdream.h"

//#include "etherdream.h"

//struct EAD_Pnt_s dot[DOT_POINTS];

#ifndef M_PI // M_PI not defined on Windows
    #define M_PI 3.14159265358979323846
#endif

#define CIRCLE_POINTS						960
struct EAD_Pnt_s circleA[CIRCLE_POINTS];
struct EAD_Pnt_s circleB[CIRCLE_POINTS];
struct EAD_Pnt_s circleC[CIRCLE_POINTS];
struct EAD_Pnt_s circleD[CIRCLE_POINTS];
struct EAD_Pnt_s circleE[CIRCLE_POINTS];


class etherdream : public signal_routing_base<etherdream>, public sample_operator<26, 1> {

public:
	MIN_DESCRIPTION {"ETHERDREAM"};
	MIN_TAGS {"audio, routing"};
	MIN_AUTHOR {"Cycling '74"};
	MIN_RELATED {"panner~, matrix~"};

	inlet<>  X1{this, "(signal) X1"};
	inlet<>  Y1{this, "(signal) Y1"};
	inlet<>  R1{ this, "(signal) R1"};
	inlet<>  G1{ this, "(signal) G1" };
	inlet<>  B1{ this, "(signal) B1" };
    inlet<>  X2{ this, "(signal) X2" };
    inlet<>  Y2{ this, "(signal) Y2" };
    inlet<>  R2{ this, "(signal) R2" };
    inlet<>  G2{ this, "(signal) G2" };
    inlet<>  B2{ this, "(signal) B2" };
    inlet<>  X3{ this, "(signal) X3" };
    inlet<>  Y3{ this, "(signal) Y3" };
    inlet<>  R3{ this, "(signal) R3" };
    inlet<>  G3{ this, "(signal) G3" };
    inlet<>  B3{ this, "(signal) B3" };
    inlet<>  X4{ this, "(signal) X4" };
    inlet<>  Y4{ this, "(signal) Y4" };
    inlet<>  R4{ this, "(signal) R4" };
    inlet<>  G4{ this, "(signal) G4" };
    inlet<>  B4{ this, "(signal) B4" };
    inlet<>  X5{ this, "(signal) X5" };
    inlet<>  Y5{ this, "(signal) Y5" };
    inlet<>  R5{ this, "(signal) R5" };
    inlet<>  G5{ this, "(signal) G5" };
    inlet<>  B5{ this, "(signal) B5" };
    inlet<>  S{ this, "(signal) SYNC" };
	outlet<> out1{this, "(anything) dac names"};
    outlet<> out2{ this, "(anything) failures" };
    attribute<int> ndacA{ this, "devicenrA", -1};
    attribute<int> ndacB{ this, "devicenrB", -1 };
    attribute<int> ndacC{ this, "devicenrC", -1 };
    attribute<int> ndacD{ this, "devicenrD", -1 };
    attribute<int> ndacE{ this, "devicenrE", -1 };
    //attribute<int> dSample{ this, "downsample", 1 };

	void addA(sample x, sample y, sample r, sample g, sample b);
    void addB(sample x, sample y, sample r, sample g, sample b);
    void addC(sample x, sample y, sample r, sample g, sample b);
    void addD(sample x, sample y, sample r, sample g, sample b);
    void addE(sample x, sample y, sample r, sample g, sample b);
	void sendFrame();
        
    message<> setup{ this, "setup",
        MIN_FUNCTION {
            if (!netherdream::LoadEtherDreamLib()) {
                cout << "cant start Etherdream" << endl;
            }
            else {
                if (netherdream::Close());
                for (uint8_t i = 0; i < netherdream::GetNumberOfDevices(); i++) {
                    out1.send(netherdream::GetDeviceName(i));
                    if (netherdream::Stop(i));
                    if (netherdream::CloseDevice(i));
                }
            }
            return {};
        }
    };

    message<> getdevices{ this, "getdevices", "get etherdreams",
    MIN_FUNCTION {
        if (netherdream::Close());
        for (uint8_t i = 0; i < netherdream::GetNumberOfDevices(); i++) {
                out1.send(netherdream::GetDeviceName(i));
                if (netherdream::CloseDevice(i));
        }
        return {};
        }
    };

    message<> start{ this, "start", "starts etherdream",
        MIN_FUNCTION {
            deviceOpenA = false;
            deviceOpenB = false;
            deviceOpenC = false;
            deviceOpenD = false;
            deviceOpenE = false;
            if (netherdream::GetNumberOfDevices() > 0){
                if (!netherdream::CloseDevice(ndacA)) {
                  out2.send("Etherdream A cant close");
                }
                if (!netherdream::CloseDevice(ndacB)) {
                  out2.send("Etherdream B cant close");
                }
                if (!netherdream::CloseDevice(ndacC)) {
                    out2.send("Etherdream C cant close");
                }
                if (!netherdream::CloseDevice(ndacD)) {
                    out2.send("Etherdream D cant close");
                }
                if (!netherdream::CloseDevice(ndacE)) {
                    out2.send("Etherdream E cant close");
                }
                
                if (!netherdream::OpenDevice(ndacA)) {
                  out2.send("Etherdream A cant open");
                }else{
                    deviceOpenA = true;
                    out2.send("Etherdream A started");
                }
                if (ndacB != ndacA) {
                    if (!netherdream::OpenDevice(ndacB)) {
                        out2.send("Etherdream B cant open");
                    }
                    else {
                        deviceOpenB = true;
                        out2.send("Etherdream B started");
                    }
                }
                if (ndacC != ndacA && ndacC != ndacB) {
                    if (!netherdream::OpenDevice(ndacC)) {
                        out2.send("Etherdream C cant open");
                    }
                    else {
                        deviceOpenC = true;
                        out2.send("Etherdream C started");
                    }
                }
                if (ndacD != ndacA && ndacD != ndacB && ndacD != ndacC) {
                    if (!netherdream::OpenDevice(ndacD)) {
                        out2.send("Etherdream D cant open");
                    }
                    else {
                        deviceOpenD = true;
                        out2.send("Etherdream D started");
                    }
                }
                if (ndacE != ndacA && ndacE != ndacB && ndacE != ndacC && ndacE != ndacD) {
                    if (!netherdream::OpenDevice(ndacE)) {
                        out2.send("Etherdream E cant open");
                    }
                    else {
                        deviceOpenE = true;
                        out2.send("Etherdream E started");
                    }
                }
            }
            return {};
        }
    };

    message<> bang{ this, "bang", "Post the greeting.",
        MIN_FUNCTION {
            //cout << netherdream::GetDeviceName(ndac) << endl;    // post to the max console
            out2.send(numberPoints);
        return {};
        }
    };

	samples<1> operator()(sample in1, sample in2, sample in3, sample in4, sample in5, sample in6, sample in7, sample in8, sample in9, sample in10, sample in11, sample in12, sample in13, sample in14, sample in15, sample in16, sample in17, sample in18, sample in19, sample in20, sample in21, sample in22, sample in23, sample in24, sample in25, sample in26) { //bliver kørt 1 gang for hvert input
        if (S.has_signal_connection()) { //THIS FAILS IF NO ETHERNET PRESENT
            addA(in1, in2, in3, in4, in5);
            addB(in6, in7, in8, in9, in10);
            addC(in11, in12, in13, in14, in15);
            addD(in16, in17, in18, in19, in20);
            addE(in21, in22, in23, in24, in25);
            if (pointA == 960) {
                sendFrame();
            }
        }
	    return {};
	}
    private:

        uint16_t pointA = 0;
        uint16_t pointB = 0;
        uint16_t pointC = 0;
        uint16_t pointD = 0;
        uint16_t pointE = 0;
        uint8_t downSample = 0;
        uint8_t dSample = 1;
        bool rising = true;
        uint16_t numberPoints = 0;
        bool deviceOpenA = false;
        bool deviceOpenB = false;
        bool deviceOpenC = false;
        bool deviceOpenD = false;
        bool deviceOpenE = false;
};
MIN_EXTERNAL(etherdream);

void etherdream::addA(sample x, sample y, sample r, sample g, sample b) {
    if (pointA < 960) {
        struct EAD_Pnt_s* ptA = &circleA[pointA];
        ptA->X = x * std::numeric_limits<int16_t>::max();
        ptA->Y = y * std::numeric_limits<int16_t>::max();
        ptA->R = r * std::numeric_limits<int16_t>::max();
        ptA->G = g * std::numeric_limits<int16_t>::max();
        ptA->B = b * std::numeric_limits<int16_t>::max();
        ptA->I = std::numeric_limits<int16_t>::max();
        pointA += 1;
    }
}

void etherdream::addB(sample x, sample y, sample r, sample g, sample b) {
    if (pointB < 960) {
        struct EAD_Pnt_s* ptB = &circleB[pointB];
        ptB->X = x * std::numeric_limits<int16_t>::max();
        ptB->Y = y * std::numeric_limits<int16_t>::max();
        ptB->R = r * std::numeric_limits<int16_t>::max();
        ptB->G = g * std::numeric_limits<int16_t>::max();
        ptB->B = b * std::numeric_limits<int16_t>::max();
        ptB->I = std::numeric_limits<int16_t>::max();
        pointB += 1;
    }
}

void etherdream::addC(sample x, sample y, sample r, sample g, sample b) {
    if (pointC < 960) {
        struct EAD_Pnt_s* ptC = &circleC[pointC];
        ptC->X = x * std::numeric_limits<int16_t>::max();
        ptC->Y = y * std::numeric_limits<int16_t>::max();
        ptC->R = r * std::numeric_limits<int16_t>::max();
        ptC->G = g * std::numeric_limits<int16_t>::max();
        ptC->B = b * std::numeric_limits<int16_t>::max();
        ptC->I = std::numeric_limits<int16_t>::max();
        pointC += 1;
    }
}

void etherdream::addD(sample x, sample y, sample r, sample g, sample b) {
    if (pointD < 960) {
        struct EAD_Pnt_s* ptD = &circleD[pointD];
        ptD->X = x * std::numeric_limits<int16_t>::max();
        ptD->Y = y * std::numeric_limits<int16_t>::max();
        ptD->R = r * std::numeric_limits<int16_t>::max();
        ptD->G = g * std::numeric_limits<int16_t>::max();
        ptD->B = b * std::numeric_limits<int16_t>::max();
        ptD->I = std::numeric_limits<int16_t>::max();
        pointD += 1;
    }
}

void etherdream::addE(sample x, sample y, sample r, sample g, sample b) {
    if (pointE <= 960) {
        struct EAD_Pnt_s* ptE = &circleE[pointE];
        ptE->X = x * std::numeric_limits<int16_t>::max();
        ptE->Y = y * std::numeric_limits<int16_t>::max();
        ptE->R = r * std::numeric_limits<int16_t>::max();
        ptE->G = g * std::numeric_limits<int16_t>::max();
        ptE->B = b * std::numeric_limits<int16_t>::max();
        ptE->I = std::numeric_limits<int16_t>::max();
        pointE += 1;
    }
}

void etherdream::sendFrame() { //netherdream::GetStatus(ndac) == netherdream::Status::Ready
    if (deviceOpenA) {
        if (pointA > 0) {
            if (!netherdream::WriteFrame(ndacA, circleA, sizeof(EAD_Pnt_s) * pointA, 48000, 1)) {
                //out2.send("Etherdream A framefail");
            }
            numberPoints = pointA;
        }
        else {
            numberPoints = 0;
        }
    }

    if (deviceOpenB) {
            if (pointB > 0) {
                bool write = netherdream::WriteFrame(ndacB, circleB, sizeof(EAD_Pnt_s) * pointB, 48000, 1);
            }
    }
    if (deviceOpenC) {
            if (pointC > 0) {
                bool write = netherdream::WriteFrame(ndacC, circleC, sizeof(EAD_Pnt_s) * pointC, 48000, 1);
            }
    }
    if (deviceOpenD) {
            if (pointD > 0) {
                bool write = netherdream::WriteFrame(ndacD, circleD, sizeof(EAD_Pnt_s) * pointD, 48000, 1);
            }
    }
    if (deviceOpenE) {
            if (pointE > 0) {
                bool write = netherdream::WriteFrame(ndacE, circleE, sizeof(EAD_Pnt_s) * pointE, 48000, 1);
            }
    }
    pointA = 0;
    pointB = 0;
    pointC = 0;
    pointD = 0;
    pointE = 0;
}
