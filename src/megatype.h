#ifndef MEGATYPE_H
#define MEGATYPE_H

typedef enum
{
    MRG_FALSE = 0,
    MRG_TRUE = 1,
}MRG_BOOL;


typedef enum
{
    BUS_VXI = 1,    /// 使用网络vxi11通信,不支持android
    BUS_SOCKET,     /// 使用网络TCP通信,支持全平台
    BUS_USB,        /// 使用USB连接,暂不支持
}GateWayBusMode;

typedef enum
{
    MMODE_NORMAL = 0,   ///普通模式(透传,MRH不会对CAN帧做任何处理)
    MMODE_MRH,          ///由模块发出的CAN帧不会直接传回上位机,而需要MRH处理
    MMODE_NMT,          ///MRH对节点进行网络配置(自动分配ID),此阶段,不响应上位机的任何命令.
}MRH_MODE_EN;

typedef enum
{
    Instruction_LINE,
    Instruction_SECTION,
}InstructionMode;

typedef enum
{
    PowerOn_DEFAULT,
    PowerOn_LAST,
}PowerOnMode;

typedef enum
{
    Interface_LAN,
    Interface_USB,
    Interface_IO,
}InterfaceMode;

typedef enum
{
    SWITCH_OFF,
    SWITCH_ON,
}SwitchState;

typedef enum
{
    Script_STOP,
    Script_RUNNING,
}ScriptState;

/*******************************************/
typedef enum
{
    RobotConfig_IDLE,
    RobotConfig_BUSY,
    RobotConfig_ERROR,
}RobotConfigState;

typedef enum
{
    MRX_TYPE_T4 = 0,    //arm
    MRX_TYPE_AS,        //megatron
    MRX_TYPE_H2,        //H2
    MRX_TYPE_DELTA,
    MRX_TYPE_RAW, //此类的机器人指的是没有构形,只有电机的机器人.这类机器人不需要空间解算,只接受PVT.
    MRX_TYPE_OTHER,
    MRX_TYPE_UNKOWN,
} MRX_TYPE;

typedef enum
{
    WAVETABLE_DEFAULT = -1,
    WAVETABLE_MIN = 0,

    WAVETABLE_MAIN = 0,
    WAVETABLE_SMALL = 1,
    WAVETABLE_P1,
    WAVETABLE_P2,
    WAVETABLE_P3,
    WAVETABLE_P4,
    WAVETABLE_P5,
    WAVETABLE_P6,
    WAVETABLE_P7,
    WAVETABLE_P8 = 9,

    WAVETABLE_MAX = WAVETABLE_P8,
}wavetable_t;


typedef enum
{
    MR_F2,
    MR_F3,
    MR_TIP,
    MR_FR,
} MRG_RobotToolType;

typedef enum
{
    TERMINAL_NORMAL,
    TERMINAL_OPEN,
} MRG_RobotTerminalMode;


/***************************************************/
typedef enum
{
    RS_IDLE,    //"IDLE"
    RS_LOADING, //"LOADING"
    RS_READY,   // "READY"
    RS_RUNNING, // "RUNNING"
    RS_STOP,    //"STOP"
    RS_ERROR,   //"ERROR"
    RS_RESERVE  //
} MRG_RobotRunState;

typedef enum
{
    MTSTATE_POWERON,
    MTSTATE_RESET,
    MTSTATE_CALCING,
    MTSTATE_CALCEND,
    MTSTATE_STANDBY,
    MTSTATE_RUNNING,
    MTSTATE_ERROR,
    MTSTATE_RESERVE,
} MRG_MT_STATE;

typedef enum
{
    MTSWITCH_RESET,
    MTSWITCH_STOP,
    MTSWITCH_RUN,
    MTSWITCH_PREPARE,
    MTSWITCH_EMERGSTOP
}MRG_MT_SWITCH;

typedef enum
{
    LSR_NONE,
    LSR_ALARM,
    LSR_STOP,
    LSR_ALARM_STOP,
}MRG_LostStepResponse;

typedef enum
{
    REPORT_ACTIVE,
    REPORT_QUERY
}MotionReportState;

typedef enum
{
    TRIGSOURCE_SOFTWARE,
    TRIGSOURCE_DIGITALIO,
    TRIGSOURCE_CAN,
    TRIGSOURCE_ALL,
}MotionTrigSource;


typedef enum
{
    TRIGGER_PATTERN,
    TRIGGER_LEVEL,
}MRG_TriggerMode;

typedef enum
{
    TRIGGER_RESERVE,
    TRIGGER_LOW,
    TRIGGER_RISE,
    TRIGGER_FALL,
    TRIGGER_HIGH,
}MRG_TriggerType;

typedef enum
{
    StepAngle_1_dot_8,
    StepAngle_0_dot_8,
    StepAngle_15,
    StepAngle_7_dot_5,
}MotorStepAngle;


typedef enum
{
    MotorMotion_ROTARY,
    MotorMotion_LINEAR,
}MotorMotionType;

typedef enum
{
    MotorPositionUnit_ANGLE,
    MotorPositionUnit_RADIAN,
    MotorPositionUnit_MILLIMETER,
}MotorPositionUnit;

typedef enum
{
    DRIVER_D17,
    DRIVER_D23,
}MRG_DriverType;

typedef enum
{
    DTMINE_1_PER_2,
    DTMINE_1_PER_4,
}MRG_DriverTuningMinEle;

typedef enum
{
    ENCODER_NONE,
    ENCODER_OFF,
    ENCODER_ON,
}MRG_EncoderState;

typedef enum
{
    ENCODER_INCREMENTAL,
    ENCODER_ABSOLUTE,
}MRG_EncoderType;

typedef enum
{
    ENCODER_SINGLE,
    ENCODER_DOUBLE,
    ENCODER_QUADRUPLE,
}MRG_EncoderMultiple;

typedef enum
{
    ENCODER_POSITIVE,
    ENCODER_NEGATIVE,
}MRG_EncoderDirection;


#endif // MEGATYPE_H
