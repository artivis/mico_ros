#include "mico_ros/pico_utils.h"
#include "mico_ros/macro.h"
#include "mico_ros/sensors/range.h"

#include <stdio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <sensor_msgs/msg/range.h>

using namespace micoros;

#define GPIO_ECHO 6
#define GPIO_TRIGGER 7

RangeSensor range_sensor(GPIO_ECHO, GPIO_TRIGGER);

template<typename Msg>
void fill_msg_stamp(Msg& msg) {
  // absolute_time_t now = get_absolute_time();
  // absolute_time_t milliseconds = (now / 1000) % 1000;
  // absolute_time_t seconds = (((now / 1000) - milliseconds) / 1000) % 60;

  // @todo time since boot, does it even make sense?
  // msg.header.stamp.sec = (int32_t)seconds;
  // msg.header.stamp.nanosec = (int32_t)seconds;
}

rcl_publisher_t publisher;
sensor_msgs__msg__Range range_msg;

void timer_callback(rcl_timer_t *timer, int64_t /*last_call_time*/) {
  if (timer) {
    range_msg.range = range_sensor.read();
    fill_msg_stamp(range_msg);
    RCSOFTCHECK(rcl_publish(&publisher, &range_msg, NULL));
  } else {
    printf("Failed to publish range. Continuing.\n");
  }
}

int main() {
  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  rcl_allocator_t allocator;
  rclc_support_t support;
  rcl_node_t node;

  rcl_timer_t timer;
  rclc_executor_t executor;

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
