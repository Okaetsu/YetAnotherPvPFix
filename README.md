# 29th December 2025 Update

PvP is now fixed with 0.7.0 update of Palworld so this mod is obsolete and will not be maintained further.

# YetAnotherPvPFix

Tired of having PvP break every single patch? This project aims to maintain PvP functionality without having to do complicated config adjustments like WorldOption.sav or other workarounds.

Does not rely on `bIsPvP` in PalWorldSettings.ini which breaks a lot of functionality like mounting and being unable to assign certain player status points. This mod uses its own config file in the mod's own folder called PVP-settings.ini where you can toggle both player and building damage individually.

# Installation

Installation is pretty straightforward, you'll need this version of [UE4SS](https://github.com/Okaetsu/RE-UE4SS/releases/tag/experimental-palworld) and the mod itself goes into the Mods folder in Win64/ue4ss/Mods

Once installed, your folder structure should look like the diagram below:

```
.
└── Win64/
    └── ue4ss/
        └── Mods/
            └── YetAnotherPvPFix/
                ├── dlls/
                │   └── main.dll
                ├── enabled.txt
                └── PVP-settings.ini
```

Note that this mod is intended for Windows Dedicated Servers only and may not work in Co-Op (Invite Only sessions).
