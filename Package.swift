// swift-tools-version:6.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "PVCrabEmu",
    platforms: [
        .iOS(.v17),
        .tvOS("15.4"),
        .watchOS(.v9),
        .macOS(.v11),
        .macCatalyst(.v17),
        .visionOS(.v1)
    ],
    products: [
        // Products define the executables and libraries produced by a package, and make them visible to other packages.
        .library(
            name: "PVCrabEmu",
            targets: ["PVCrabEmu"]),
        .library(
            name: "PVCrabEmu-Dynamic",
            type: .dynamic,
            targets: ["PVCrabEmu"]),
        .library(
            name: "PVCrabEmu-Static",
            type: .static,
            targets: ["PVCrabEmu"]),
    ],
    dependencies: [
        .package(path: "../../PVAudio"),
        .package(path: "../../PVCoreBridge"),
        .package(path: "../../PVCoreObjCBridge"),
        .package(path: "../../PVEmulatorCore"),
        .package(path: "../../PVLogging"),
        .package(path: "../../PVObjCUtils"),
        .package(path: "../../PVPlists"),
        .package(path: "../../PVSupport"),
        .package(path: "../../PVSettings"),
        .package(url: "https://github.com/Provenance-Emu/SwiftGenPlugin.git", branch: "develop"),
        
        //        .package(path: "../../External/minizip")
        .package(url: "https://github.com/ZipArchive/ZipArchive.git", branch: "master"),
    ],
    targets: [
        // MARK: ---- Core ----

        .target(
            name: "PVCrabEmu",
            dependencies: [
                "PVEmulatorCore",
                "PVCoreBridge",
                "PVLogging",
                "PVAudio",
                "PVSupport",
                "libcrabemu",
                "PVPlists",
                "PVSettings",
                "PVCrabEmuBridge"
            ],
            resources: [
                .process("Resources/Core.plist")
            ],
            cSettings: [
                .define("USE_STRUCTS", to: "1"),
                .define("__LIBRETRO__", to: "1"),
                .define("HAVE_COCOATOJUCH", to: "1"),
                .define("__GCCUNIX__", to: "1"),
                .headerSearchPath("../libcrabemu/include"),
                .headerSearchPath("../libcrabemu/crabemu")
            ],
            plugins: [
                .plugin(name: "SwiftGenPlugin", package: "SwiftGenPlugin")
            ]
        ),
        // MARK: ---- Bridge ----
        .target(
            name: "PVCrabEmuBridge",
            dependencies: [
                "PVEmulatorCore",
                "PVCoreBridge",
                "PVSupport",
                "PVPlists",
                "PVCoreObjCBridge",
                "PVObjCUtils",
                "PVSettings",
                "libcrabemu",
            ],
            cSettings: [
                .define("USE_STRUCTS", to: "1"),
                .define("__LIBRETRO__", to: "1"),
                .define("HAVE_COCOATOJUCH", to: "1"),
                .define("__GCCUNIX__", to: "1"),
                .headerSearchPath("../libcrabemu/include"),
                .headerSearchPath("../libcrabemu/crabemu"),
                .headerSearchPath("../libcrabemu/crabemu/sound/"),
                .headerSearchPath("../libcrabemu/crabemu/consoles/"),
                .headerSearchPath("../libcrabemu/crabemu/consoles/chip8/"),
                .headerSearchPath("../libcrabemu/crabemu/consoles/colecovision/"),
                .headerSearchPath("../libcrabemu/crabemu/consoles/nes/"),
                .headerSearchPath("../libcrabemu/crabemu/consoles/sms/"),
                .headerSearchPath("../libcrabemu/crabemu/cpu/Crab6502/"),
                .headerSearchPath("../libcrabemu/crabemu/cpu/CrabZ80/"),
                .headerSearchPath("../libcrabemu/crabemu/cpu/cz80/"),
                .headerSearchPath("../libcrabemu/crabemu/fmemopen/"),
                .headerSearchPath("../libcrabemu/crabemu/sound/"),
                .headerSearchPath("../libcrabemu/crabemu/sound/nes_apu"),
                .headerSearchPath("../libcrabemu/crabemu/utils/"),
                .headerSearchPath("../libcrabemu/crabemu/utils/minizip"),
                .headerSearchPath("../libcrabemu/crabemu/utils/scale2x"),
            ]
        ),
        // MARK: ---- Emulator ----
        .target(
            name: "libcrabemu",
            dependencies: [
                //"minizip"
                "ZipArchive"
            ],
            exclude: [
            ],
            sources: [
                "crabemu/consoles/colecovision/colecomem.c",
                "crabemu/consoles/colecovision/colecovision.c",
                "crabemu/consoles/nes/mappers/nesmapper0.c",
                "crabemu/consoles/nes/mappers/nesmapper1.c",
                "crabemu/consoles/nes/mappers/nesmapper2.c",
                "crabemu/consoles/nes/mappers/nesmapper3.c",
                "crabemu/consoles/nes/mappers/nesmapper66.c",
                "crabemu/consoles/nes/mappers/nesmapper7.c",
                "crabemu/consoles/nes/mappers/nesmapper9.c",
                "crabemu/consoles/nes/nes.c",
                "crabemu/consoles/nes/nesmem.c",
                "crabemu/consoles/nes/nesppu.c",
                "crabemu/consoles/sms/93c46.c",
                "crabemu/consoles/sms/cheats.c",
                "crabemu/consoles/sms/mapper-4PAA.c",
                "crabemu/consoles/sms/mapper-93c46.c",
                "crabemu/consoles/sms/mapper-codemasters.c",
                "crabemu/consoles/sms/mapper-janggun.c",
                "crabemu/consoles/sms/mapper-korean.c",
                "crabemu/consoles/sms/mapper-koreanmsx.c",
                "crabemu/consoles/sms/mapper-none.c",
                "crabemu/consoles/sms/mapper-sega.c",
                "crabemu/consoles/sms/mapper-sg1000.c",
                "crabemu/consoles/sms/mappers.c",
                "crabemu/consoles/sms/sdscterminal.c",
                "crabemu/consoles/sms/sms.c",
                "crabemu/consoles/sms/smsmem-gg.c",
                "crabemu/consoles/sms/smsmem.c",
                "crabemu/consoles/sms/smsvdp.c",
                "crabemu/consoles/sms/smsz80.c",
                "crabemu/consoles/sms/terebi.c",
                "crabemu/consoles/sms/tms9918a.c",
                "crabemu/cpu/Crab6502/Crab6502.c",
                "crabemu/cpu/Crab6502/Crab6502d.c",
                "crabemu/cpu/CrabZ80/CrabZ80.c",
                "crabemu/cpu/CrabZ80/CrabZ80d.c",
                "crabemu/fmemopen/fmemopen.c",
                "crabemu/fmemopen/open_memstream.c",
                "crabemu/rom.c",
                "crabemu/sound/nes_apu/fds_snd.c",
                "crabemu/sound/nes_apu/mmc5_snd.c",
                "crabemu/sound/nes_apu/nes_apu.c",
                "crabemu/sound/nes_apu/vrcvisnd.c",
                "crabemu/sound/nesapu-nosefart.c",
                "crabemu/sound/sn76489.c",
                "crabemu/sound/ym2413.c",
                //                "crabemu/utils/minizip/ioapi.c",
                //                "crabemu/utils/minizip/unzip.c",
            ],
            packageAccess: true,
            cSettings: [
                .define("CRABZ80_NO_READMAP_FALLBACK"),
                .define("IN_CRABEMU"),
                
                .define("USE_STRUCTS", to: "1"),
                .define("__LIBRETRO__", to: "1"),
                .define("HAVE_COCOATOUUCH", to: "1"),
                .define("__GCCUNIX__", to: "1"),
                .headerSearchPath("./"),
                .headerSearchPath("crabemu/sound/"),
                .headerSearchPath("crabemu/consoles/"),
                .headerSearchPath("crabemu/consoles/chip8/"),
                .headerSearchPath("crabemu/consoles/colecovision/"),
                .headerSearchPath("crabemu/consoles/nes/"),
                .headerSearchPath("crabemu/consoles/sms/"),
                .headerSearchPath("crabemu/cpu/Crab6502/"),
                .headerSearchPath("crabemu/cpu/CrabZ80/"),
                .headerSearchPath("crabemu/cpu/cz80/"),
                .headerSearchPath("crabemu/fmemopen/"),
                .headerSearchPath("crabemu/sound/"),
                .headerSearchPath("crabemu/sound/nes_apu"),
                .headerSearchPath("crabemu/utils/"),
                .headerSearchPath("crabemu/utils/minizip"),
                .headerSearchPath("crabemu/utils/scale2x"),
            ],
            linkerSettings: [
                .linkedLibrary("z"),
                .linkedLibrary("bz2")
            ]
        ),
        // MARK: --- Tests
        .testTarget(
            name: "PVCrabEmuTests",
            dependencies: ["PVCrabEmu"]),
    ],
    swiftLanguageModes: [.v5, .v6],
    cLanguageStandard: .gnu11,
    cxxLanguageStandard: .gnucxx17
)
