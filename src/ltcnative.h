#ifndef _LTCNATIVE_H
#define _LTCNATIVE_H

#define STR_INDIR(x) #x
#define STR(x) STR_INDIR(x)

#define NAPI_CALL(env, call)                                      \
  do {                                                            \
    napi_status status = (call);                                  \
    if (status != napi_ok) {                                      \
      const napi_extended_error_info* error_info = NULL;          \
      napi_get_last_error_info((env), &error_info);               \
      const char* err_message = error_info->error_message;        \
      bool is_pending;                                            \
      napi_is_exception_pending((env), &is_pending);              \
      if (!is_pending) {                                          \
        const char* message = (err_message == NULL)               \
            ? "empty error message"                               \
            : err_message;                                        \
        napi_throw_error((env), NULL, message);                   \
        return NULL;                                              \
      }                                                           \
    }                                                             \
  } while(0)

#define NAPI_STATUS_RETURN(error)\
  if (status != napi_ok) {\
    napi_throw_error(env, NULL, error); \
    return NULL;\
  }

#define NAPI_STATUS_ABORT(error)\
  if (status != napi_ok) {\
    napi_throw_error(env, NULL, error); \
    return;\
  }


#define NAPI_ERROR_RETURN(error)\
    napi_throw_error(env, NULL, error); \
    return NULL;

enum ltc_soundformat {
  LTC_SOUND_FORMAT_U8 = 0,
  LTC_SOUND_FORMAT_S16 = 1,
  LTC_SOUND_FORMAT_U16 = 2,
  LTC_SOUND_FORMAT_FL = 3,
};

struct ltc_decoder_object {
  napi_env env;
  napi_deferred read_deferred;
  napi_deferred write_deferred;
  napi_async_work read_work;
  napi_async_work write_work;
  LTCDecoder *decoder;
  LTCFrameExt frame;
  int framerate;
  int32_t apv;
  int32_t queue_size;
  enum ltc_soundformat sound_format;
  ltc_off_t position;
};

#endif