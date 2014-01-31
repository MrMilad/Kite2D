#ifndef KSYSTEMTYPES_H
#define KSYSTEMTYPES_H

namespace Kite{

/* Power Sourece State */
enum KPowerStateTypes{
    KPS_ACON,
    KPS_ACOFF,
    KPS_UNKNOWN
};

/* Battery State */
enum KBatteryStateTypes{
    KBS_USING,
    KBS_CHARGING,
    KBS_NOBATTERY,
    KBS_UNKNOWN
};

enum KSeqIOAccTypes{
    KSIOA_READ,
    KSIOA_WRITE,
    KSIOA_APPEND,
    KSIOA_RW,
    KSIOA_RWEX,
    KSIOA_RWTOEND
};

enum KVectorComponent{
    KVC_X = 0,
    KVC_Y = 1,
    KVC_Z = 2,
    KVC_W = 3
};

}
#endif // KSYSTEMTYPES_H
