//
//  CrabEMUGameCore.swift
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
public class CrabEMUGameCore: PVEmulatorCore, @unchecked Sendable {
    
    let _bridge: PVCrabEmuBridge = .init()
    
    required init() {
        super.init()
        #if NO_ZLIB
        self.extractArchive = true
        #else
        self.extractArchive = false
        #endif
        self.bridge = (_bridge as! any ObjCBridgedCoreBridge)
    }
    
    // CrabEMu
    public var bufLock: NSLock = .init()
    public var paused: Bool = false
    public var romFile: URL? = nil
}

extension CrabEMUGameCore: GameWithCheat {
    public func setCheat(code: String, type: String, codeType: String, cheatIndex: UInt8, enabled: Bool) -> Bool {
        // TODO: Support this cheat type or make it optional in the protocol
        return false
    }
    
    public var supportsCheatCode: Bool {
        switch _bridge.systemType {
        case .SMS, .GameGear:
            return true
        default:
            return false
        }
    }
    
    public var cheatCodeTypes: [String] {
        return getCheatCodeTypes()
    }
    
    public func setCheat(code: String, type: String, enabled: Bool ) -> Bool {
       _bridge.setCheat(code, setType: type, setEnabled: enabled)
        return true;
    }

    @objc public func getCheatCodeTypes() -> [String] {
        // GameGear: Action Replay
        // SMS: Action Replay
        switch _bridge.systemType {
        case .SMS, .GameGear:
            return ["Pro Action Replay"]
        default:
            return []
        }
    }
}
extension CrabEMUGameCore: PVSG1000SystemResponderClient {
    public func didPush(_ button: PVCoreBridge.PVSG1000Button, forPlayer player: Int) {
        (_bridge as! PVSG1000SystemResponderClient).didPush(button, forPlayer: player)
    }
    public func didRelease(_ button: PVCoreBridge.PVSG1000Button, forPlayer player: Int) {
        (_bridge as! PVSG1000SystemResponderClient).didRelease(button, forPlayer: player)
    }
}
extension CrabEMUGameCore: PVMasterSystemSystemResponderClient {
    public func didPush(_ button: PVCoreBridge.PVMasterSystemButton, forPlayer player: Int) {
        (_bridge as! PVMasterSystemSystemResponderClient).didPush(button, forPlayer: player)
    }
    public func didRelease(_ button: PVCoreBridge.PVMasterSystemButton, forPlayer player: Int) {
        (_bridge as! PVMasterSystemSystemResponderClient).didRelease(button, forPlayer: player)
    }
}
extension CrabEMUGameCore: PVColecoVisionSystemResponderClient {
    public func didPush(_ button: PVCoreBridge.PVColecoVisionButton, forPlayer player: Int) {
        (_bridge as! PVColecoVisionSystemResponderClient).didPush(button, forPlayer: player)
    }
    public func didRelease(_ button: PVCoreBridge.PVColecoVisionButton, forPlayer player: Int) {
        (_bridge as! PVColecoVisionSystemResponderClient).didRelease(button, forPlayer: player)
    }
}
