// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_SENDER_AUDIO_SENDER_H_
#define MEDIA_CAST_SENDER_AUDIO_SENDER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/tick_clock.h"
#include "base/time/time.h"
#include "media/base/audio_bus.h"
#include "media/cast/cast_config.h"
#include "media/cast/sender/frame_sender.h"

namespace media {
namespace cast {

class AudioEncoder;

// Not thread safe. Only called from the main cast thread.
// This class owns all objects related to sending audio, objects that create RTP
// packets, congestion control, audio encoder, parsing and sending of
// RTCP packets.
// Additionally it posts a bunch of delayed tasks to the main thread for various
// timeouts.
class AudioSender : public FrameSender,
                    public base::NonThreadSafe,
                    public base::SupportsWeakPtr<AudioSender> {
 public:
  AudioSender(scoped_refptr<CastEnvironment> cast_environment,
              const AudioSenderConfig& audio_config,
              CastTransportSender* const transport_sender);

  ~AudioSender() override;

  CastInitializationStatus InitializationResult() const {
    return cast_initialization_status_;
  }

  // Note: It is not guaranteed that |audio_frame| will actually be encoded and
  // sent, if AudioSender detects too many frames in flight.  Therefore, clients
  // should be careful about the rate at which this method is called.
  //
  // Note: It is invalid to call this method if InitializationResult() returns
  // anything but STATUS_AUDIO_INITIALIZED.
  void InsertAudio(scoped_ptr<AudioBus> audio_bus,
                   const base::TimeTicks& recorded_time);

 protected:
  int GetNumberOfFramesInEncoder() const override;
  base::TimeDelta GetInFlightMediaDuration() const override;
  void OnAck(uint32 frame_id) override;

 private:
  // Called by the |audio_encoder_| with the next EncodedFrame to send.
  void OnEncodedAudioFrame(int encoder_bitrate,
                           scoped_ptr<EncodedFrame> encoded_frame,
                           int samples_skipped);

  // Encodes AudioBuses into EncodedFrames.
  scoped_ptr<AudioEncoder> audio_encoder_;

  // The number of audio samples enqueued in |audio_encoder_|.
  int samples_in_encoder_;

  // NOTE: Weak pointers must be invalidated before all other member variables.
  base::WeakPtrFactory<AudioSender> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(AudioSender);
};

}  // namespace cast
}  // namespace media

#endif  // MEDIA_CAST_SENDER_AUDIO_SENDER_H_
