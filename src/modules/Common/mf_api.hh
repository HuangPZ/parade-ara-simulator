
/*
    Copyright (C) 1999-2008 by Mark D. Hill and David A. Wood for the
    Wisconsin Multifacet Project.  Contact: gems@cs.wisc.edu
    http://www.cs.wisc.edu/gems/

    --------------------------------------------------------------------

    This file is part of the Ruby Multiprocessor Memory System Simulator,
    a component of the Multifacet GEMS (General Execution-driven
    Multiprocessor Simulator) software toolset originally developed at
    the University of Wisconsin-Madison.

    Ruby was originally developed primarily by Milo Martin and Daniel
    Sorin with contributions from Ross Dickson, Carl Mauer, and Manoj
    Plakal.

    Substantial further development of Multifacet GEMS at the
    University of Wisconsin was performed by Alaa Alameldeen, Brad
    Beckmann, Jayaram Bobba, Ross Dickson, Dan Gibson, Pacia Harper,
    Derek Hower, Milo Martin, Michael Marty, Carl Mauer, Michelle Moravan,
    Kevin Moore, Andrew Phelps, Manoj Plakal, Daniel Sorin, Haris Volos,
    Min Xu, and Luke Yen.
    --------------------------------------------------------------------

    If your use of this software contributes to a published paper, we
    request that you (1) cite our summary paper that appears on our
    website (http://www.cs.wisc.edu/gems/) and (2) e-mail a citation
    for your published paper to gems@cs.wisc.edu.

    If you redistribute derivatives of this software, we request that
    you notify us and either (1) ask people to register with us at our
    website (http://www.cs.wisc.edu/gems/) or (2) collect registration
    information and periodically send it to us.

    --------------------------------------------------------------------

    Multifacet GEMS is free software; you can redistribute it and/or
    modify it under the terms of version 2 of the GNU General Public
    License as published by the Free Software Foundation.

    Multifacet GEMS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Multifacet GEMS; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307, USA

    The GNU General Public License is contained in the file LICENSE.

### END HEADER ###
*/

/*------------------------------------------------------------------------*/
/* Includes                                                               */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/* Macro declarations                                                     */
/*------------------------------------------------------------------------*/

#ifndef _MF_MEMORY_API_H_
#define _MF_MEMORY_API_H_

#define SIM_ENHANCE
#define SIM_NET_PORTS
#define SIM_MEMORY

typedef struct CacheConfig
{
  int valid;
  unsigned numSets;
  unsigned blockSize;
  unsigned associativity;
} CacheConfigStruct;

typedef struct BiNBufferRequest_t
{
  unsigned int numberOfPoints;
  unsigned int* bufferSize;
  unsigned int* bandwidthRequirement;
  unsigned int* cacheImpact;
  int nodeID;
  int bufferID;
} BiNBufferRequest;

void scheduleCB(void (*)(void*), void*, uint64_t);
uint64_t GetSystemTime();

#ifdef SIM_MEMORY
#define SIM_MEMORY_READ 0
#define SIM_MEMORY_WRITE 1
#define SIM_MEMORY_SPM_READ 2
#define SIM_MEMORY_SPM_WRITE 3
#define SIM_MEMORY_BIC_READ 4
#define SIM_MEMORY_BIC_READ_COPY 5
#define SIM_MEMORY_BIC_WRITE 6

typedef struct gem5MemoryInterfaceMemRequest_t
{
  // Indicates that a response to this message is being solicited. Not
  // responding to this message is an error.  0 implies 'no', otherwise 'yes'
  int responseNeeded;

  // A unique ID marking this request
  uint64_t requestID;

  // Time the request was originated, measured in cycles. This is the time that
  // a CPU emitted the request
  uint64_t emitTime;

  // Device source. This is the node ID of the source from enumerated CPUs or
  // accelerators
  int source;
  // Machine type enum work-around
  int sourceType;

  // Device target
  int target;
  int targetType;

  // Program counter
  uint64_t sourcePC;

  // Zero-normalized priority. priority of zero is 'normal', higher is better.
  int priority;

  uint64_t logicalAddr;
  uint64_t physicalAddr;

  uint64_t size;

  int bufferID;
  int bufferSize;

  // Marks the access as a read/write/eviction.
  int type;

  int solicitingDeviceID;

} gem5MemoryInterfaceMemRequest;

typedef struct gem5MemoryInterfaceMemResponse_t
{
  // Relates to an ID associated with MemRequest
  uint64_t requestID;
  int priority;
  int source;
  int sourceType;
  int target;
  int targetType;
  uint64_t logicalAddr;
  uint64_t physicalAddr;
  uint64_t size;
  int bufferID;
  int bufferSize;
  int type;
} gem5MemoryInterfaceMemResponse;

typedef struct mf_simics_memory_interface_api_x
{
  void (*RegisterMemMsgReciever)(void* obj, int controller,
    void (*handler)(const gem5MemoryInterfaceMemRequest* mReq, void* args),
    void* args);

  void (*EmitMemMsgResponse)(void* obj, int controller,
    const gem5MemoryInterfaceMemResponse* mResp, unsigned int delay);

  int (*GetControllerCount)(void* obj);

  uint64_t (*CurrentTime)(void* obj);

} mf_simics_memory_interface_api;

#endif

#endif //_MF_MEMORY_API_H_
