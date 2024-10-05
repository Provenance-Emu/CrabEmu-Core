/*
 Copyright (c) 2009, OpenEmu Team
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the OpenEmu Team nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY OpenEmu Team ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL OpenEmu Team BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

@import Foundation;
@import libcrabemu;
@import PVCoreObjCBridge;

@protocol ObjCBridgedCoreBridge;
@protocol GameWithCheat;
@protocol PVMasterSystemSystemResponderClient;
@protocol PVSG1000SystemResponderClient;
@protocol PVColecoVisionSystemResponderClient;

typedef NS_ENUM(NSInteger, CrabEMUSystemType) {
    None = CONSOLE_NULL,
    SMS = CONSOLE_SMS,
    GameGear = CONSOLE_GG,
    SG1000 = CONSOLE_SG1000,
    SC3000 = CONSOLE_SC3000,
    ColecoVision = CONSOLE_COLECOVISION,
    NES = CONSOLE_NES,
    Chip8 = CONSOLE_CHIP8
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything" // Silence "Cannot find protocol definition" warning due to forward declaration.
@interface PVCrabEmuBridge: PVCoreObjCBridge <ObjCBridgedCoreBridge, PVMasterSystemSystemResponderClient, PVSG1000SystemResponderClient, PVColecoVisionSystemResponderClient>
#pragma clang diagnostic pop
{
    @public
    NSMutableDictionary *cheatList;
}

@property (nonatomic, assign) CrabEMUSystemType systemType;
- (double)sampleRate;

@end

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything" // Silence "Cannot find protocol definition" warning due to forward declaration.
@interface PVCrabEmuBridge (GameWithCheat) <GameWithCheat>
#pragma clang diagnostic pop
- (void)setCheat:(NSString *)code setType:(NSString *)type setEnabled:(BOOL)enabled;
@end
