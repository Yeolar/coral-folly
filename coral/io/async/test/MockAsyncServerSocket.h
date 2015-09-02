/*
 * Copyright 2015 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <gmock/gmock.h>

#include <coral/io/async/AsyncServerSocket.h>
#include <coral/io/async/EventBase.h>

namespace coral {

namespace test {

class MockAsyncServerSocket : public AsyncServerSocket {
public:
  typedef std::unique_ptr<MockAsyncServerSocket, Destructor> UniquePtr;

  // We explicitly do not mock destroy(), since the base class implementation
  // in DelayedDestruction is what actually deletes the object.
  //MOCK_METHOD0(destroy,
  //             void());
  MOCK_METHOD1(bind,
               void(const coral::SocketAddress& address));
  MOCK_METHOD2(bind,
               void(const std::vector<coral::IPAddress>& ipAddresses,
                    uint16_t port));
  MOCK_METHOD1(bind,
               void(uint16_t port));
  MOCK_METHOD1(listen,
               void(int backlog));
  MOCK_METHOD0(startAccepting,
               void());
  MOCK_METHOD3(addAcceptCallback,
               void(AcceptCallback *callback,
                    EventBase *eventBase,
                    uint32_t maxAtOnce));
};

}}
