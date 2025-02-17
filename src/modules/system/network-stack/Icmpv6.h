/*
 * Copyright (c) 2011 Matthew Iselin, Heights College
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
#ifndef MACHINE_ICMPV6_H
#define MACHINE_ICMPV6_H

#include <utilities/String.h>
#include <utilities/Vector.h>
#include <processor/types.h>
#include <process/Semaphore.h>
#include <machine/Network.h>

class IpBase;

/**
 * The Pedigree network stack - ICMPv6 layer
 */
class Icmpv6
{
    public:
        Icmpv6();
        virtual ~Icmpv6();

        /** For access to the stack without declaring an instance of it */
        static Icmpv6& instance()
        {
            return icmpInstance;
        }

        /** Packet arrival callback */
        void receive(IpAddress from, IpAddress to, uintptr_t packet, size_t nBytes, IpBase *pIp, Network* pCard);

        /** Sends an ICMP packet */
        static void send(IpAddress dest, IpAddress from, uint8_t type, uint8_t code, uintptr_t payload, size_t nBytes, Network *pCard = 0);

    private:

        static Icmpv6 icmpInstance;

        struct icmpv6Header
        {
            uint8_t type;
            uint8_t code;
            uint16_t checksum;
        } __attribute__ ((packed));
};

#endif
