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

#import "SMSGameCore.h"
#import <OpenEmuBase/OERingBuffer.h>
#import <OpenGL/gl.h>
#import "OESMSSystemResponderClient.h"
#import "OEGGSystemResponderClient.h"
#import "OESG1000SystemResponderClient.h"
#import "OEColecoVisionSystemResponderClient.h"

#include "sms.h"
#include "smsmem.h"
#include "sound.h"
#include "smsvdp.h"
#include "smsz80.h"
#include "rom.h"
#include "colecovision.h"
#include "colecomem.h"
#include "cheats.h"
#include "console.h"

#include "fmemopen/fmemopen.h"
#include "fmemopen/open_memstream.h"

#define SAMPLERATE 44100

@interface SMSGameCore () <OESMSSystemResponderClient, OEGGSystemResponderClient, OESG1000SystemResponderClient, OEColecoVisionSystemResponderClient>
{
    unsigned char *tempBuffer;
    NSLock        *bufLock;
    BOOL           paused;
    NSURL         *romFile;
    NSMutableDictionary *cheatList;
}
- (int)crabButtonForButton:(OESMSButton)button player:(NSUInteger)player;
- (int)crabButtonForSG1000Button:(OESG1000Button)button;
- (int)crabButtonForColecoVisionButton:(OEColecoVisionButton)button player:(NSUInteger)player;
@end

@implementation SMSGameCore

// Global variables because the callbacks need to access them...
static OERingBuffer *ringBuffer;
console_t *cur_console;

- (id)init
{
    self = [super init];
    if(self != nil)
    {
        bufLock = [[NSLock alloc] init];
        tempBuffer = malloc(256 * 256 * 4);
        cheatList = [[NSMutableDictionary alloc] init];
        ringBuffer = [self ringBufferAtIndex:0];
    }
    return self;
}

- (void)dealloc
{
    DLog(@"releasing/deallocating CrabEmu memory");
    free(tempBuffer);

    cur_console->shutdown();
}

# pragma mark - Execution

- (BOOL)loadFileAtPath:(NSString *)path error:(NSError **)error
{
    romFile = [NSURL fileURLWithPath:path];
    int console = rom_detect_console([path UTF8String]);
    DLog(@"Loaded File");
    //TODO: add choice NTSC/PAL
    if(console == CONSOLE_COLECOVISION)
    {
        NSString *biosPath = [[self biosDirectoryPath] stringByAppendingPathComponent:@"coleco.rom"];
        coleco_init(VIDEO_NTSC);
        coleco_mem_load_bios([biosPath UTF8String]);
        coleco_mem_load_rom([path UTF8String]);
    }
    else
    {
        NSData *dataObj = [NSData dataWithContentsOfFile:[path stringByStandardizingPath]];
        const void *data = [dataObj bytes];
        uint16_t offset = 0;
        int region = SMS_REGION_DOMESTIC;

        // Detect SMS ROM header
        if(memcmp(&data[0x1ff0], "TMR SEGA", 8) == 0)
            offset = 0x1ff0;
        else if (memcmp(&data[0x3ff0], "TMR SEGA", 8) == 0)
            offset = 0x3ff0;
        else if (memcmp(&data[0x7ff0], "TMR SEGA", 8) == 0)
            offset = 0x7ff0;

        if(offset)
        {
            // Set machine region
            switch (((char *)data)[offset + 0x0f] >> 4)
            {
                case 3: // SMS Japan
                    region = SMS_REGION_DOMESTIC;
                    break;
                case 4: // SMS Export
                    // Force system region to Japan if user locale is Japan and the cart is world/multi-region
                    region = [[self systemRegion] isEqualToString: @"Japan"] ? SMS_REGION_DOMESTIC : SMS_REGION_EXPORT;
                    break;
                case 5: // GG Japan
                case 6: // GG Export
                case 7: // GG International
                default:
                    region = SMS_REGION_DOMESTIC;
                    break;
            }
        }
        else
            // No header means Japan region
            region = SMS_REGION_DOMESTIC;

        sms_init(SMS_VIDEO_NTSC, region, 0); // 1 = VDP borders
        sms_mem_load_rom([path UTF8String], console);
        cur_console->frame(0);
    }

    if(cur_console->console_type != CONSOLE_COLECOVISION)
    {
        NSString *extensionlessFilename = [[romFile lastPathComponent] stringByDeletingPathExtension];
        NSURL *batterySavesDirectory = [NSURL fileURLWithPath:[self batterySavesDirectoryPath]];
        [[NSFileManager defaultManager] createDirectoryAtURL:batterySavesDirectory withIntermediateDirectories:YES attributes:nil error:nil];
        NSURL *saveFile = [batterySavesDirectory URLByAppendingPathComponent:[extensionlessFilename stringByAppendingPathExtension:@"sav"]];

        if([saveFile checkResourceIsReachableAndReturnError:nil] && sms_read_cartram_from_file([[saveFile path] UTF8String]) == 0)
            NSLog(@"CrabEmu: Loaded sram");
    }

    return YES;
}

- (void)executeFrame
{
    [bufLock lock];
    cur_console->frame(0);
    [bufLock unlock];
}

- (void)resetEmulation
{
    cur_console->soft_reset();
}

- (void)stopEmulation
{
    if(cur_console->console_type != CONSOLE_COLECOVISION)
    {
        NSString *extensionlessFilename = [[romFile lastPathComponent] stringByDeletingPathExtension];
        NSURL *batterySavesDirectory = [NSURL fileURLWithPath:[self batterySavesDirectoryPath]];
        NSURL *saveFile = [batterySavesDirectory URLByAppendingPathComponent:[extensionlessFilename stringByAppendingPathExtension:@"sav"]];

        cur_console->save_sram([[saveFile path] UTF8String]);
    }

    [super stopEmulation];
}

- (NSTimeInterval)frameInterval
{
    return 60;
}

# pragma mark - Video

- (OEIntSize)bufferSize
{
    return OEIntSizeMake(cur_console->console_type == CONSOLE_GG ? 160 : 256, cur_console->console_type == CONSOLE_GG ? 144 : 256);
}

- (OEIntRect)screenRect
{
    return OEIntRectMake(0, 0, cur_console->console_type == CONSOLE_GG ? 160 : 256, cur_console->console_type == CONSOLE_GG ? 144 : smsvdp.lines);
}

- (OEIntSize)aspectSize
{
    return OEIntSizeMake(cur_console->console_type == CONSOLE_GG ? 160 : 4, cur_console->console_type == CONSOLE_GG ? 144 : 3);
}

- (const void *)videoBuffer
{
    if (cur_console->console_type != CONSOLE_GG)
        return smsvdp.framebuffer;
    else
        for (int i = 0; i < 144; i++)
            //jump 24 lines, skip 48 pixels and capture for each line of the buffer 160 pixels
            // sizeof(unsigned char) is always equal to 1 by definition
            memcpy(tempBuffer + i * 160 * 4, smsvdp.framebuffer  + 24 * 256 * 1 + 48 * 1 + i * 256 * 1, 160 * 4);
    return tempBuffer;
}

- (GLenum)pixelFormat
{
    return GL_BGRA;
}

- (GLenum)pixelType
{
    return GL_UNSIGNED_INT_8_8_8_8_REV;
}

- (GLenum)internalPixelFormat
{
    return GL_RGB8;
}

# pragma mark - Audio

- (double)audioSampleRate
{
    return SAMPLERATE;
}

- (NSUInteger)channelCount
{
    return 2;
}

# pragma mark - Save States

- (BOOL)saveStateToFileAtPath:(NSString *)fileName
{
    return cur_console->save_state([fileName fileSystemRepresentation]) == 0;
}

- (BOOL)loadStateFromFileAtPath:(NSString *)fileName
{
    return cur_console->load_state([fileName fileSystemRepresentation]) == 0;
}

- (NSData *)serializeStateWithError:(NSError **)outError
{
    void *bytes;
    size_t length;
    FILE *fp = open_memstream((char **)&bytes, &length);

    int status;
    if(cur_console->console_type == CONSOLE_COLECOVISION) {
        status = coleco_write_state(fp);
    }
    else {
        status = sms_write_state(fp);
    }

    if(status == 0) {
        fclose(fp);
        return [NSData dataWithBytesNoCopy:bytes length:length];
    }
    else {
        if(outError) {
            *outError = [NSError errorWithDomain:OEGameCoreErrorDomain
                                            code:OEGameCoreCouldNotSaveStateError
                                        userInfo:@{
                                                   NSLocalizedDescriptionKey : @"Save state data could not be written",
                                                   NSLocalizedRecoverySuggestionErrorKey : @"The emulator could not write the state data."
                                                   }];
        }

        fclose(fp);
        free(bytes);
        return nil;
    }
}

- (BOOL)deserializeState:(NSData *)state withError:(NSError **)outError
{
    const void *bytes = [state bytes];
    size_t length = [state length];

    FILE *fp = fmemopen((void *)bytes, length, "rb");

    int status;
    if(cur_console->console_type == CONSOLE_COLECOVISION) {
        status = coleco_read_state(fp);
    }
    else {
        status = sms_read_state(fp);
    }
    fclose(fp);

    if(status == 0)
    {
        return YES;
    }
    else
    {
        if(outError)
        {
            *outError = [NSError errorWithDomain:OEGameCoreErrorDomain
                                            code:OEGameCoreCouldNotLoadStateError
                                        userInfo:@{
                                                   NSLocalizedDescriptionKey : @"The save state data could not be read",
                                                   NSLocalizedRecoverySuggestionErrorKey : @"Could not load data from the save state"
                                                   }];
        }

        return NO;
    }
}

/*
 CrabEmu callbacks
 */

void sound_update_buffer(signed short *buf, int length)
{
    //NSLog(@"%s %p", __FUNCTION__, ringBuffer);
    [ringBuffer write:buf maxLength:length];
}

int sound_init(int channels, int region)
{
    return 0;
}

void sound_shutdown(void)
{

}

void sound_reset_buffer(void)
{

}

void gui_set_viewport(int w, int h)
{
    //NSLog(@"viewport, width: %d, height: %d", w, h);
}

void gui_set_aspect(float x, float y)
{
    //NSLog(@"set_aspect, x: %f, y: %f", x, y);
}

void gui_set_title(const char *str)
{
    //NSLog(@"set_title%s", str);
}

void gui_set_console(console_t *c)
{
    cur_console = c;
}

# pragma mark - Input

- (int)crabButtonForButton:(OESMSButton)button player:(NSUInteger)player;
{
    int btn = -1;
    switch(button)
    {
        case OESMSButtonUp    : btn = SMS_UP;       break;
        case OESMSButtonDown  : btn = SMS_DOWN;     break;
        case OESMSButtonLeft  : btn = SMS_LEFT;     break;
        case OESMSButtonRight : btn = SMS_RIGHT;    break;
        case OESMSButtonA     : btn = SMS_BUTTON_1; break;
        case OESMSButtonB     : btn = SMS_BUTTON_2; break;
        default : break;
    }

    return btn;
}

- (int)crabButtonForButton:(OEGGButton)button;
{
    int btn = -1;
    switch(button)
    {
        case OEGGButtonUp:    btn = SMS_UP;         break;
        case OEGGButtonDown:  btn = SMS_DOWN;       break;
        case OEGGButtonLeft:  btn = SMS_LEFT;       break;
        case OEGGButtonRight: btn = SMS_RIGHT;      break;
        case OEGGButtonA:     btn = SMS_BUTTON_1;   break;
        case OEGGButtonB:     btn = SMS_BUTTON_2;   break;
        case OEGGButtonStart: btn = GAMEGEAR_START; break;
        default : break;
    }

    return btn;
}

- (int)crabButtonForSG1000Button:(OESG1000Button)button;
{
    int btn = -1;
    switch(button)
    {
        case OESG1000ButtonUp:    btn = SMS_UP;       break;
        case OESG1000ButtonDown:  btn = SMS_DOWN;     break;
        case OESG1000ButtonLeft:  btn = SMS_LEFT;     break;
        case OESG1000ButtonRight: btn = SMS_RIGHT;    break;
        case OESG1000Button1:     btn = SMS_BUTTON_1; break;
        case OESG1000Button2:     btn = SMS_BUTTON_2; break;
        default : break;
    }

    return btn;
}

- (int)crabButtonForColecoVisionButton:(OEColecoVisionButton)button player:(NSUInteger)player;
{
    int btn = -1;
    switch(button)
    {
        case OEColecoVisionButtonUp          : btn = COLECOVISION_UP;       break;
        case OEColecoVisionButtonDown        : btn = COLECOVISION_DOWN;     break;
        case OEColecoVisionButtonLeft        : btn = COLECOVISION_LEFT;     break;
        case OEColecoVisionButtonRight       : btn = COLECOVISION_RIGHT;    break;
        case OEColecoVisionButtonLeftAction  : btn = COLECOVISION_L_ACTION; break;
        case OEColecoVisionButtonRightAction : btn = COLECOVISION_R_ACTION; break;
        case OEColecoVisionButton1           : btn = COLECOVISION_1;        break;
        case OEColecoVisionButton2           : btn = COLECOVISION_2;        break;
        case OEColecoVisionButton3           : btn = COLECOVISION_3;        break;
        case OEColecoVisionButton4           : btn = COLECOVISION_4;        break;
        case OEColecoVisionButton5           : btn = COLECOVISION_5;        break;
        case OEColecoVisionButton6           : btn = COLECOVISION_6;        break;
        case OEColecoVisionButton7           : btn = COLECOVISION_7;        break;
        case OEColecoVisionButton8           : btn = COLECOVISION_8;        break;
        case OEColecoVisionButton9           : btn = COLECOVISION_9;        break;
        case OEColecoVisionButton0           : btn = COLECOVISION_0;        break;
        case OEColecoVisionButtonAsterisk    : btn = COLECOVISION_STAR;     break;
        case OEColecoVisionButtonPound       : btn = COLECOVISION_POUND;    break;
        default : break;
    }

    return btn;
}

- (oneway void)didPushGGButton:(OEGGButton)button;
{
    int btn = [self crabButtonForButton:button];
    if(btn > -1) sms_button_pressed(1, btn);
}

- (oneway void)didReleaseGGButton:(OEGGButton)button;
{
    int btn = [self crabButtonForButton:button];
    if(btn > -1) sms_button_released(1, btn);
}

- (oneway void)didPushSMSButton:(OESMSButton)button forPlayer:(NSUInteger)player;
{
    int btn = [self crabButtonForButton:button player:player];
    if(btn > -1) sms_button_pressed(player, btn);
}

- (oneway void)didReleaseSMSButton:(OESMSButton)button forPlayer:(NSUInteger)player;
{
    int btn = [self crabButtonForButton:button player:player];
    if(btn > -1) sms_button_released(player, btn);
}

- (oneway void)didPushSMSStartButton;
{
    sms_button_pressed(1, GAMEGEAR_START);
}

- (oneway void)didReleaseSMSStartButton;
{

}

- (oneway void)didPushSMSResetButton;
{
    sms_button_pressed(1, SMS_CONSOLE_RESET);
}

- (oneway void)didReleaseSMSResetButton;
{
    sms_button_released(1, SMS_CONSOLE_RESET);
}

- (oneway void)didPushSG1000Button:(OESG1000Button)button;
{
    int btn = [self crabButtonForSG1000Button:button];
    if(btn > -1) sms_button_pressed(1, btn);
}

- (oneway void)didReleaseSG1000Button:(OESG1000Button)button;
{
    int btn = [self crabButtonForSG1000Button:button];
    if(btn > -1) sms_button_released(1, btn);
}

- (oneway void)didPushColecoVisionButton:(OEColecoVisionButton)button forPlayer:(NSUInteger)player;
{
    int btn = [self crabButtonForColecoVisionButton:button player:player];
    if(btn > -1) coleco_button_pressed(player, btn);
}

- (oneway void)didReleaseColecoVisionButton:(OEColecoVisionButton)button forPlayer:(NSUInteger)player;
{
    int btn = [self crabButtonForColecoVisionButton:button player:player];
    if(btn > -1) coleco_button_released(player, btn);
}

#pragma mark - Cheats

- (void)setCheat:(NSString *)code setType:(NSString *)type setEnabled:(BOOL)enabled
{
    // Sanitize
    code = [code stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];

    // Remove any spaces
    code = [code stringByReplacingOccurrencesOfString:@" " withString:@""];

    // Remove address-value separator
    code = [code stringByReplacingOccurrencesOfString:@"-" withString:@""];

    if (enabled)
        [cheatList setValue:@YES forKey:code];
    else
        [cheatList removeObjectForKey:code];

    sms_cheat_reset();

    NSArray *multipleCodes = [[NSArray alloc] init];

    // Apply enabled cheats found in dictionary
    for (id key in cheatList)
    {
        if ([[cheatList valueForKey:key] isEqual:@YES])
        {
            // Handle multi-line cheats
            multipleCodes = [key componentsSeparatedByString:@"+"];
            for (NSString *singleCode in multipleCodes)
            {
                if ([singleCode length] == 8)
                {
                    // Action Replay GG/SMS format: XXXX-YYYY
                    NSString *address = [singleCode substringWithRange:NSMakeRange(0, 4)];
                    NSString *value = [singleCode substringWithRange:NSMakeRange(4, 4)];

                    // Convert AR hex to int
                    uint32_t outAddress, outValue;
                    NSScanner *scanAddress = [NSScanner scannerWithString:address];
                    NSScanner *scanValue = [NSScanner scannerWithString:value];
                    [scanAddress scanHexInt:&outAddress];
                    [scanValue scanHexInt:&outValue];

                    sms_cheat_t *arCode = (sms_cheat_t *)malloc(sizeof(sms_cheat_t));
                    memset(arCode, 0, sizeof(sms_cheat_t));
                    arCode->ar_code = (outAddress << 16) | outValue;
                    strcpy(arCode->desc, [singleCode UTF8String]);
                    arCode->enabled = 1;

                    sms_cheat_add(arCode);
                    sms_cheat_enable();
                }
            }
        }
    }
}

@end
