// vdp_audio
/*
 * Title:		AGON VDPAUDIO - VDP audio c header interface
 * Author:		James Higgs
 * Created:		2023-05-06
 * Last Updated:	2023-05-06
 * 
 * Modinfo:
 * 2023-05-06:		Initial version
 */

#include <defines.h>
//#include "mos-interface.h"
//#include "stdint.h"

#ifndef VDPAUDIO_H
#define VDPAUDIO_H

/* // OLD MOS 1.03 - compatible functions
// Notes:
// VDU 23, 0, 0x85, channel8, wave8, vol8, freq16, duration16
// vdu_sys_audio: void vdu_sys_audio() {
//	byte channel			// 0 to 2 (3 channels)
//	byte waveform			// ignored! (tri wave or square wave only)
//	byte volume
//	word frequency
//	word duration
UINT8 audio_play(UINT8 channel, UINT8 volume, UINT16 frequency, UINT16 duration);
 */

// MOS/VDP 1.04 audio
// https://github.com/breakintoprogram/agon-docs/wiki/VDP-%E2%80%90-Enhanced-Audio-API
// ("wave" parameter now seems to be used as a "command id")

// Command 0 - Play note
UINT8 audio_playNote(UINT8 channel, UINT8 volume, UINT16 frequency, UINT16 duration);

// Command 1 - Status
UINT8 audio_status(UINT8 channel);

// Command 2 - Set Volume
void audio_setVolume(UINT8 channel, UINT8 volume);

// Command 3 - Set frequency\
void audio_setFreq(UINT8 channel, UINT16 frequency);

// Command 4 - Set waveform
// Value  Waveform
// 0      Square wave
// 1      Triangle wave
// 2      Sawtooth wave
// 3      Sine wave
// 4      Noise (simple white noise with no frequency support)
// 5      VIC Noise (emulates a VIC6561; supports frequency)
// negative number -> set channel to use the sample id
void audio_setWaveform(UINT8 channel, INT8 waveform);

// Command 5 - Sample management
// VDU 23, 0, &85, sample, 5, 0, length; lengthHighByte, <sampleData>
UINT8 audio_loadSample(INT8 sampleId, UINT24 length, UINT8 *data);
UINT8 audio_clearSample(INT8 sampleId);

// Command 6 - Volume envelope
void audio_setVolumeEnvelope(UINT8 channel, UINT16 attack, UINT16 decay, UINT8 sustain, UINT16 release);
void audio_disableVolumeEnvelope(UINT8 channel);
 
// Command 7 - Frequency envelope
// TODO !
//void audio_setFreqEnvelope(UINT8 channel, ...);
//void audio_disableFreqEnvelope(UINT8 channel);

// Command 8 - Enable channel
void audio_enableChannel(UINT8 channel);

// Command 9 - Disable channel
void audio_disableChannel(UINT8 channel);

// Command 10 - Reset channel
void audio_resetChannel(UINT8 channel);

#endif VDPAUDIO_H