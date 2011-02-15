
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usb.h"    /* this is libusb, see http://libusb.sourceforge.net/ */
#include <iostream>
#include <fstream>
#include <windows.h>

#define USBDEV_SHARED_VENDOR    0x16C0  /* VOTI */
#define USBDEV_SHARED_PRODUCT   0x05DC  /* Obdev's free shared PID */

#define USB_ERROR_NOTFOUND  1
#define USB_ERROR_ACCESS    2
#define USB_ERROR_IO        3

static int usbOpenDevice(usb_dev_handle **device, int vendor, char *vendorName, int product, char *productName)
{
    struct usb_bus      *bus;
    struct usb_device   *dev;
    usb_dev_handle      *handle = NULL;
    int                 errorCode = USB_ERROR_NOTFOUND;
    static int          didUsbInit = 0;

    if(!didUsbInit){
        didUsbInit = 1;
        usb_init();
    }
    usb_find_busses();
    usb_find_devices();
    for(bus=usb_get_busses(); bus; bus=bus->next){
        for(dev=bus->devices; dev; dev=dev->next){
            if(dev->descriptor.idVendor == vendor && dev->descriptor.idProduct == product)
            {
                handle = usb_open(dev); /* we need to open the device in order to query strings */
                if(!handle){
                    errorCode = USB_ERROR_ACCESS;
                    fprintf(stderr, "Warning: cannot open USB device: %s\n", usb_strerror());
                    continue;
                }
            }
        }
        if(handle)
            break;
    }
    if(handle != NULL){
        errorCode = 0;
        *device = handle;
    }
    return errorCode;
}


int main(int argc, char **argv)
{
    usb_dev_handle      *handle = NULL;
    unsigned char       buffer[8];
    int                 nBytes;
    usb_init();
    if(usbOpenDevice(&handle, USBDEV_SHARED_VENDOR, 0, USBDEV_SHARED_PRODUCT, 0) != 0){
        fprintf(stderr, "Could not find USB device \"PowerSwitch\" with vid=0x%x pid=0x%x\n", USBDEV_SHARED_VENDOR, USBDEV_SHARED_PRODUCT);
        exit(1);
    }
    /* We have searched all devices on all busses for our USB device above. Now
 * try to open it and perform the vendor specific control operations for the
 * function requested by the user.
 */
DWORD time = GetTickCount();
int count = 1000;
    for(int i=0;i<count;i++)
    {
        nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, 0, 0, 0, (char *)buffer, sizeof(buffer), 5000);
        if(nBytes < 2){
            if(nBytes < 0)
                fprintf(stderr, "USB error: %s\n", usb_strerror());
            fprintf(stderr, "only %d bytes received in iteration %d\n", nBytes, i);
            continue;
        }

        std::cout << (char *)buffer << std::endl;
    }
    time = GetTickCount()-time;
    std::cout << (float)count*8 / time * 1000.f << std::endl;
    std::ofstream file("123.txt");
    file << time;

    printf("test succeeded\n");

    usb_close(handle);
    system("pause");
    return 0;
}
