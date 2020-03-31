#ifndef _ENUMAPI_H_
#define _ENUMAPI_H_


/*  OBSERVATIONS */ 
typedef enum { 
      cbAndLed_led4_ledState=80,
      cbAndLed_led5_ledState=73,
      cbAndLed_led1_ledState=32,
      cbAndLed_led3_ledState=25,
      cbAndLed_led2_ledState=18,
      cbAndLed_led6_ledState=15,
      cbAndLed_led7_ledState=12,
      cbAndLed_led8_ledState=9,
      NUM_observation_types
} observation_types;


/*  OBSERVATION TYPES */ 
enum cbAndLed_led4_ledState_values {cbAndLed_led4_ledState_on=0, cbAndLed_led4_ledState_off=1 };
enum cbAndLed_led5_ledState_values {cbAndLed_led5_ledState_on=0, cbAndLed_led5_ledState_off=1 };
enum cbAndLed_led1_ledState_values {cbAndLed_led1_ledState_on=0, cbAndLed_led1_ledState_off=1 };
enum cbAndLed_led3_ledState_values {cbAndLed_led3_ledState_on=0, cbAndLed_led3_ledState_off=1 };
enum cbAndLed_led2_ledState_values {cbAndLed_led2_ledState_on=0, cbAndLed_led2_ledState_off=1 };
enum cbAndLed_led6_ledState_values {cbAndLed_led6_ledState_on=0, cbAndLed_led6_ledState_off=1 };
enum cbAndLed_led7_ledState_values {cbAndLed_led7_ledState_on=0, cbAndLed_led7_ledState_off=1 };
enum cbAndLed_led8_ledState_values {cbAndLed_led8_ledState_on=0, cbAndLed_led8_ledState_off=1 };


/*  COMMANDS */ 
typedef enum commands { 
      cbAndLed_cb4_cmdIn=84,
      cbAndLed_cb5_cmdIn=77,
      cbAndLed_cb11_cmdIn=70,
      cbAndLed_cb15_cmdIn=66,
      cbAndLed_cb13_cmdIn=62,
      cbAndLed_cb14_cmdIn=56,
      cbAndLed_cb12_cmdIn=52,
      cbAndLed_cb8_cmdIn=48,
      cbAndLed_cb7_cmdIn=44,
      cbAndLed_cb6_cmdIn=40,
      cbAndLed_cb1_cmdIn=36,
      cbAndLed_cb3_cmdIn=29,
      cbAndLed_cb2_cmdIn=22,
      cbAndLed_cb9_cmdIn=6,
      cbAndLed_cb10_cmdIn=2,
      NUM_command_types
} command_types;


/*  COMMAND TYPES */ 
enum cbAndLed_cb4_cmdIn_values {cbAndLed_cb4_cmdIn_noCommand=0, cbAndLed_cb4_cmdIn_on=1, cbAndLed_cb4_cmdIn_off=2, cbAndLed_cb4_cmdIn_reset=3 };
enum cbAndLed_cb5_cmdIn_values {cbAndLed_cb5_cmdIn_noCommand=0, cbAndLed_cb5_cmdIn_on=1, cbAndLed_cb5_cmdIn_off=2, cbAndLed_cb5_cmdIn_reset=3 };
enum cbAndLed_cb11_cmdIn_values {cbAndLed_cb11_cmdIn_noCommand=0, cbAndLed_cb11_cmdIn_on=1, cbAndLed_cb11_cmdIn_off=2, cbAndLed_cb11_cmdIn_reset=3 };
enum cbAndLed_cb15_cmdIn_values {cbAndLed_cb15_cmdIn_noCommand=0, cbAndLed_cb15_cmdIn_on=1, cbAndLed_cb15_cmdIn_off=2, cbAndLed_cb15_cmdIn_reset=3 };
enum cbAndLed_cb13_cmdIn_values {cbAndLed_cb13_cmdIn_noCommand=0, cbAndLed_cb13_cmdIn_on=1, cbAndLed_cb13_cmdIn_off=2, cbAndLed_cb13_cmdIn_reset=3 };
enum cbAndLed_cb14_cmdIn_values {cbAndLed_cb14_cmdIn_noCommand=0, cbAndLed_cb14_cmdIn_on=1, cbAndLed_cb14_cmdIn_off=2, cbAndLed_cb14_cmdIn_reset=3 };
enum cbAndLed_cb12_cmdIn_values {cbAndLed_cb12_cmdIn_noCommand=0, cbAndLed_cb12_cmdIn_on=1, cbAndLed_cb12_cmdIn_off=2, cbAndLed_cb12_cmdIn_reset=3 };
enum cbAndLed_cb8_cmdIn_values {cbAndLed_cb8_cmdIn_noCommand=0, cbAndLed_cb8_cmdIn_on=1, cbAndLed_cb8_cmdIn_off=2, cbAndLed_cb8_cmdIn_reset=3 };
enum cbAndLed_cb7_cmdIn_values {cbAndLed_cb7_cmdIn_noCommand=0, cbAndLed_cb7_cmdIn_on=1, cbAndLed_cb7_cmdIn_off=2, cbAndLed_cb7_cmdIn_reset=3 };
enum cbAndLed_cb6_cmdIn_values {cbAndLed_cb6_cmdIn_noCommand=0, cbAndLed_cb6_cmdIn_on=1, cbAndLed_cb6_cmdIn_off=2, cbAndLed_cb6_cmdIn_reset=3 };
enum cbAndLed_cb1_cmdIn_values {cbAndLed_cb1_cmdIn_noCommand=0, cbAndLed_cb1_cmdIn_on=1, cbAndLed_cb1_cmdIn_off=2, cbAndLed_cb1_cmdIn_reset=3 };
enum cbAndLed_cb3_cmdIn_values {cbAndLed_cb3_cmdIn_noCommand=0, cbAndLed_cb3_cmdIn_on=1, cbAndLed_cb3_cmdIn_off=2, cbAndLed_cb3_cmdIn_reset=3 };
enum cbAndLed_cb2_cmdIn_values {cbAndLed_cb2_cmdIn_noCommand=0, cbAndLed_cb2_cmdIn_on=1, cbAndLed_cb2_cmdIn_off=2, cbAndLed_cb2_cmdIn_reset=3 };
enum cbAndLed_cb9_cmdIn_values {cbAndLed_cb9_cmdIn_noCommand=0, cbAndLed_cb9_cmdIn_on=1, cbAndLed_cb9_cmdIn_off=2, cbAndLed_cb9_cmdIn_reset=3 };
enum cbAndLed_cb10_cmdIn_values {cbAndLed_cb10_cmdIn_noCommand=0, cbAndLed_cb10_cmdIn_on=1, cbAndLed_cb10_cmdIn_off=2, cbAndLed_cb10_cmdIn_reset=3 };

#endif
