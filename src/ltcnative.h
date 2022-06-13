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

#define NAPI_GETOBJPARAM_NUMBER_ORRETURN(name, saveto) \
	status = napi_get_named_property(env, args[1], name, &value); \
	if (status != napi_ok) {\
		napi_throw_error(env, NULL, "Error fetching value from property '" name "' in timecode object");\
		return NULL;\
	}\
	status = napi_typeof(env, value, &type);\
	if (type != napi_number) {\
		napi_throw_error(env, NULL, "Error fetching integer value from property '" name "' in timecode object: not numeric");\
		return NULL;\
	}\
	status = napi_get_value_int32(env, value, &intvalue);\
	if (status != napi_ok) {\
		napi_throw_error(env, NULL, "Error reading integer value from property '" name "' in timecode object");\
		return NULL;\
	}\
	saveto = intvalue;

#define NAPI_GETOBJPARAM_STRING_ORRETURN(name, saveto, length) \
	status = napi_get_named_property(env, args[1], name, &value); \
	if (status != napi_ok) {\
		napi_throw_error(env, NULL, "Error fetching value from property '" name "' in timecode object");\
		return NULL;\
	}\
	status = napi_typeof(env, value, &type);\
	if (type != napi_string) {\
		napi_throw_error(env, NULL, "Error fetching string value from property '" name "' in timecode object: not a string");\
		return NULL;\
	}\
	status = napi_get_value_string_utf8(env, value, saveto, length, &stringlength);\
	if (status != napi_ok) {\
		napi_throw_error(env, NULL, "Error reading integer value from property '" name "' in timecode object");\
		return NULL;\
	}

#define NAPI_SETOBJPARAM_NUMBER_ORRETURN(name, val) \
	status = napi_create_int32(env, val, &value); \
	if (status != napi_ok) {\
		napi_throw_error(env, NULL, "Error creating '" name "' parameter in timecode object");\
	}\
	status = napi_set_named_property(env, result, name, value); \
	if (status != napi_ok) {\
		napi_throw_error(env, NULL, "Error creating '" name "' parameter in timecode object");\
		return NULL;\
	}

#define NAPI_SETOBJPARAM_STRING_ORRETURN(name, val) \
	status = napi_create_string_utf8(env, val, NAPI_AUTO_LENGTH, &value); \
	if (status != napi_ok) {\
		napi_throw_error(env, NULL, "Error creating '" name "' parameter in timecode object");\
	}\
	status = napi_set_named_property(env, result, name, value); \
	if (status != napi_ok) {\
		napi_throw_error(env, NULL, "Error creating '" name "' parameter in timecode object");\
		return NULL;\
	}

enum ltc_soundformat {
  LTC_SOUND_FORMAT_U8 = 0,
  LTC_SOUND_FORMAT_S16 = 1,
  LTC_SOUND_FORMAT_U16 = 2,
  LTC_SOUND_FORMAT_FL = 3,
};

struct ltc_decoder_object {
  napi_env env;
  LTCDecoder *decoder;
  LTCFrameExt frame;
  int32_t apv;
  int32_t queue_size;
  enum ltc_soundformat sound_format;
  ltc_off_t position;
};

struct ltc_encoder_object {
  napi_env env;
  LTCEncoder *encoder;
  LTCFrameExt frame;
  double fps;
  double sample_rate;
  int flags;
};

#endif