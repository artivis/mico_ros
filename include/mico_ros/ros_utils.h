#ifndef _MICO_ROS_MICO_ROS_ROS_UTILS_H_
#define _MICO_ROS_MICO_ROS_ROS_UTILS_H_


namespace micoros {

template<typename Msg>
void fill_msg_stamp(Msg& msg) {
  // absolute_time_t now = get_absolute_time();
  // absolute_time_t milliseconds = (now / 1000) % 1000;
  // absolute_time_t seconds = (((now / 1000) - milliseconds) / 1000) % 60;

  // @todo time since boot, does it even make sense?
  // msg.header.stamp.sec = (int32_t)seconds;
  // msg.header.stamp.nanosec = (int32_t)seconds;
}

} // namespace micoros

#endif // _MICO_ROS_MICO_ROS_ROS_UTILS_H_