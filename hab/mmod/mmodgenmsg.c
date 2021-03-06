/**
 * This file is automatically generated by mig. DO NOT EDIT THIS FILE.
 * This file implements the functions for encoding and decoding the
 * 'mmod_general_msg' message type. See mmodgenmsg.h for more details.
 */

#include <message.h>
#include "mmodgenmsg.h"

uint16_t MMODGENMSG_node_id_get(tmsg_t *msg)
{
  return tmsg_read_ube(msg, 0, 16);
}

void MMODGENMSG_node_id_set(tmsg_t *msg, uint16_t value)
{
  tmsg_write_ube(msg, 0, 16, value);
}

uint16_t MMODGENMSG_volt_get(tmsg_t *msg)
{
  return tmsg_read_ube(msg, 16, 16);
}

void MMODGENMSG_volt_set(tmsg_t *msg, uint16_t value)
{
  tmsg_write_ube(msg, 16, 16, value);
}

uint16_t MMODGENMSG_temp_get(tmsg_t *msg)
{
  return tmsg_read_ube(msg, 32, 16);
}

void MMODGENMSG_temp_set(tmsg_t *msg, uint16_t value)
{
  tmsg_write_ube(msg, 32, 16, value);
}

uint16_t MMODGENMSG_photo_get(tmsg_t *msg)
{
  return tmsg_read_ube(msg, 48, 16);
}

void MMODGENMSG_photo_set(tmsg_t *msg, uint16_t value)
{
  tmsg_write_ube(msg, 48, 16, value);
}

uint16_t MMODGENMSG_accel_x_get(tmsg_t *msg)
{
  return tmsg_read_ube(msg, 64, 16);
}

void MMODGENMSG_accel_x_set(tmsg_t *msg, uint16_t value)
{
  tmsg_write_ube(msg, 64, 16, value);
}

uint16_t MMODGENMSG_accel_y_get(tmsg_t *msg)
{
  return tmsg_read_ube(msg, 80, 16);
}

void MMODGENMSG_accel_y_set(tmsg_t *msg, uint16_t value)
{
  tmsg_write_ube(msg, 80, 16, value);
}

uint16_t MMODGENMSG_accel_flags_get(tmsg_t *msg)
{
  return tmsg_read_ube(msg, 96, 16);
}

void MMODGENMSG_accel_flags_set(tmsg_t *msg, uint16_t value)
{
  tmsg_write_ube(msg, 96, 16, value);
}

