/* Name: set-led.c
 * Project: custom-class, a basic USB example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-10
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 * This Revision: $Id: set-led.c 692 2008-11-07 15:07:40Z cs $
 */

/*
General Description:
This is the host-side driver for the custom-class example device. It searches
the USB for the LEDControl device and sends the requests understood by this
device.
This program must be linked with libusb on Unix and libusb-win32 on Windows.
See http://libusb.sourceforge.net/ or http://libusb-win32.sourceforge.net/
respectively.
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>        /* this is libusb */
#include "opendevice.h" /* common code moved to separate module */
#include <windows.h>


#include "requests.h"   /* custom request numbers */
#include "usbconfig.h"  /* device's VID/PID and names */
#include "ComPort.h"

int main(int argc, char **argv)
{
    usb_dev_handle      *handle = NULL;
    const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
    char                vendor[] = "Atmel", product[] = "JTAGICE mkII";
    char                buffer[1000];
    int                 cnt, vid, pid;

    usb_init();
    /* compute VID/PID from usbconfig.h so that there is a central source of information */
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    /* The following function is in opendevice.c: */
    if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
    {
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        system("pause");
        exit(1);
    }

    try{

    usb_set_configuration(handle, 1);
    usb_claim_interface(handle, 0);
    int i, count=0, total=0;
    DWORD time = GetTickCount();
    char send[] = "1234567890abcdef";
    char recv[256];
    //ComPort com;
    int res;
    for(i = 0; i < 1000; i++)
    {
        //if(!com.WriteBuffer(send, 16))
        //{
        //    std::cout << "Com write error" << std::endl;
        //}
        std::cout << i  << std::endl;
        res = usb_bulk_read(handle, 0x82, recv, 128, 500);
        if(res < 0)
        {
            std::cout << "bulk = " << res << std::endl << usb_strerror() << std::endl;
        }
        else
        {
            recv[res]=0;
            std::cout << "res = " << res << " \t" << recv << std::endl;
            total+=res;
        }
        //Sleep(100);
    }
    time = GetTickCount() - time;
    std::cout << "transfer compleate:\n";
    std::cout << total << " bytes in " << (float)time/1000.f << "seconds" << std::endl;
    std::cout << (float)total/(float)time*1000.f << "bytes per second" << std::endl;
}
    catch(std::exception &e)
    {
        std::cout << e.what()<< std::endl;
    }
    usb_close(handle);
    system("pause");
    return 0;
}
