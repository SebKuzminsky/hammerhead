/**
 * This file is automatically generated by mig. DO NOT EDIT THIS FILE.
 * This file defines the layout of the 'mmod_settings_msg' message type.
 */

#ifndef MMODSETTINGSMSG_H
#define MMODSETTINGSMSG_H
#include <message.h>

enum {
  /** The default size of this message type in bytes. */
  MMODSETTINGSMSG_SIZE = 22,

  /** The Active Message type associated with this message. */
  MMODSETTINGSMSG_AM_TYPE = 51,

  /* Field node_id: type uint16_t, offset (bits) 0, size (bits) 16 */
  /** Offset (in bytes) of the field 'node_id' */
  MMODSETTINGSMSG_NODE_ID_OFFSET = 0,
  /** Offset (in bits) of the field 'node_id' */
  MMODSETTINGSMSG_NODE_ID_OFFSETBITS = 0,
  /** Size (in bytes) of the field 'node_id' */
  MMODSETTINGSMSG_NODE_ID_SIZE = 2,
  /** Size (in bits) of the field 'node_id' */
  MMODSETTINGSMSG_NODE_ID_SIZEBITS = 16,

  /* Field thres_accel: type uint16_t, offset (bits) 16, size (bits) 16 */
  /** Offset (in bytes) of the field 'thres_accel' */
  MMODSETTINGSMSG_THRES_ACCEL_OFFSET = 2,
  /** Offset (in bits) of the field 'thres_accel' */
  MMODSETTINGSMSG_THRES_ACCEL_OFFSETBITS = 16,
  /** Size (in bytes) of the field 'thres_accel' */
  MMODSETTINGSMSG_THRES_ACCEL_SIZE = 2,
  /** Size (in bits) of the field 'thres_accel' */
  MMODSETTINGSMSG_THRES_ACCEL_SIZEBITS = 16,

  /* Field sample_interval: type uint16_t, offset (bits) 32, size (bits) 16 */
  /** Offset (in bytes) of the field 'sample_interval' */
  MMODSETTINGSMSG_SAMPLE_INTERVAL_OFFSET = 4,
  /** Offset (in bits) of the field 'sample_interval' */
  MMODSETTINGSMSG_SAMPLE_INTERVAL_OFFSETBITS = 32,
  /** Size (in bytes) of the field 'sample_interval' */
  MMODSETTINGSMSG_SAMPLE_INTERVAL_SIZE = 2,
  /** Size (in bits) of the field 'sample_interval' */
  MMODSETTINGSMSG_SAMPLE_INTERVAL_SIZEBITS = 16,

  /* Field num_accel_samples: type uint16_t, offset (bits) 48, size (bits) 16 */
  /** Offset (in bytes) of the field 'num_accel_samples' */
  MMODSETTINGSMSG_NUM_ACCEL_SAMPLES_OFFSET = 6,
  /** Offset (in bits) of the field 'num_accel_samples' */
  MMODSETTINGSMSG_NUM_ACCEL_SAMPLES_OFFSETBITS = 48,
  /** Size (in bytes) of the field 'num_accel_samples' */
  MMODSETTINGSMSG_NUM_ACCEL_SAMPLES_SIZE = 2,
  /** Size (in bits) of the field 'num_accel_samples' */
  MMODSETTINGSMSG_NUM_ACCEL_SAMPLES_SIZEBITS = 16,

  /* Field accel_sample_interval: type uint16_t, offset (bits) 64, size (bits) 16 */
  /** Offset (in bytes) of the field 'accel_sample_interval' */
  MMODSETTINGSMSG_ACCEL_SAMPLE_INTERVAL_OFFSET = 8,
  /** Offset (in bits) of the field 'accel_sample_interval' */
  MMODSETTINGSMSG_ACCEL_SAMPLE_INTERVAL_OFFSETBITS = 64,
  /** Size (in bytes) of the field 'accel_sample_interval' */
  MMODSETTINGSMSG_ACCEL_SAMPLE_INTERVAL_SIZE = 2,
  /** Size (in bits) of the field 'accel_sample_interval' */
  MMODSETTINGSMSG_ACCEL_SAMPLE_INTERVAL_SIZEBITS = 16,

  /* Field heartbeat_time: type uint16_t, offset (bits) 80, size (bits) 16 */
  /** Offset (in bytes) of the field 'heartbeat_time' */
  MMODSETTINGSMSG_HEARTBEAT_TIME_OFFSET = 10,
  /** Offset (in bits) of the field 'heartbeat_time' */
  MMODSETTINGSMSG_HEARTBEAT_TIME_OFFSETBITS = 80,
  /** Size (in bytes) of the field 'heartbeat_time' */
  MMODSETTINGSMSG_HEARTBEAT_TIME_SIZE = 2,
  /** Size (in bits) of the field 'heartbeat_time' */
  MMODSETTINGSMSG_HEARTBEAT_TIME_SIZEBITS = 16,

  /* Field accel_flags: type uint16_t, offset (bits) 96, size (bits) 16 */
  /** Offset (in bytes) of the field 'accel_flags' */
  MMODSETTINGSMSG_ACCEL_FLAGS_OFFSET = 12,
  /** Offset (in bits) of the field 'accel_flags' */
  MMODSETTINGSMSG_ACCEL_FLAGS_OFFSETBITS = 96,
  /** Size (in bytes) of the field 'accel_flags' */
  MMODSETTINGSMSG_ACCEL_FLAGS_SIZE = 2,
  /** Size (in bits) of the field 'accel_flags' */
  MMODSETTINGSMSG_ACCEL_FLAGS_SIZEBITS = 16,

  /* Field tv_sec: type uint32_t, offset (bits) 112, size (bits) 32 */
  /** Offset (in bytes) of the field 'tv_sec' */
  MMODSETTINGSMSG_TV_SEC_OFFSET = 14,
  /** Offset (in bits) of the field 'tv_sec' */
  MMODSETTINGSMSG_TV_SEC_OFFSETBITS = 112,
  /** Size (in bytes) of the field 'tv_sec' */
  MMODSETTINGSMSG_TV_SEC_SIZE = 4,
  /** Size (in bits) of the field 'tv_sec' */
  MMODSETTINGSMSG_TV_SEC_SIZEBITS = 32,

  /* Field tv_usec: type uint32_t, offset (bits) 144, size (bits) 32 */
  /** Offset (in bytes) of the field 'tv_usec' */
  MMODSETTINGSMSG_TV_USEC_OFFSET = 18,
  /** Offset (in bits) of the field 'tv_usec' */
  MMODSETTINGSMSG_TV_USEC_OFFSETBITS = 144,
  /** Size (in bytes) of the field 'tv_usec' */
  MMODSETTINGSMSG_TV_USEC_SIZE = 4,
  /** Size (in bits) of the field 'tv_usec' */
  MMODSETTINGSMSG_TV_USEC_SIZEBITS = 32,
};

/**
 * Return the value of the field 'node_id'
 */
uint16_t MMODSETTINGSMSG_node_id_get(tmsg_t *msg);

/**
 * Set the value of the field 'node_id'
 */
void MMODSETTINGSMSG_node_id_set(tmsg_t *msg, uint16_t value);

/**
 * Return the value of the field 'thres_accel'
 */
uint16_t MMODSETTINGSMSG_thres_accel_get(tmsg_t *msg);

/**
 * Set the value of the field 'thres_accel'
 */
void MMODSETTINGSMSG_thres_accel_set(tmsg_t *msg, uint16_t value);

/**
 * Return the value of the field 'sample_interval'
 */
uint16_t MMODSETTINGSMSG_sample_interval_get(tmsg_t *msg);

/**
 * Set the value of the field 'sample_interval'
 */
void MMODSETTINGSMSG_sample_interval_set(tmsg_t *msg, uint16_t value);

/**
 * Return the value of the field 'num_accel_samples'
 */
uint16_t MMODSETTINGSMSG_num_accel_samples_get(tmsg_t *msg);

/**
 * Set the value of the field 'num_accel_samples'
 */
void MMODSETTINGSMSG_num_accel_samples_set(tmsg_t *msg, uint16_t value);

/**
 * Return the value of the field 'accel_sample_interval'
 */
uint16_t MMODSETTINGSMSG_accel_sample_interval_get(tmsg_t *msg);

/**
 * Set the value of the field 'accel_sample_interval'
 */
void MMODSETTINGSMSG_accel_sample_interval_set(tmsg_t *msg, uint16_t value);

/**
 * Return the value of the field 'heartbeat_time'
 */
uint16_t MMODSETTINGSMSG_heartbeat_time_get(tmsg_t *msg);

/**
 * Set the value of the field 'heartbeat_time'
 */
void MMODSETTINGSMSG_heartbeat_time_set(tmsg_t *msg, uint16_t value);

/**
 * Return the value of the field 'accel_flags'
 */
uint16_t MMODSETTINGSMSG_accel_flags_get(tmsg_t *msg);

/**
 * Set the value of the field 'accel_flags'
 */
void MMODSETTINGSMSG_accel_flags_set(tmsg_t *msg, uint16_t value);

/**
 * Return the value of the field 'tv_sec'
 */
uint32_t MMODSETTINGSMSG_tv_sec_get(tmsg_t *msg);

/**
 * Set the value of the field 'tv_sec'
 */
void MMODSETTINGSMSG_tv_sec_set(tmsg_t *msg, uint32_t value);

/**
 * Return the value of the field 'tv_usec'
 */
uint32_t MMODSETTINGSMSG_tv_usec_get(tmsg_t *msg);

/**
 * Set the value of the field 'tv_usec'
 */
void MMODSETTINGSMSG_tv_usec_set(tmsg_t *msg, uint32_t value);

#endif
