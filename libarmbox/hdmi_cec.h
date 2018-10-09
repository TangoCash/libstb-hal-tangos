/*
	Copyright (C) 2018 TangoCash

	License: GPLv2

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation;

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <OpenThreads/Thread>
#include <OpenThreads/Condition>

#include "video_lib.h"

struct cec_message
{
	unsigned char initiator;
	unsigned char destination;
	unsigned char opcode;
	unsigned char data[256];
	unsigned char length;
} __attribute__((packed));

struct addressinfo
{
	unsigned char logical;
	unsigned char physical[2];
	unsigned char type;
};

enum {
	KEY_RELEASED = 0,
	KEY_PRESSED,
	KEY_AUTOREPEAT
};

class hdmi_cec : public OpenThreads::Thread
{
private:
	hdmi_cec();
	static hdmi_cec *hdmi_cec_instance;
	void run();
	bool Start();
	bool Stop();
	void Receive();
	unsigned char physicalAddress[2];
	bool autoview_cec_activ;
	unsigned char deviceType, logicalAddress;
	int hdmiFd;
	long translateKey(unsigned char code);
	void handleCode(long code, bool keypressed);
	int rc_send(int fd, unsigned int code, unsigned int value);
	void rc_sync(int fd);
	bool standby;
protected:
	bool running;
public:
	~hdmi_cec();
	static hdmi_cec* getInstance();
	bool SetCECMode(VIDEO_HDMI_CEC_MODE);
	void SetCECAutoView(bool);
	void SetCECAutoStandby(bool);
	void GetCECAddressInfo();
	void SendCECMessage(struct cec_message &message);
	void SetCECState(bool state);
	void ReportPhysicalAddress();
	bool standby_cec_activ;
};