/**
 * @file range_node.cpp
 * @author Jeremie Deray
 * @note Accompanying blog post at artivis.github.io/post/2021/pi-pico-uros-sonar
 * @date 2021-05-19
 */

#include "mico_ros/pico_utils.h"
#include "mico_ros/macro.h"
#include "mico_ros/ros_utils.h"
#include "mico_ros/sensors/range.h"

#include <stdio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rmw_uros/options.h>
#include <sensor_msgs/msg/range.h>

using namespace micoros;

#define GPIO_ECHO 6
#define GPIO_TRIGGER 7

RangeSensor range_sensor(GPIO_ECHO, GPIO_TRIGGER);

rcl_publisher_t publisher;
sensor_msgs__msg__Range range_msg;

void timer_callback(rcl_timer_t *timer, int64_t /*last_call_time*/) {
  if (timer) {
    range_msg.range = range_sensor.read();
    fill_msg_stamp(range_msg.header.stamp);
    RCSOFTCHECK(rcl_publish(&publisher, &range_msg, NULL));
  } else {
    printf("Failed to publish range. Continuing.\n");
  }
}

int main() {

  rmw_uros_set_custom_transport(
		true,
		NULL,
		pico_serial_transport_open,
		pico_serial_transport_close,
		pico_serial_transport_write,
		pico_serial_transport_read
	);

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  rcl_allocator_t allocator;
  rclc_support_t support;
  rcl_node_t node;

  rcl_timer_t timer;
  rclc_executor_t executor;

  // Wait for agent successful ping for 2 minutes.
  const int timeout_ms = 1000;
  const uint8_t attempts = 120;

  rcl_ret_t ret = rmw_uros_ping_agent(timeout_ms, attempts);

  if (ret != RCL_RET_OK) {
    while (true) {
      gpio_put(LED_PIN, 1);
      sleep_ms(500);
      gpio_put(LED_PIN, 0);
      sleep_ms(500);
    }
    // Unreachable agent, exiting program.
    return ret;
  }

  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  RCCHECK(rclc_node_init_default(&node, "range_node", "pico", &support));

  // rclc_publisher_init_default is reliable
  // thus it 'takes' more time to publish
  // (back and forth acknowledgments)
  RCCHECK(
    rclc_publisher_init_best_effort(
      &publisher,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Range),
      "range"
    )
  );

  RCCHECK(
    rclc_timer_init_default(
      &timer,
      &support,
      RCL_S_TO_NS(1./SONAR_FREQ),
      timer_callback
    )
  );

  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));

  // Synchronize time
  // @note no such thing on the pico atm
  // RCCHECK(rmw_uros_sync_session(timeout_ms));
  // int64_t time_ms = rmw_uros_epoch_millis();
  // time_t time_s = time_ms/1000;
  // //setTime(time_s);

  // parameters for the hc-sr04 module

  range_msg.radiation_type = sensor_msgs__msg__Range__ULTRASOUND;
  range_msg.field_of_view = 30; // datasheet
  range_msg.min_range = 0.02;
  range_msg.max_range = 4;

  range_msg.header.frame_id.data = (char *)SONAR_0_FRAME;
  range_msg.header.frame_id.size = sizeof(SONAR_0_FRAME);
  range_msg.header.frame_id.capacity = sizeof(SONAR_0_FRAME);

  gpio_put(LED_PIN, 1);

  RCCHECK(rclc_executor_spin(&executor));

	RCCHECK(rcl_publisher_fini(&publisher, &node));;
	RCCHECK(rcl_node_fini(&node));

  return EXIT_SUCCESS;
}
