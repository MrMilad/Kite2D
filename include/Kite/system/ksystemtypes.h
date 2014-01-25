#ifndef KSYSTEMTYPES_H
#define KSYSTEMTYPES_H

namespace Kite{

/* Power Sourece State */
enum KPowerStateTypes{
    KPOWER_ACON,
    KPOWER_ACOFF,
    KPOWER_UNKNOWN
};

/* Battery State */
enum KBatteryStateTypes{
    KBATTERY_USING,
    KBATTERY_CHARGING,
    KBATTERY_NOBATTERY,
    KBATTERY_UNKNOWN
};

enum KSeqIOAccTypes{
    KSEQIO_ACC_READ,
    KSEQIO_ACC_WRITE,
    KSEQIO_ACC_APPEND,
    KSEQIO_ACC_RW,
    KSEQIO_ACC_RWEX,
    KSEQIO_ACC_RWTOEND
};

enum KVectorComponent{
    KVC_X = 0,
    KVC_Y = 1,
    KVC_Z = 2,
    KVC_W = 3
};

}
#endif // KSYSTEMTYPES_H
