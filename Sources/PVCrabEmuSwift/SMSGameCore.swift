//
//  SMSGameCore.swift
//  PVCrabEmu
//
//  Created by Joseph Mattiello on 5/25/24.
//  Copyright © 2024 Provenance EMU. All rights reserved.
//

import Foundation
public import PVEmulatorCore

@objc
@objcMembers
public class SMSGameCore: PVEmulatorCore {
    public var valueChangedHandler: GCExtendedGamepadValueChangedHandler? = nil

    // MARK: Cheats
    public var cheatList: NSMutableDictionary = .init()
    public var supportsCheatCode: Bool { true }

    // CrabEMu
    public var bufLock: NSLock = .init()
    public var paused: Bool = false
    public var romFile: URL? = nil
}
