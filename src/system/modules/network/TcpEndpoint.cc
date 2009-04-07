/*
 * Copyright (c) 2008 James Molloy, J�rg Pf�hler, Matthew Iselin
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

#include "TcpManager.h"
#include <Log.h>
#include <syscallError.h>

int TcpEndpoint::state()
{
  return static_cast<int>(TcpManager::instance().getState(m_ConnId));
}

Endpoint* TcpEndpoint::accept()
{
  // acquire() will return true when there is at least one release?
  NOTICE("Endpoint::accept: m_IncomingConnectionCount is at " << reinterpret_cast<uintptr_t>(&m_IncomingConnectionCount) << ".");
  m_IncomingConnectionCount.acquire();
  Endpoint* e = m_IncomingConnections.popFront();
  return e;
}

void TcpEndpoint::listen()
{
  NOTICE("Clearing incoming connections");
  m_IncomingConnections.clear();
  NOTICE("Cleared");
  m_ConnId = TcpManager::instance().Listen(this, getLocalPort());
}

bool TcpEndpoint::connect(Endpoint::RemoteEndpoint remoteHost, bool bBlock)
{
  setRemoteHost(remoteHost);
  setRemotePort(remoteHost.remotePort);
  if(getLocalPort() == 0)
  {
    uint16_t port = TcpManager::instance().allocatePort();
    setLocalPort(port);
    if(getLocalPort() == 0)
      return false;
  }
  m_ConnId = TcpManager::instance().Connect(m_RemoteHost, getLocalPort(), this, bBlock, m_Card);
  if(m_ConnId == 0)
    NOTICE("TcpEndpoint::connect: got 0 for the connection id");
  return (m_ConnId != 0); /// \todo Error codes
}

void TcpEndpoint::close()
{
  TcpManager::instance().Disconnect(m_ConnId);
}

int TcpEndpoint::send(size_t nBytes, uintptr_t buffer)
{
  /// \todo Send needs to return an error code or something, and PUSH needs to be an option
  return TcpManager::instance().send(m_ConnId, buffer, true, nBytes);
};

int TcpEndpoint::recv(uintptr_t buffer, size_t maxSize, bool bBlock, bool bPeek)
{
  NOTICE("TcpEndpoint::recv will " << (bBlock ? "" : "not ") << "block");
  
  if((!buffer || !maxSize) && !bPeek)
    return -1;

  bool queueReady = false;
  queueReady = dataReady(bBlock);
  
  if(queueReady)
  {
    // read off the front
    uintptr_t front = m_DataStream.getBuffer();
    
    // how many bytes to read
    size_t nBytes = maxSize;
    if(nBytes > m_DataStream.getSize())
      nBytes = m_DataStream.getSize();

    // if we're merely peeking, return the theoretical maximum number of
    // bytes readable
    if(bPeek)
      return nBytes;
    
    // copy
    memcpy(reinterpret_cast<void*>(buffer), reinterpret_cast<void*>(front), nBytes);
    
    // remove from the buffer, we've read
    m_DataStream.remove(0, nBytes);
    
    // we've read in this block
    return nBytes;
  }
  
  // no data is available - EOF?
  if(TcpManager::instance().getState(m_ConnId) > Tcp::FIN_WAIT_2)
    return 0;
  else
  {
    SYSCALL_ERROR(NoMoreProcesses);
    return -1;
  }
};

void TcpEndpoint::depositPayload(size_t nBytes, uintptr_t payload, uint32_t sequenceNumber, bool push)
{
  /// \note Perhaps nBytes should also have an upper limit check?
  if(!nBytes || !payload)
  {
    NOTICE("Dud arguments to depositPayload!");
    return;
  }
  
  // here goes
  m_ShadowDataStream.insert(payload, nBytes, sequenceNumber - nBytesRemoved, false);
  if(push)
  {
    // take all the data OUT of the shadow stream, shove it into the user stream
    size_t shadowSize = m_ShadowDataStream.getSize();
    m_DataStream.append(m_ShadowDataStream.getBuffer(), shadowSize);
    m_ShadowDataStream.remove(0, shadowSize);
    nBytesRemoved += shadowSize;
  }
}

bool TcpEndpoint::dataReady(bool block, uint32_t tmout)
{
  if(block)
  {
    Semaphore timedOut(0);
    
    Timer* t = Machine::instance().getTimer();
    NetworkBlockTimeout* timeout = new NetworkBlockTimeout;
    if(timeout)
    {
      timeout->setTimeout(tmout);
      timeout->setSemaphore(&timedOut);
      if(t)
        t->registerHandler(timeout);
    }
    
    bool ret = false;
    while(true)
    {
      if(timedOut.tryAcquire())
      {
        // only acquired if we time out!
        break;
      }
      
      if(m_DataStream.getSize() != 0)
      {
        ret = true;
        break;
      }
      
      // if there's no more data in the stream, and we need to close, do it
      // you'd think the above would handle this, but timing is an awful thing to assume
      // much testing has led to the addition of the stream size check
      if(TcpManager::instance().getState(m_ConnId) > Tcp::FIN_WAIT_2 && (m_DataStream.getSize() == 0))
      {
        break;
      }
    }
    
    if(timeout)
    {
      if(t)
        t->unregisterHandler(timeout);
      delete timeout;
    }
    return ret;
  }
  else
  {
    return (m_DataStream.getSize() != 0);
  }
};
