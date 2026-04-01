# YetAnotherPvPFix

Skips needing `bIsPvP` to be enabled to avoid side-effects like not being able to use elixirs, players being able to interact with chests in bases that they don't own and some other stuff I'm forgetting.

# Installation

Installation is pretty straightforward, grab the [latest version of experimental UE4SS](https://github.com/UE4SS-RE/RE-UE4SS/releases/tag/experimental-latest) and the mod itself goes into the Mods folder in Win64/ue4ss/Mods

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
