#ifndef LOGFILEMANAGER_H
#define LOGFILEMANAGER_H
#include <QObject>
#include <QString>
#include <QFile>
#include <QVariantMap>
class LogFileManager : public QObject
{
    Q_OBJECT
public:
    LogFileManager(QObject *parent=0);
private:

    class LogField
    {
        public:
        enum FieldType
        {
            U08,
            S08,
            U16,
            S16,
            U32,
            S32,
            S64,
            F32,
            B08,
            B16,
            B32
        };

        QString name;
        QString units;
        FieldType type;
        uint8_t bytes;
        bool issigned;
        bool isfloat;
        float scale;
        float transform;

    };

    class DataPoint
    {
    public:
        std::string name;
        float value;
    };
    class DataLine
    {
    public:
        int time;
        std::vector<DataPoint> pointlist;
    };
    QFile *m_logFile;
    uint64_t m_blockpos;
    uint16_t infodatastart_offset;
    uint32_t databegin_offset;
    uint16_t record_length;
    uint16_t loggerfield_count;
    QList<LogField> blockordering;
signals:
    void dataLogPayloadDecoded(QVariantMap payload);
    void logFileProgress(int current, int total);
private slots:
    void timerTick();
};

#endif // LOGFILEMANAGER_H
