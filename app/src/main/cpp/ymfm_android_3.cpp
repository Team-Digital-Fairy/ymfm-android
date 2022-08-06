#include <jni.h>

#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <list>
#include <string>

#include "ymfm_misc.h"
#include "ymfm_opl.h"
#include "ymfm_opm.h"
#include "ymfm_opn.h"

#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

const char* LOG_TAG = "ymfm_jni";

#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,  __VA_ARGS__);
#define LOG_D(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,  __VA_ARGS__);

static int16_t *buffer[2]; // buffer for audio
static uint32_t buffer_size = 192;
static uint8_t currentbuffer = 0;
static uint32_t renderedSz[2];
static uint32_t sample_rate = 44100;

static SLObjectItf engineObject;
static SLEngineItf engineEngine;
static SLObjectItf outputMixObject;

static SLObjectItf  bqPlayerObject = NULL;
static SLPlayItf    bqPlayerPlay;
static SLAndroidSimpleBufferQueueItf    bqPlayerBufferQueue;
static SLVolumeItf                      bqPlayerVolume;


// Begin android OPENGLES Thing

extern "C" {
static void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {

}

void startOpenSLES(int nsr, int fpb) {
    SLresult res;
    SLDataLocator_OutputMix loc_outMix;
    SLDataSink audioSnk;

    // Place init code here

    //

    buffer_size = fpb; // Stereo
    //buffer_size = 48000;
    sample_rate = nsr;

    LOG_D("C-Side: OpenSL start with sample rate %d, buffersz %d",sample_rate,buffer_size);

    // allocate buffer
    buffer[0] = static_cast<int16_t *>(malloc(buffer_size * sizeof(int16_t) * 2)); // buffer_size is in 16bit. malloc() returns in byte. and render in stereo.
    buffer[1] = static_cast<int16_t *>(malloc(buffer_size * sizeof(int16_t) * 2));

    memset(buffer[0],0,buffer_size * sizeof(int16_t) * 2);
    memset(buffer[1],0,buffer_size * sizeof(int16_t) * 2);

    res = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(res == SL_RESULT_SUCCESS);
    res = (*engineObject)->Realize(engineObject,SL_BOOLEAN_FALSE);
    assert(res == SL_RESULT_SUCCESS);
    res = (*engineObject)->GetInterface(engineObject,SL_IID_ENGINE,&engineEngine);
    assert(res == SL_RESULT_SUCCESS);
    res = (*engineEngine)->CreateOutputMix(engineEngine,&outputMixObject,0,NULL,NULL);
    assert(res == SL_RESULT_SUCCESS);
    res = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    assert(res == SL_RESULT_SUCCESS);

    //SLDataFormat_PCM format_pcm;
    SLDataLocator_AndroidSimpleBufferQueue locBufQ = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 8};
    SLAndroidDataFormat_PCM_EX format_pcm_ex = {
            .formatType = SL_ANDROID_DATAFORMAT_PCM_EX,
            .numChannels = 2,
            //.sampleRate = SL_SAMPLINGRATE_44_1,
            .sampleRate = nsr==48000?SL_SAMPLINGRATE_48:SL_SAMPLINGRATE_44_1,
            .bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16,
            .containerSize = SL_PCMSAMPLEFORMAT_FIXED_16,
            //.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_32,
            //.containerSize = SL_PCMSAMPLEFORMAT_FIXED_32,
            .channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            .endianness = SL_BYTEORDER_LITTLEENDIAN,
            //.representation = SL_ANDROID_PCM_REPRESENTATION_FLOAT
            .representation = SL_ANDROID_PCM_REPRESENTATION_SIGNED_INT
    };
    SLDataSource audioSrc = {
            .pLocator = &locBufQ,
            .pFormat = &format_pcm_ex
    };

    loc_outMix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
    loc_outMix.outputMix = outputMixObject;
    audioSnk.pLocator = &loc_outMix;
    audioSnk.pFormat = NULL;

    const SLInterfaceID ids[2] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME};
    const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    res = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk, 2, ids, req);
    assert(res == SL_RESULT_SUCCESS);
    res = (*bqPlayerObject)->Realize(bqPlayerObject,SL_BOOLEAN_FALSE);
    assert(res == SL_RESULT_SUCCESS);
    res = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    assert(res == SL_RESULT_SUCCESS);
    res = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE,&bqPlayerBufferQueue);
    assert(res == SL_RESULT_SUCCESS);

    // This is where you set callback.
    res = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, playerCallback, NULL);
    assert(res == SL_RESULT_SUCCESS);
    res = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
    assert(res == SL_RESULT_SUCCESS);
    res = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PAUSED);
    assert(res == SL_RESULT_SUCCESS);

    res = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, buffer[0],buffer_size * sizeof(int16_t) * 2);

    if(res != SL_RESULT_SUCCESS) {
        abort();
    }

    currentbuffer ^= 1;
}

};


// Begin JNI functions

extern "C"
JNIEXPORT void JNICALL
Java_team_digitalfairy_xcel_ymfm_1android_13_Ymfm_HelloWorld(JNIEnv *env, jobject thiz) {
    // TODO: implement HelloWorld()
}