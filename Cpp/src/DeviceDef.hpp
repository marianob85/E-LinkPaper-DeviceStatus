#define COLORED 1
#define UNCOLORED 0

// Pin definition
#define RST_PIN 13
#define DC_PIN 26
#define CS_PIN 7
#define BUSY_PIN 6

#define CHANNEL 1

struct DeviceAddress
{
	const char* ip;
	const char* name;
};

static const DeviceAddress deviceAddress[] = {
	{ "192.168.1.1", "M,MT-Gateway" },	{ "192.168.1.2", "M:MT-WifiAP" },		  { "192.168.1.3", "M:MT-LTE" },
	{ "192.168.1.101", "M:HTPC" },		  { "192.168.1.110", "M:Server-Debian" }, { "192.168.1.152", "M:STB-DM800" },
	{ "192.168.10.1", "O:MT-Gateway" },   { "192.168.10.2", "O:MT-AP1" },		  { "192.168.10.3", "O:MT-AP2" },
	{ "192.168.10.4", "O:MT-AP3" },		  { "192.168.10.160", "O:STB-Zgemma" },   { "192.168.10.172", "O:MotionEyeOS" },
	{ "192.168.10.174", "O:NanoPiNeo2" }, { "192.168.13.1", "O:Huawei-HiLink" },  { "192.168.20.1", "P:MT-Gateway-P" }
};