#include "canMessageHandler.h"
#include <wiringPi/wiringPi.h>
#include <wiringPi/wiringPiSPI.h>

CANMessagesHandler::CANMessagesHandler():
    can(0, 500000, 6)
    , m_canMsgId(0)
    , m_canMsgDLC(0)
    , m_engineRpm(0)
    , m_engineTemp(0)
    , m_fuelLevel(0)
    , m_intakeAirTemp(0)
    , m_mafRatio(0)
    , m_malfunctionIndicator(false)
    , m_writeCANThread(this)
{
    //*class instance of MCP CAN*//
    //initializing SPI parameters
    // SPI0
    // baudrate 500 kbit/s
    // interrupt pin 25 (BCM 6)
    wiringPiSetup();
    can.setupSpi();
    can.setupInterruptGpio();
    can.begin(MCP_NORMAL, CAN_500KBPS, MCP_8MHZ);
    can.setMode(MCP_NORMAL);

    m_writeCANThread.start(QThread::TimeCriticalPriority);
}

CANMessagesHandler::~CANMessagesHandler()
{
    m_writeCANThread.requestInterruption();
}

void CANMessagesHandler::pollLoop() {
    can.readMsgBuf(&m_canMsgId, &m_canMsgDLC, m_canMsgData);

    if (m_canMsgId == OBD_QUERY_ID) {
        m_obdMode = m_canMsgData[1];
        m_obdPID = m_canMsgData[2];
        handleOBDRequest();
        //clean received message id buffer after processing it to avoid
        //re-entering this condition if no OBD message is received
        m_canMsgId = 0x00;
    }
}

void CANMessagesHandler::setSpeed (int value) {
    if (m_speed != value) {
        m_speed = value;
        emit speedChanged();
    }
}

void CANMessagesHandler::setEngineRpm (int value) {
    if (m_engineRpm != value) {
        m_engineRpm = value;
        emit engineRpmChanged();
    }
}

void CANMessagesHandler::setEngineTemp (int value) {
    if (m_engineTemp != value) {
        m_engineTemp = value;
        emit engineTempChanged();
    }
}

void CANMessagesHandler::setFuelLevel (int value) {
    if (m_fuelLevel != value) {
        m_fuelLevel = value;
        emit fuelLevelChanged();
    }
}

void CANMessagesHandler::setMafRatio(float value) {
    if (m_mafRatio != value) {
        m_mafRatio = value;
        emit mafRatioChanged();
    }
}

void CANMessagesHandler::setIntakeAirTemp(int value) {
    if (m_intakeAirTemp != value) {
        m_intakeAirTemp = value;
        emit intakeAirTempChanged();
    }
}

void CANMessagesHandler::setMapPressure(int value) {
    if (m_mapPressure != value) {
        m_mapPressure = value;
        emit mapPressureChanged();
    }
}

void CANMessagesHandler::setMalfunctionDistance(int value) {
    if (m_malfunctionDistance != value) {
        m_malfunctionDistance = value;
        emit malfunctionDistanceChanged();
    }
}

int CANMessagesHandler::speed() {
    return m_speed;
}

int CANMessagesHandler::engineRpm() {
    return m_engineRpm;
}

int CANMessagesHandler::engineTemp() {
    return m_engineTemp;
}

int CANMessagesHandler::fuelLevel() {
    return m_fuelLevel;
}

bool CANMessagesHandler::malfunctionIndicator() {
    return m_malfunctionIndicator;
}

float CANMessagesHandler::mafRatio() {
    return m_mafRatio;
}

int CANMessagesHandler::intakeAirTemp() {
    return m_intakeAirTemp;
}

int CANMessagesHandler::mapPressure() {
    return m_mapPressure;
}

int CANMessagesHandler::malfunctionDistance() {
    return m_malfunctionDistance;
}

void CANMessagesHandler::handleOBDRequest()
{
    switch (m_obdMode)
    {
    case RealTime:
        processRealTimeDataQuery(m_obdPID);
        break;
    case DTC:
        processDTCQuery();
        break;
    case EraseDTC:
        eraseStoredDTC();
        break;
    case VehicleInfo:
        vehicleInfoReply();
        break;
    case PendingDTC:
        processDTCQuery();
        break;
    default:
        break;
    }
}

void CANMessagesHandler::processRealTimeDataQuery(INT8U pid)
{
    unsigned char messageToSend[MAX_CHAR_IN_MESSAGE] = {0x00, RealTimeRply, pid, 0x00, 0x00, 0x00, 0x00, 0x00};

    if (pid == FirstSupportedPIDs) {
        unsigned char msgToSendOne[8] = {0x06, 0x41, 0x00, 0x08, 0x3B, 0x00, 0x01, 0x00};
        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, msgToSendOne);
    }

    if (pid == SecondSUpportedPIDs) {
        unsigned char msgToSendTwo[8] = {0x06, 0x41, 0x20, 0x80, 0x02, 0x00, 0x00, 0x00};
        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, msgToSendTwo);
    }

    if (pid == 28)
    {
        unsigned char obd_Std_Msg[8] = {4, 65, 0x1C, 0x0A};
        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, obd_Std_Msg);
    }

    if (pid == FuelLevel) {
        messageToSend[0] = 3;
        messageToSend[3] = 100 * fuelLevel() / 255;

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, messageToSend);
    }

    if (pid == RPM) {
        unsigned int rpm_A = (long)engineRpm() * 4 / 256;
        unsigned int rpm_B = (long)engineRpm() * 4 % 256;

        messageToSend[0] = 4;
        messageToSend[3] = (unsigned char)rpm_A;
        messageToSend[4] = (unsigned char)rpm_B;

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, messageToSend);
    }

    if (pid == VehicleSpeed) {
        messageToSend[0] = 3;
        messageToSend[3] = speed();

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, messageToSend);
    }

    if (pid == IntakeAirTemperature) {
        messageToSend[0] = 3;
        messageToSend[3] = intakeAirTemp() + 40;

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, messageToSend);
    }

    if (pid == MAF) {
        messageToSend[0] = 4;
        messageToSend[3] = (long)mafRatio() / 255;
        messageToSend[4] = (long)mafRatio();

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, messageToSend);
    }

    if (pid == EngineCoolantTemperature) {
        messageToSend[0] = 3;
        messageToSend[3] = engineTemp() + 40;

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, messageToSend);
    }

    if (pid == MAP) {
        messageToSend[0] = 3;
        messageToSend[3] = mapPressure();

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, messageToSend);
    }

    if (pid == MalfunctionDistance) {
        //formula 256A + B
        messageToSend[0] = 4;
        messageToSend[3] = (long)malfunctionDistance() / 255;
        messageToSend[4] = (long)malfunctionDistance();

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, messageToSend);
    }
}

void CANMessagesHandler::eraseStoredDTC()
{
    setMalfunctionIndicator(false);
}

void CANMessagesHandler::processDTCQuery()
{
    //simulated DTCs:
    //P0261 ----> Cylinder 1 injector circuit low voltage
    //P0700 ----> Transmission Control System Malfunction
    unsigned char DTC[] = {6, DTCRply, 0x02, 0x02, 0x61, 0x07, 0x00, 0};

    if (m_malfunctionIndicator)
        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, DTC);
}

void CANMessagesHandler::vehicleInfoReply()
{
    unsigned char vinStr[18] = "SCBZK25EXYC813876";
    unsigned char calibration_ID[18] = "FW00116MHZ1111111";

    if (m_canMsgData[2] == 0)
    {
        unsigned char msgToSendNine[8] = {0x06, 0x49, 0x00, 0x28, 0x28, 0x00, 0x00, 0x00};

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, msgToSendNine);
    }

    if (m_canMsgData[2] == 2)
    {
        unsigned char frame1[8] = {16, 20, 73, 2, 1, vinStr[0], vinStr[1], vinStr[2]};
        unsigned char frame2[8] = {33, vinStr[3], vinStr[4], vinStr[5], vinStr[6], vinStr[7], vinStr[8], vinStr[9]};
        unsigned char frame3[8] = {34, vinStr[10], vinStr[11], vinStr[12], vinStr[13], vinStr[14], vinStr[15], vinStr[16]};

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, frame1);
        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, frame2);
        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, frame3);
    }
    else if (m_canMsgData[2] == 4)
    {
        unsigned char frame1[8] = {16, 20, 73, 4, 1, calibration_ID[0], calibration_ID[1], calibration_ID[2]};
        unsigned char frame2[8] = {33, calibration_ID[3], calibration_ID[4], calibration_ID[5], calibration_ID[6], calibration_ID[7], calibration_ID[8], calibration_ID[9]};
        unsigned char frame3[8] = {34, calibration_ID[10], calibration_ID[11], calibration_ID[12], calibration_ID[13], calibration_ID[14], calibration_ID[15], calibration_ID[16]};

        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, frame1);
        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, frame2);
        can.sendMsgBuf(OBD_REPLY_ID, MAX_CHAR_IN_MESSAGE, frame3);
    }
}

CANMessagesHandler::WriteCANThread::WriteCANThread(CANMessagesHandler *parent) :
    m_parent(parent)
{
}

void CANMessagesHandler::WriteCANThread::run()
{
    while(!isInterruptionRequested())
    {
        m_parent->pollLoop();
        msleep(30);
    }
}

void CANMessagesHandler::setMalfunctionIndicator(bool value) {
    if (m_malfunctionIndicator != value) {
        m_malfunctionIndicator = value;
        emit malfunctionIndicatorChanged();
    }
}
