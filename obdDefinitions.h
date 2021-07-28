#ifndef OBDDEFINITIONS_H
#define OBDDEFINITIONS_H

#define OBD_QUERY_ID 0x7DF
#define OBD_REPLY_ID 0x7E8

enum OBDMode {
    RealTime = 0x01,
    FreezeData = 0x02,
    DTC = 0x03,
    EraseDTC = 0x04,
    PendingDTC = 0x07,
    VehicleInfo = 0x09
};

enum OBDRplyMode {
    RealTimeRply = 0x41,
    FreezeDataRply,
    DTCRply = 0x43,
    EraseDTCRply,
    VehicleInfoRply = 0x49
};

enum PIDS {
    FirstSupportedPIDs = 0x00,
    MAP = 0x0B,
    RPM = 0x0C,
    VehicleSpeed = 0x0D,
    IntakeAirTemperature = 0x0F,
    MAF = 0x10,
    SecondSUpportedPIDs = 0x20,
    MalfunctionDistance = 0x21,
    FuelLevel = 0x2F,
    EngineCoolantTemperature = 0x05
};
#endif // OBDDEFINITIONS_H
