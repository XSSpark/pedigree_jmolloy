/*
 * Copyright (c) 2010 Eduard Burtescu
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <usb/UsbDevice.h>
#include <usb/UsbHub.h>
#include <usb/UsbHubDevice.h>

UsbHubDevice::UsbHubDevice(UsbDevice *dev) : Device(dev), UsbDevice(dev)
{
    for(size_t i=0;i<8;i++)
    {
        uint16_t portStatus[2];
        control(0xa3, 0, 0, i+1, 4, reinterpret_cast<uintptr_t>(&portStatus));
        if(portStatus[0] & 1)
        {
            control(0x23, 3, 4, i+1);
            deviceConnected(i);
        }
    }
}

UsbHubDevice::~UsbHubDevice()
{
}

void UsbHubDevice::doAsync(uint8_t nAddress, uint8_t nEndpoint, uint8_t nPid, uintptr_t pBuffer, uint16_t nBytes, void (*pCallback)(uintptr_t, ssize_t), uintptr_t pParam)
{
    dynamic_cast<UsbHub*>(m_pParent)->doAsync(nAddress, nEndpoint, nPid, pBuffer, nBytes, pCallback, pParam);
}

void UsbHubDevice::addInterruptInHandler(uint8_t nAddress, uint8_t nEndpoint, uintptr_t pBuffer, uint16_t nBytes, void (*pCallback)(uintptr_t, ssize_t), uintptr_t pParam)
{
    dynamic_cast<UsbHub*>(m_pParent)->addInterruptInHandler(nAddress, nEndpoint, pBuffer, nBytes, pCallback, pParam);
}