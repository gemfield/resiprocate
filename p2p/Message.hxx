#ifndef __P2P_RELOADMESSAGE_HXX
#define __P2P_RELOADMESSAGE_HXX

#include <assert.h>
#include "rutil/Data.hxx"
#include "p2p/Signable.hxx"
#include "p2p/ResourceId.hxx"
#include "p2p/NodeId.hxx"
#include "p2p/MessageStructsGen.hxx"

namespace p2p 
{

class MessageContents;
class ErrorResponse;
class Event;

enum ErrorResponseCode
{
   EMovedTemporarily = 302,
   EUnauthorized = 401,
   EForbidden = 403,
   ENotFound = 404,
   ERequestTimeout = 408,
   EPreconditionFailed = 412,
   EIncompatibleWithOverlay = 498
};

class JoinAns;
class UpdateAns;
class LeaveAns;

class Message : public Signable, private s2c::ForwardingLayerMessageStruct
{
   public:
      // Used to make a request. Will populate the rid into the destination list.  
      Message(ResourceId rid, const resip::Data& overlayName);
      
      virtual ~Message() = 0;

      enum MessageType
      {
         PingReqType = 1,
         PingAnsType = 2,
         ConnectReqType = 3, //
         ConnectAnsType = 4, //
         TunnelReqType = 5,
         TunnelAnsType = 6,
         StoreReqType = 7, //
         StoreAnsType = 8, //
         FetchReqType = 9, //
         FetchAnsType = 10, //
         RemoveReqType = 11,
         RemoveAnsType = 12,
         FindReqType = 13,
         FindAnsType = 14,
         JoinReqType = 15, //
         JoinAnsType = 16, //
         LeaveReqType = 17,
         LeaveAnsType = 18,
         UpdateReqType = 19, //
         UpdateAnsType = 20, //
         RouteQueryReqType = 21,
         RouteQueryAnsType = 22,
         FailureRequestType = 0xFFFE,
         FailureResponseType = 0xFFFF
      };

	  bool isRequest() const;  // convenience function

      virtual MessageType getMessageType() const = 0;

      // copies via list to dest. list in reverse order
      virtual Message *makeErrorResponse(ErrorResponseCode code, 
                                         const resip::Data& reason) const;

	  JoinAns* makeJoinResponse(const resip::Data &overlaySpecific);
	  UpdateAns* makeUpdateResponse();
	  LeaveAns* makeLeaveResponse();
      
	  //virtual JoinResponse *makeJoinResponse();
      
      // encoding/parsing methods
      resip::Data encode() const;	
      static Message *parse(const resip::Data &message, NodeId senderID);

      // Forwarding Header
      UInt8 getTTL() const;
      UInt32 getOverlay() const;
      UInt64 getTransactionID() const;
      UInt16 getFlags() const; 
      //void setFlags(UInt16 flags);
      void pushVia(NodeId node);
      // placeholder for doing via list compression
      
      DestinationId nextDestination() const;
      void popNextDestinationId(); 

      Event* event() 
      {
         assert(0);
         return 0;
      }
      
protected:
	ResourceId mResourceId;
	resip::Data mOverlayName;
	resip::Data mEncodedData;

	virtual void getPayload(resip::Data &data) const = 0;
	virtual std::vector<resip::Data> collectSignableData() const;

	Message() {}
};

class MessageContents 
{
   public:
};

class ErrorResponse : public Message
{
   public:
      virtual MessageType getMessageType() const { return FailureResponseType; }
      
      ErrorResponseCode getErrorCode() const;
      const resip::Data& getReasonPhrase() const;

   protected:
      virtual std::vector<resip::Data> collectSignableData() const;
};

class RouteQueryAnsMessage : public Message
{
public:
	virtual MessageType getMessageType() const { return RouteQueryAnsType; }
};

class RouteQueryReqMessage : public Message
{
   public:
      virtual MessageType getMessageType() const { return RouteQueryReqType; }
};

class RemoveAnsMessage : public Message
{
   public:
      virtual MessageType getMessageType() const { return RemoveAnsType; }
};


class RemoveReqMessage : public Message
{
   public:
      virtual MessageType getMessageType() const { return RemoveReqType; }
};


class TunnelAnsMessage : public Message
{
   public:
      virtual MessageType getMessageType() const { return TunnelAnsType; }
};


class TunnelReqMessage : public Message
{
   public:
      virtual MessageType getMessageType() const { return TunnelReqType; }
};

class PingAnsMessage : public Message
{
   public:
      virtual MessageType getMessageType() const { return PingAnsType; }
};


class PingReqMessage : public Message
{
   public:
      virtual MessageType getMessageType() const { return PingReqType; }
};

class ResourceMessage : public Message
{
   public:
      resip::Data& getResourceId();
      const resip::Data& getResourceId() const;
      void setResourceId(const resip::Data& resourceId);

      resip::Data& getResourceName();
      const resip::Data& getResourceName() const;
      void setResourceName(const resip::Data& resorceName);
      
   private:
      resip::Data mResourceId;
      resip::Data mResourceName;
};

}   

#endif


/* ======================================================================
 *  Copyright (c) 2008, Various contributors to the Resiprocate project
 *  All rights reserved.
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *      - Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 *      - The names of the project's contributors may not be used to
 *        endorse or promote products derived from this software without
 *        specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 *  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *  THE POSSIBILITY OF SUCH DAMAGE.
 *====================================================================== */
