#pragma once

namespace NManiaPlanet {
    enum {
        ECurVersion = 3,
    };

    typedef unsigned int Nat32;
    typedef unsigned int Bool;

    struct Vec3 {
        float x, y, z;
    };
    struct Quat {
        float w, x, y, z;
    };

    struct STelemetry {
        struct SHeader {
            char        Magic[32] = "TmForever_Telemetry";
            Nat32       Version = 3;
            Nat32       Size = sizeof(STelemetry);
        };
        enum EGameState {
            EState_Starting = 0,
            EState_Menus,
            EState_Running,
            EState_Paused,
        };
        enum ERaceState {
            ERaceState_BeforeState = 0,
            ERaceState_Running,
            ERaceState_Finished,
        };
        struct SGameState {
            EGameState  State;
            char        GameplayVariant[64];    // player model 'StadiumCar', 'CanyonCar', ....
            char        MapId[64];
            char        MapName[256];
            char        __future__[128];
        };
        struct SRaceState {
            ERaceState  State;
            Nat32       Time;
            Nat32       NbRespawns;
            Nat32       NbCheckpoints;
            Nat32       CheckpointTimes[125];
            Nat32       NbCheckpointsPerLap;
            Nat32       NbLapsPerRace;
            Nat32       Timestamp;
            Nat32       StartTimestamp;         // timestamp when the State will change to 'Running', or has changed when after the racestart.
            char        __future__[16];
        };
        struct SObjectState {
            Nat32       Timestamp;
            Nat32       DiscontinuityCount = 0;     // the number changes everytime the object is moved not continuously (== teleported).
            Quat        Rotation;
            Vec3        Translation;            // +x is "left", +y is "up", +z is "front"
            Vec3        Velocity;               // (world velocity)
            Nat32       LatestStableGroundContactTime;
            char        __future__[32];
        };
        struct SVehicleState {
            Nat32       Timestamp;

            float       InputSteer;
            float       InputGasPedal;
            Bool        InputIsBraking;
            Bool        InputIsHorn;

            float       EngineRpm;              // 1500 -> 10000
            int         EngineCurGear;
            float       EngineTurboRatio;       // 1 turbo starting/full .... 0 -> finished
            Bool        EngineFreeWheeling;

            Bool        WheelsIsGroundContact[4];
            Bool        WheelsIsSliping[4];
            float       WheelsDamperLen[4];
            float       WheelsDamperRangeMin;
            float       WheelsDamperRangeMax;

            float       RumbleIntensity = 0;

            Nat32       SpeedMeter;             // unsigned km/h
            Bool        IsInWater;
            Bool        IsSparkling = 0;
            Bool        IsLightTrails;
            Bool        IsLightsOn;
            Bool        IsFlying;               // long time since touching ground.
            Bool        IsOnIce;

                                                // 000000000000000000000000000 G S B F N
                                                // 000000000000000000000000000 1 1 1 1 1
                                               //0b000000000000000000000000000 1 0 0 0 0 
                                               //0b000000000000000000000000000 0 1 0 0 0 
                                               //0b000000000000000000000000000 0 0 1 0 0 
                                               //0b000000000000000000000000000 0 0 0 1 0 
                                               //0b000000000000000000000000000 0 0 0 0 1 
            Nat32       Handicap;               // bit mask: [reserved..] [NoGrip] [NoSteering] [NoBrakes] [EngineForcedOn] [EngineForcedOff]
            float       BoostRatio = 1;             // 1 thrusters starting/full .... 0 -> finished

            char        __future__[20];
        };
        struct SDeviceState {   // VrChair state.
            Vec3        Euler;                  // yaw, pitch, roll  (order: pitch, roll, yaw)
            float       CenteredYaw;            // yaw accumulated + recentered to apply onto the device
            float       CenteredAltitude;       // Altitude accumulated + recentered

            char        __future__[32];
        };

        struct SPlayerState {
            Bool        IsLocalPlayer = 1;          // Is the locally controlled player, or else it is a remote player we're spectating, or a replay.
            char        Trigram[4] = "TMN";             // 'TMN'
            char        DossardNumber[4] = "000";       // '01'
            float       Hue;
            char        UserName[256];
            char        __future__[28];
        };

        SHeader         Header;

        Nat32           UpdateNumber;
        SGameState      Game;
        SRaceState      Race;
        SObjectState    Object;
        SVehicleState   Vehicle;
        SDeviceState    Device;
        SPlayerState    Player;
    };
}