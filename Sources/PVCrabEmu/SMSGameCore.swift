//
//  SMSGameCore.swift
//  PVCrabEmu
//
//  Created by Joseph Mattiello on 5/25/24.
//  Copyright © 2024 Provenance EMU. All rights reserved.
//

import Foundation
import PVEmulatorCore
import PVCrabEmuBridge
import PVCoreBridge

#if canImport(GameController)
import GameController
#endif

@objc
@objcMembers
public class SMSGameCore: PVEmulatorCore, ObjCBridgedCore, @unchecked Sendable {
    
    // PVEmulatorCoreBridged
    public typealias Bridge = PVCrabEmuBridge
    public lazy var bridge: Bridge = {
        let core = PVCrabEmuBridge()
        return core
    }()
    
    
#if canImport(GameController)
    public var valueChangedHandler: GCExtendedGamepadValueChangedHandler? = nil
#endif
    
    required init() {
        super.init()
        #if NO_ZLIB
        self.extractArchive = true
        #else
        self.extractArchive = false
        #endif
    }
    
    // MARK: Cheats
    public var cheatList: NSMutableDictionary = .init()
    public var supportsCheatCode: Bool { true }

    // CrabEMu
    public var bufLock: NSLock = .init()
    public var paused: Bool = false
    public var romFile: URL? = nil
}
