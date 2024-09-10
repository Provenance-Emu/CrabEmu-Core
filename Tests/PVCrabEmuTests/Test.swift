//
//  Test.swift
//  PVVirtualJaguar
//
//  Created by Joseph Mattiello on 8/5/24.
//

import Testing
import PVEmulatorCore
@testable import libcrabemu
@testable import PVCrabEmuBridge
@testable import PVCrabEmu

struct Test {
    
    let testRomFilename: String = ""

    @Test func VirtualJaguarTest() async throws {
        // Write your test here and use APIs like `#expect(...)` to check expected conditions.
        let core = PVJaguarGameCore()
        #expect(core != nil)
    }
    
    @Test func LoadFileTest() async throws {
        // Write your test here and use APIs like `#expect(...)` to check expected conditions.
        let core = PVJaguarGameCore()
        #expect(core != nil)

//        do {
//            try core.loadFile(atPath: testRomFilename)
//        } catch {
//            print("Failed to load file: \(error.localizedDescription)")
//            throw error
//        }
    }
}
