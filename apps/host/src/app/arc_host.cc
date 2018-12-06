#include "arc_host.hh"

#include <opencv2/opencv.hpp>

namespace arc {
  namespace host {

const std::string ArcHost::kLogName = "arc_host";

ArcHost::ArcHost(
  const char* const my_ip_addr,      /**< [in] IP address that this side runs from */
  const char* const my_port,         /**< [in] Port that this side runs from */
  const char* const dest_ip_addr,     /**< [in] IP address of destination */
  const char* const dest_port        /**< [in] Port of destination */
) : h2ai_(Host2ArcInterface::GetInstance()),
    user_if_(),
    logger_(common::Log::RetrieveLogger(kLogName))
{
  logger_->info("initializing.");
  logger_->set_level(spdlog::level::debug);

  h2ai_.Connect(my_ip_addr, my_port, dest_ip_addr, dest_port);
}

ArcHost::~ArcHost() {
  logger_->info("terminating.");
}

void ArcHost::Spin() {
  /* Just make an opencv image to test the library */
  cv::Mat image;

  bool quit_now = false;

  /* register some callbacks to user interface */
  user_if_.AddCallback('a', [&](){
    common::AHICommandMessage msg(common::AHICommandMessage::kAhiCmdUpperRotorUp);
    logger_->debug("commanding upper rotor up.");
    h2ai_.Send(&msg);
  });
  user_if_.AddCallback('z', [&](){
    common::AHICommandMessage msg(common::AHICommandMessage::kAhiCmdUpperRotorDown);
    logger_->debug("commanding upper rotor down.");
    h2ai_.Send(&msg);
  });

  user_if_.AddCallback('s', [&](){
    common::AHICommandMessage msg(common::AHICommandMessage::kAhiCmdLowerRotorUp);
    logger_->debug("commanding lower rotor up.");
    h2ai_.Send(&msg);
  });
  user_if_.AddCallback('x', [&](){
    common::AHICommandMessage msg(common::AHICommandMessage::kAhiCmdLowerRotorDown);
    logger_->debug("commanding lower rotor down.");
    h2ai_.Send(&msg);
  });

  user_if_.AddCallback('d', [&](){
    common::AHICommandMessage msg(common::AHICommandMessage::kAhiCmdPitchForward);
    logger_->debug("commanding pitch fwd.");
    h2ai_.Send(&msg);
  });
  user_if_.AddCallback('c', [&](){
    common::AHICommandMessage msg(common::AHICommandMessage::kAhiCmdPitchBack);
    logger_->debug("commanding pitch back.");
    h2ai_.Send(&msg);
  });

  user_if_.AddCallback('q', [&](){
    common::AHICommandMessage msg(common::AHICommandMessage::kAhiCmdQuit);
    logger_->debug("commanding program termination.");
    h2ai_.Send(&msg);
  });

  /* main loop */
  while(!quit_now) {
    user_if_.ProcessInput(&quit_now);
  }
}

  } // arc::host
}