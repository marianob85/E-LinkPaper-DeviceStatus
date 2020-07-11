#include "bluetooth.hpp"

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void btSetDiscoverable()
{
	int ctl = socket( AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI );
	// Open HCI socket
	if( ctl < 0 )
		cerr << "Can't set scan mode" << endl;

	struct hci_dev_req dr;
	dr.dev_id  = 0; // hci0
	dr.dev_opt = SCAN_DISABLED;
	dr.dev_opt = SCAN_PAGE | SCAN_INQUIRY;

	if( ioctl( ctl, HCISETSCAN, ( unsigned long )&dr ) < 0 )
		cerr << "Can't set scan mode" << endl;
	else
		cout << "hci0 is now discoverable" << endl;

	close( ctl );
}
