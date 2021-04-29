#include "mico_ros/macro.h"
#include "mico_ros/sensors/temperature.h"

#include <stdio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <sensor_msgs/msg/temperature.h>

#include <functional>

using namespace micoros;

template<typename Msg>
void fill_msg_stamp(Msg& msg) {
  // absolute_time_t now = get_absolute_time();
  // absolute_time_t milliseconds = (now / 1000) % 1000;
  // absolute_time_t seconds = (((now / 1000) - milliseconds) / 1000) % 60;

  // @todo time since boot, does it even make sense?
  // msg.header.stamp.sec = (int32_t)seconds;
  // msg.header.stamp.nanosec = (int32_t)seconds;
}

rcl_publisher_t temperature_publisher;
sensor_msgs__msg__Temperature temperature_msg;

void timer_callback(rcl_timer_t *timer, int64_t /*last_call_time*/) {
  if (timer) {
    temperature_msg.temperature = read_onboard_temperature();
    fill_msg_stamp(temperature_msg);
    RCSOFTCHECK(rcl_publish(&temperature_publisher, &temperature_msg, NULL));
  } else {
    printf("Failed to publish onboard temperature. Continuing.\n");
  }
}

int main() {
  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  init_onboard_temperature();

  rcl_allocator_t allocator;
  rclc_support_t support;
  rcl_node_t node;

  rcl_timer_t timer;
  rclc_executor_t executor;

  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  RCCHECK(rclc_node_init_default(&node, "temperature_node", "pico", &support));

  // rclc_publisher_init_default is reliable
  // thus it 'takes' more time to publish
  // (back and forth acknowledgments)
  RCCHECK(
    rclc_publisher_init_best_effort(
      &temperature_publisher,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Temperature),
      "onboard_temperature"
    )
  );

  RCCHECK(
    rclc_timer_init_default(
      &timer,
      &support,
      RCL_S_TO_NS(1),
      timer_callback
    )
  );

  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));

  gpio_put(LED_PIN, 1);

  temperature_msg.header.frame_id.data = (char *)PICO_FRAME;
  temperature_msg.header.frame_id.size = sizeof(PICO_FRAME);
  temperature_msg.header.frame_id.capacity = sizeof(PICO_FRAME);

  RCCHECK(rclc_executor_spin(&executor));

	RCCHECK(rcl_publisher_fini(&temperature_publisher, &node));;
	RCCHECK(rcl_node_fini(&node));

  return EXIT_SUCCESS;
}
