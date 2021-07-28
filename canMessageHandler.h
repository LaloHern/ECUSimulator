#ifndef CANMessagesHandler_H
#define CANMessagesHandler_H

#include <qobject.h>
#include <QThread>
#include "mcp_can_rpi/mcp_can_rpi.h"
#include "obdDefinitions.h"

class CANMessagesHandler : public QObject
{
    Q_OBJECT

    //properties for CAN signals
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int engineRpm READ engineRpm WRITE setEngineRpm NOTIFY engineRpmChanged)
    Q_PROPERTY(int engineTemp READ engineTemp WRITE setEngineTemp NOTIFY engineTempChanged)
    Q_PROPERTY(int fuelLevel READ fuelLevel WRITE setFuelLevel NOTIFY fuelLevelChanged)
    Q_PROPERTY(bool malfunctionIndicator READ malfunctionIndicator WRITE setMalfunctionIndicator NOTIFY malfunctionIndicatorChanged)
    Q_PROPERTY(float mafRatio READ mafRatio WRITE setMafRatio NOTIFY mafRatioChanged)
    Q_PROPERTY(int intakeAirTemp READ intakeAirTemp WRITE setIntakeAirTemp NOTIFY intakeAirTempChanged)
    Q_PROPERTY(int mapPressure READ mapPressure WRITE setMapPressure NOTIFY mapPressureChanged)
    Q_PROPERTY(int malfunctionDistance READ malfunctionDistance WRITE setMalfunctionDistance NOTIFY malfunctionDistanceChanged)

    Q_DISABLE_COPY(CANMessagesHandler);

public:
    CANMessagesHandler();
    ~CANMessagesHandler();

    //INT8U *datapayload = &m_canMsgData[0];

    int speed();
    int engineRpm();
    int engineTemp();
    int fuelLevel();
    bool malfunctionIndicator();
    float mafRatio();
    int intakeAirTemp();
    int mapPressure();
    int malfunctionDistance();

    void setSpeed(int);
    void setEngineRpm(int);
    void setEngineTemp(int);
    void setFuelLevel(int);
    void setMalfunctionIndicator(bool);
    void setMafRatio(float);
    void setIntakeAirTemp(int);
    void setMapPressure(int);
    void setMalfunctionDistance(int);


    void handleOBDRequest();
    void processRealTimeDataQuery(unsigned char);
    void processFreezedDataQuery();
    void processDTCQuery();
    void eraseStoredDTC();
    void vehicleInfoReply();

private:
    MCP_CAN can;
    INT32U m_canMsgId;
    INT8U m_canMsgDLC;
    INT8U m_canMsgData[MAX_CHAR_IN_MESSAGE];

    int m_speed;
    int m_engineRpm;
    int m_engineTemp;
    int m_fuelLevel;
    int m_intakeAirTemp;
    int m_mafRatio;
    int m_mapPressure;
    int m_malfunctionDistance;

    bool m_malfunctionIndicator;

    unsigned char m_obdMode;
    unsigned char m_obdPID;

    //thread class used to separate the CAN bus write process
    //from the main program thread
    class WriteCANThread: public QThread
    {
    public:
        WriteCANThread(CANMessagesHandler *parent = 0);

    protected:
        void run();

    private:
        CANMessagesHandler *m_parent;

        friend class CANMessagesHandler;
    };

    WriteCANThread m_writeCANThread;

signals:
    void speedChanged();
    void engineRpmChanged();
    void engineTempChanged();
    void fuelLevelChanged();
    void malfunctionIndicatorChanged();
    void mafRatioChanged();
    void intakeAirTempChanged();
    void mapPressureChanged();
    void malfunctionDistanceChanged();

public slots:
    void pollLoop();
};

#endif // CANMessagesHandler_H

