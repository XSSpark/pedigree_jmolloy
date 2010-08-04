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

#ifndef USBHUMANINTERFACEDEVICE_H
#define USBHUMANINTERFACEDEVICE_H

#include <processor/types.h>
#include <usb/UsbDevice.h>

class UsbHumanInterfaceDevice : public UsbDevice
{
    public:
        UsbHumanInterfaceDevice (UsbDevice *dev);
        virtual ~UsbHumanInterfaceDevice();
        
        virtual bool initialiseDevice();

        virtual void getName(String &str)
        {
            str = "USB Human Interface Device";
        }

    private:

        struct HidDescriptor
        {
            inline HidDescriptor(UnknownDescriptor *pDes) :  pDescriptor(static_cast<struct Descriptor*>(pDes->pDescriptor)) {}

            struct Descriptor
            {
                uint8_t nLength;
                uint8_t nType;
                uint16_t nBcdHidRelease;
                uint8_t nCountryCode;
                uint8_t nDescriptors;
                uint8_t nDescriptorType;
                uint16_t nDescriptorLength;
            } PACKED *pDescriptor;
        };

        struct HidReportBlock
        {
            enum
            {
                Ignore,
                Absolute,
                Relative,
                Array
            } type;
            size_t nCount;
            size_t nSize;
            uint32_t nUsagePage;
            uint32_t nUsageBase;
            uint8_t nLogSize;
            Vector<size_t> *pUsages;
        };

         struct HidReport
        {
            size_t nBytes;
            List<HidReportBlock *> pBlockList;
        };

        static void callback(uintptr_t pParam, ssize_t ret);
        void inputHandler();

        HidReport *m_pReport;
        uint8_t *m_pReportBuffer;
        uint8_t *m_pOldReportBuffer;
};

#endif
